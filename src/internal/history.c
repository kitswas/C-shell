#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "history.h"

const char *const history_file = ".chistory";

struct history_list h_list;

void add_to_history(char *line)
{
	if (h_list.count < MAX_HISTORY_ITEMS)
	{
		asprintf(&(h_list.hist[h_list.count]), "%s", line); //! allocate memory and deep copy
		h_list.count++;
	}
	else
	{
		free(h_list.hist[0]); //! free the memory
		for (int i = 0; i < MAX_HISTORY_ITEMS - 1; i++)
		{
			h_list.hist[i] = h_list.hist[i + 1];
		}
		asprintf(&(h_list.hist[MAX_HISTORY_ITEMS - 1]), "%s", line); //! allocate memory and deep copy
	}
}

void clear_history()
{
	for (int i = 0; i < h_list.count; i++)
	{
		free(h_list.hist[i]); //! free the memory
		h_list.hist[i] = NULL;
	}
	h_list.count = 0;
}

const struct history_list *get_history()
{
	return &h_list;
}

void print_history(int n)
{
	n = n < h_list.count ? n : h_list.count;
	for (int i = h_list.count - n; i < h_list.count; i++)
	{
		printf("\t%d: %s\n", i + 1, h_list.hist[i]);
	}
}

int read_history_from_file()
{
	FILE *f = fopen(history_file, "r");
	if (f == NULL)
	{
		switch (errno)
		{
		case ENOENT:
			return 0;

		default:
			fprintf(stderr, "[ERROR] %d %s\n", errno, strerror(errno));
			return -1;
		}
	}

	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	while ((read = getline(&line, &len, f)) != -1)
	{
		line[read - 1] = '\0'; // remove the newline character
		add_to_history(line);
	}

	free(line);
	fclose(f);
	return 0;
}

int write_history_to_file()
{
	FILE *f = fopen(history_file, "w");
	if (f == NULL)
	{
		fprintf(stderr, "[ERROR] %d %s\n", errno, strerror(errno));
		return -1;
	}

	for (int i = 0; i < h_list.count; i++)
	{
		fprintf(f, "%s\n", h_list.hist[i]);
	}

	fclose(f);
	return 0;
}

extern int opterr, optind, optopt;
extern char *optarg;

int history(int nargs, char **args)
{
	opterr = 0;
	optind = 0;
	int c = -1;

	// deal with the options
	while ((c = getopt(nargs, args, "crw")) != -1)
		switch (c)
		{
		case 'c':
			clear_history();
			break;
		case 'r':
			read_history_from_file();
			break;
		case 'w':
			write_history_to_file();
			break;
		case '?':
			if (isprint(optopt))
				fprintf(stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
			return 1;
		default:
			fprintf(stderr, "Aborting in ls...");
			abort();
		}

	if (optind > 1) // If we handled any options, we are done
		return 0;

	errno = 0;
	if (optind < nargs)
	{
		int n;
		if (sscanf(args[optind], "%d", &n) == 1)
			print_history(n);
		else
			fprintf(stderr, "Invalid argument to history: %s\n", args[optind]);
	}
	else
	{
		print_history(DEFAULT_HIST_ITEMS);
	}

	switch (errno)
	{
	case 0:
		// do nothing
		break;
	default:
		fprintf(stderr, "[ERROR] %d %s\n", errno, strerror(errno));
		break;
	}

	return 0;
}
