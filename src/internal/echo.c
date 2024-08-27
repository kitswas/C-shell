#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../internal_commands.h"

extern int opterr, optind, optopt;
extern char *optarg;

int echo(int nargs, char **args)
{
	option no_newline = false;
	option use_comma = false;
	option as_list = false;
	opterr = 0;
	optind = 0;
	int c = -1;

	// deal with the options
	while ((c = getopt(nargs, args, "ncl")) != -1)
		switch (c)
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
			fprintf(stderr, "Aborting in echo...");
			abort();
		}

	for (int i = 1; optind < nargs; optind++, i++)
	{
		if (as_list)
			printf("%d. ", i);
		printf("%s", args[optind]);
		if (use_comma && optind < nargs - 1)
		{
			printf(",");
		}
		printf("%c", as_list ? '\n' : ' ');
	}
	if (!no_newline)
		printf("\n");
	return 0;
}
