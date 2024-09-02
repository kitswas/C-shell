#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "../internal_commands.h"

int pinfo(pid_t pid)
{
	if (pid == -1) // If pid is -1, use the parent process id
	{
		pid = getppid();
	}
	else if (pid == 0) // If pid is 0, use the current process id
	{
		pid = getpid();
	}

	// Read the process status from /proc/pid/status
	char path[100];
	sprintf(path, "/proc/%d/status", pid);
	FILE *file = fopen(path, "r");
	if (file == NULL)
	{
		switch (errno)
		{
		case 0:
			// do nothing
			break;
		case ENOENT:
			fprintf(stdout, "Process not found.\n");
			break;
		default:
			fprintf(stderr, "[ERROR] Unable to read process info: %s\n", strerror(errno));
			break;
		}
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
		fprintf(stderr, "[ERROR] Unable to read executable path: %s\n", strerror(errno));
		return -1;
	}

	return 0;
}
