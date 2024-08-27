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
