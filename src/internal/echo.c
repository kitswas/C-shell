/**
 * @file echo.c
 * @author Swastik Pal
 * @brief
 * @version 0.1
 * @date 2022-11-05
 */

#include <stdio.h>
#include <string.h>
#include "../internal_commands.h"

int echo(int nargs, char **args)
{
	int start_at = 1;

	option no_newline = false;
	option use_comma = false;
	option as_list = false;

	// check for options
	while (start_at < nargs && strlen(args[start_at]) > 1 && args[start_at][0] == '-') // consume options
	{
		// we have options
		switch (args[start_at][1])
		{
		case 'n':
			no_newline = true;
			break;
		case 'c':
			use_comma = true;
			break;
		case 'l':
			as_list = true;
			break;
		default:
			break;
		}
		start_at++;
	}

	for (int i = 1; start_at < nargs; start_at++, i++)
	{
		if (as_list)
			printf("%d. ", i);
		printf("%s", args[start_at]);
		if (use_comma && start_at < nargs - 1)
		{
			printf(",");
		}
		printf("%c", as_list ? '\n' : ' ');
	}
	if (!no_newline)
		printf("\n");
	return 0;
}
