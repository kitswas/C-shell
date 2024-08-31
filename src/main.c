/**
 * @file main.c
 * @brief The entrypoint to the program.
 * @author Swastik Pal
 * @date 2022-11-03
 *
 * @copyright Copyright (c) 2022
 */

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "internal_commands.h"
#include "internal/history.h"
#include "main.h"
#include "parse.h"
#include "shelltypes.h"
#include "terminal.h"

/**
 * @brief Execution starts here
 */
int main()
{
	cls;
	// load config files, if any.
	load_settings();
	read_history_from_file();
	// loop
	loop();
	// cleanup
	return 0;
}

int execute(int nargs, char *command, char **args)
{
	if (!strcasecmp(command, "cd"))
	{
		cd(nargs, args);
	}
	else if (!strcasecmp(command, "cls"))
	{
		cls;
	}
	else if (!strcasecmp(command, "echo"))
	{
		echo(nargs, args);
	}
	else if (!strcasecmp(command, "exit"))
	{
		write_history_to_file();
		printf("\033[0m"); // reset all terminal attributes
		exit(EXIT_SUCCESS);
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
	else
	{
		pid_t child_pid = fork();
		if (child_pid == 0)
		{
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
			int status = 0;
			waitpid(child_pid, &status, 0);
			return status;
		}
	}

	return 0;
}

int load_settings()
{
	// not yet implemented
	return 0;
}

int loop()
{
	while (true)
	{
		print_prompt();

		size_t line_size = LINE_BUFFER_SIZE;
		char *line = calloc(line_size, sizeof(*line));
		read_line(line, line_size);
		// printf("You entered: %s\n", line); // debuggging only

		if (line[0] == '\0')
			continue; // ignore empty lines

		add_to_history(line);

		struct job *j = parse_line_to_jobs(line);
		while (j)
		{
			printf("Job: %s\n", j->user_command); // debugging only
			struct command *c = j->first_command;
			while (c)
			{
				printf("Command: %s\n", c->argv[0]); // debugging only
				c = c->next;
			}
			j = j->next;
		}
	}
}

int print_prompt()
{
	char hostname[MAXHOSTNAMELEN];
	gethostname(hostname, MAXHOSTNAMELEN);
	char *current_dir = getcwd(NULL, 0);
	return printf("%s@%s:%s> ", getenv("USER"), hostname, current_dir);
}
