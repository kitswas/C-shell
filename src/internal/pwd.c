/**
 * @file pwd.c
 * @author Swastik Pal
 * @brief
 * @version 0.1
 * @date 2022-11-07
 */

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../internal_commands.h"

extern int opterr, optind, optopt;
extern char *optarg;

int pwd(int nargs, char **args)
{
	option use_env_pwd = false;
	option use_cwd = true;
	opterr = 0;
	optind = 0;
	int c = -1;

	// deal with the options
	while ((c = getopt(nargs, args, "LP")) != -1)
		switch (c)
		{
		case 'L':
			use_env_pwd = true;
			break;
		case 'P':
			use_cwd = true;
			break;
		case '?':
			// if (optopt == 'c')
			// 	fprintf(stderr, "Option -%c requires an argument.\n", optopt);
			// else
			if (isprint(optopt))
				fprintf(stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
			return 1;
		default:
			fprintf(stderr, "Aborting in pwd...");
			abort();
		}

	if (use_env_pwd)
	{
		printf("%s\n", getenv("PWD"));
	}
	else
	{
		char buffer[1024];
		printf("%s\n", getcwd(buffer, 1024));
		// free(buffer);
	}
	return 0;
}
