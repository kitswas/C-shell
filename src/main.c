/**
 * @file main.c
 * @brief The entrypoint to the program.
 * @author Swastik Pal
 * @version 0.1
 * @date 2022-11-03
 *
 * @copyright Copyright (c) 2022
 *
 * Comments _can_ be written in [**Markdown**](https://www.markdownguide.org/cheat-sheet/).
 */

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "internal_commands.h"
#include "main.h"

/**
 * @brief Execution starts here
 */
int main()
{
	cls;
	// load config files, if any.
	load_settings();
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
		printf("\033[0m"); // reset all terminal attributes
		exit(EXIT_SUCCESS);
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
}

int loop()
{
	while (true)
	{
		print_prompt();

		size_t line_size = LINE_BUFFER_SIZE;
		char *line = malloc(line_size * sizeof(*line));
		read_line(line, line_size);
		// printf("You entered: %s\n", line); // debuggging only

		int nargs = 0;
		char **args = parse_line(line, line_size, &nargs);
		// for (size_t i = 0; i < nargs; i++) // debuggging only
		// {
		// 	printf("You entered: %s\n", args[i]);
		// }

		int status = 0;
		if (nargs > 0 && args[0] != NULL) // ignore empty lines and NULL commands
			status = execute(nargs, args[0], args);

		free(line);
		free(args);
	}
}

char **parse_line(char *line, size_t line_size, int *nargs)
{
	char **tokens = malloc(line_size * sizeof(*tokens));
	char *token = strtok(line, C_SHELL_TOK_DELIM);
	int position = 0;

	do
	{
		tokens[position] = token;
		++position;
		token = strtok(NULL, C_SHELL_TOK_DELIM);
	} while (token != NULL);
	*nargs = position;
	tokens[position] = NULL; // NULL-terminate the list of strings

	return tokens;
}

int print_prompt()
{
	char hostname[MAXHOSTNAMELEN];
	gethostname(hostname, MAXHOSTNAMELEN);
	printf("%s@%s$ ", getenv("USER"), hostname);
}

int read_line(char *buffer, int buffer_size)
{
	int ch = 0;
	int i = 0;
	do
	{
		ch = getchar();
		buffer[i] = ch;
		++i;
	} while (ch != '\n' && ch != EOF && i < buffer_size);
	buffer[i - 1] = '\0';			// null terminate the string
	while (ch != '\n' && ch != EOF) // discard the rest of the input line
		ch = getchar();
	return i;
}
