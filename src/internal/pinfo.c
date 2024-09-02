#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "../internal_commands.h"

/**
 * Sample output:
 *
 * pid -- 231
 * Process Status -- {R/S/S+/Z}
 * memory -- 67854 {Virtual Memory}
 * Executable Path -- ~/a.out
 *
 * Process status codes:
 * 1. R/R+: Running
 * 2. S/S+: Sleeping in an interruptible wait
 * 3. Z: Zombie
 * 4. T: Stopped (on a signal)
 * Note: “+” must be added to the status code if the process is in the foreground
 *
 * @param pid pid of the process
 * @return int 0 on success, -1 on failure
 */
int print_process_info(pid_t pid)
{
	char path[100];
	sprintf(path, "/proc/%d/status", pid);
	FILE *file = fopen(path, "r");
	if (file == NULL)
	{
		fprintf(stderr, "Error opening file: %s\n", strerror(errno));
		return -1;
	}

	bool is_foreground = false;
	is_foreground = getpgid(pid) == tcgetpgrp(STDIN_FILENO);

	printf("pid -- %d\n", pid);

	char line[256];
	while (fgets(line, sizeof(line), file))
	{
		if (strstr(line, "State:"))
		{
			printf("Process Status -- {");
			char *token = strtok(line, " \t");
			token = strtok(NULL, " \t");
			printf("%s", token);
			if (is_foreground)
				printf("+");
			printf("}\n");
		}
		else if (strstr(line, "VmSize:"))
		{
			printf("memory -- ");
			char *token = strtok(line, " \t");
			token = strtok(NULL, " \t");
			printf("%s {Virtual Memory}\n", token);
		}
	}
	fclose(file);

	// Get the executable path from /proc/pid/exe by reading the symbolic link
	char exe_path[100];
	sprintf(exe_path, "/proc/%d/exe", pid);
	char exe_path_buf[100];
	ssize_t len = readlink(exe_path, exe_path_buf, sizeof(exe_path_buf) - 1);
	if (len != -1)
	{
		exe_path_buf[len] = '\0';
		printf("Executable Path -- %s\n", exe_path_buf);
	}
	else
	{
		fprintf(stderr, "Error reading symbolic link: %s\n", strerror(errno));
		return -1;
	}

	return 0;
}

extern int opterr, optind, optopt;
extern char *optarg;

int pinfo(int nargs, char **args)
{
	opterr = 0;
	optind = 0;
	int c = -1;

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
			fprintf(stderr, "Aborting in pinfo...");
			abort();
		}

	errno = 0;
	if (optind < nargs)
	{
		pid_t pid;
		if (sscanf(args[optind], "%d", &pid) == 1)
			print_process_info(pid);
		else
			fprintf(stderr, "Invalid argument to pinfo: %s\n", args[optind]);
	}
	else
		print_process_info(getpid());

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
