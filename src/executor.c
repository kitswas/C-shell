#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include "internal_commands.h"
#include "internal/history.h"
#include "main.h"
#include "shelltypes.h"
#include "terminal.h"

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

		errno = 0;
		if (execvp(command, args))
			switch (errno)
			{
			case 0:
				// do nothing
				break;
			case ENOENT:
				fprintf(stderr, "Command not found.\n");
				break;
			default:
				fprintf(stderr, "[ERROR] %d %s\n", errno, strerror(errno));
				break;
			}
		exit(EXIT_SUCCESS);
	}
	else
	{
		cmd->pid = child_pid;
		int status = 0;
		do
		{
			waitpid(cmd->pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));

		return status;
	}

	return 0;
}

void launch_job(struct job *j)
{
	struct command *cmd = j->first_command;
	int pipe_des[2];
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

		execute(cmd);

		if (cmd->fd_in != STDIN_FILENO)
			close(cmd->fd_in);
		if (cmd->fd_out != STDOUT_FILENO)
			close(cmd->fd_out);

		cmd = cmd->next;
	}
}
