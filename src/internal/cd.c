/**
 * @file cd.c
 * @author Swastik Pal
 * @brief
 * @version 0.1
 * @date 2022-11-09
 */

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../internal_commands.h"

extern int opterr, optind, optopt;
extern char *optarg;

int cd(int nargs, char **args)
{
	opterr = 0;
	optind = 0;
	int c = -1;

	const char *const default_path = "/";

	// deal with the options
	while ((c = getopt(nargs, args, "")) != -1)
		switch (c)
		{
		case '?':
			if (isprint(optopt))
				fprintf(stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
			return 1;
		default:
			fprintf(stderr, "Aborting in cd...");
			abort();
		}

	errno = 0;
	if (optind < nargs)
	{
		chdir(args[optind]);
	}
	else
	{
		chdir(default_path);
	}
	switch (errno)
	{
	case 0:
		// do nothing
		break;
	case ENOTDIR:
		fprintf(stderr, "Not a directory.\n");
		break;
	default:
		fprintf(stderr, "[ERROR] %d %s\n", errno, strerror(errno));
		break;
	}

	return 0;
}
