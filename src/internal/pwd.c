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
			// this is no different from the default behavior
			break;
		case '?':
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
		char *buffer = NULL;
		printf("%s\n", getcwd(buffer, 0)); // let GNU handle the memory allocation
		free(buffer);
	}
	return 0;
}
