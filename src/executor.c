#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include "internal_commands.h"
#include "internal/history.h"
#include "job_store.h"
#include "main.h"
#include "redirection.h"
#include "shelltypes.h"
#include "signals.h"
#include "terminal.h"

pid_t shell_pgid;
int shell_terminal;

int execute(struct command *cmd)
{
	char *command = cmd->argv[0];
	char **args = cmd->argv;
	int nargs = cmd->nargs;

	pid_t child_pid = fork();
	if (child_pid == 0)
	{
		if (cmd->fd_in != STDIN_FILENO)
		{
			dup2(cmd->fd_in, STDIN_FILENO);
			close(cmd->fd_in);
		}
		if (cmd->fd_out != STDOUT_FILENO)
		{
			dup2(cmd->fd_out, STDOUT_FILENO);
			close(cmd->fd_out);
		}
		if (cmd->fd_err != STDERR_FILENO)
		{
			dup2(cmd->fd_err, STDERR_FILENO);
			close(cmd->fd_err);
		}
		// Reset signals that were set to ignore in the main shell.
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGTSTP, SIG_DFL);
		signal(SIGTTIN, SIG_DFL);
		signal(SIGTTOU, SIG_DFL);
		signal(SIGCHLD, SIG_DFL);

		if (!strcasecmp(command, "echo"))
		{
			echo(nargs, args);
		}
		else if (!strcasecmp(command, "history"))
		{
			history(nargs, args);
		}
		else if (!strcasecmp(command, "ls"))
		{
			ls(nargs, args);
		}
		else if (!strcasecmp(command, "pwd"))
		{
			pwd(nargs, args);
		}
		else if (!strcasecmp(command, "pinfo"))
		{
			pid_t lookup_pid;
			if (nargs == 1)
			{
				lookup_pid = shell_pgid;
			}
			else
			{
				if (sscanf(args[1], "%d", &lookup_pid) != 1)
				{
					fprintf(stderr, "Invalid PID %s\n", args[1]);
					exit(EXIT_FAILURE);
				}
			}
			pinfo(lookup_pid);
		}
		else
		{
			errno = 0;
			if (execvp(command, args))
				switch (errno)
				{
				case 0:
					// do nothing
					break;
				case ENOENT:
					fprintf(stderr, "Command %s not found.\n", command);
					break;
				default:
					fprintf(stderr, "[ERROR] %d %s\n", errno, strerror(errno));
					break;
				}
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		cmd->pid = child_pid;
		// printf("Command %s started with PID %d in process group %d\n", command, child_pid, getpgrp()); // debugging only
		int status = 0;
		do
		{
			waitpid(cmd->pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));

		return status;
	}

	return 0;
}

void init_shell()
{
	shell_terminal = STDIN_FILENO;
	// Loop until the shell is in the foreground.
	while (tcgetpgrp(shell_terminal) != (shell_pgid = getpgrp()))
		kill(-shell_pgid, SIGTTIN);
	// Handle interactive and job-control signals.
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, handle_sigtstp);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGCHLD, handle_sigchld);
	// Put the shell in its own process group.
	shell_pgid = getpid();
	if (setpgid(shell_pgid, shell_pgid) < 0)
	{
		perror("Couldn't put the shell in its own process group");
		exit(1);
	}
	/* Grab control of the terminal. */
	tcsetpgrp(shell_terminal, shell_pgid);
}

void launch_job(struct job *j)
{
	struct command *cmd = j->first_command;

	char *command = cmd->argv[0];
	char **args = cmd->argv;
	int nargs = cmd->nargs;

	if (!strcasecmp(command, "cd"))
	{
		cd(nargs, args);
	}
	else if (!strcasecmp(command, "cls"))
	{
		cls;
	}
	else if (!strcasecmp(command, "exit"))
	{
		write_history_to_file();
		printf("\033[0m"); // reset all terminal attributes
		exit(EXIT_SUCCESS);
	}
	else if (!strcasecmp(command, "jobs"))
	{
		list_jobs();
	}
	else
	{
		int pipe_des[2];

		pid_t child_pid = fork();
		if (child_pid == 0)
		{
			// sleep(60); // debugging only - time to attach debugger
			while (cmd)
			{
				if (cmd->next)
				{
					if (pipe(pipe_des))
					{
						fprintf(stderr, "[ERROR] Unable to create pipe.\n");
						return;
					}
					cmd->fd_out = pipe_des[1];
					cmd->next->fd_in = pipe_des[0];
				}
				else
				{
					cmd->fd_out = STDOUT_FILENO;
				}

				if (handle_redirection(cmd) < 0)
					exit(EXIT_FAILURE);
				execute(cmd);

				if (cmd->fd_in != STDIN_FILENO)
					close(cmd->fd_in);
				if (cmd->fd_out != STDOUT_FILENO)
					close(cmd->fd_out);

				cmd = cmd->next;
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			j->pgid = child_pid;
			// printf("Job %s started with PID %d\n", j->user_command, child_pid); // debugging only
			setpgid(child_pid, child_pid);

			add_jobs_to_store(j);
			if (!j->background)
			{
				tcsetpgrp(STDIN_FILENO, child_pid);
				// Send a continue signal to the job. -(j->pgid) means the process group ID.
				if (kill(-j->pgid, SIGCONT) < 0)
					perror("kill (SIGCONT)");
				int status = 0;
				do
				{
					waitpid(child_pid, &status, WUNTRACED);
				} while (!WIFEXITED(status) && !WIFSIGNALED(status) && !WIFSTOPPED(status));
				if (WIFEXITED(status))
				{
					remove_job_from_store(j->pgid);
				}

				tcsetpgrp(STDIN_FILENO, getpgrp());
			}
			else
			{
				printf("%d\n", j->pgid);
			}
		}
	}
}
