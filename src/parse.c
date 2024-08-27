#include "parse.h"
#include <stdlib.h>
#include <string.h>

char **parse_command(char *command, size_t command_size, int *nargs)
{
	char **tokens = malloc(command_size * sizeof(*tokens));
	char *token = strtok(command, C_SHELL_TOK_DELIM);
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

char **parse_line(char *line, size_t line_size, int *ncmds)
{
	char **commands = malloc(line_size * sizeof(*commands));
	char *command = strtok(line, COMMAND_DELIM);
	int position = 0;

	do
	{
		commands[position] = command;
		++position;
		command = strtok(NULL, COMMAND_DELIM);
	} while (command != NULL);
	*ncmds = position;
	commands[position] = NULL; // NULL-terminate the list of strings

	return commands;
}
