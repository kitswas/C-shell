#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shelltypes.h"

void show_error(char *file)
{
	switch (errno)
	{
	case 0:
		// do nothing
		break;
	case EACCES:
		fprintf(stderr, "[REDIRECT ERROR] %s: Permission denied\n", file);
		break;
	case ENOENT:
		fprintf(stderr, "[REDIRECT ERROR] %s: No such file or directory\n", file);
		break;
	default:
		fprintf(stderr, "[REDIRECT ERROR] %s: %s\n", file, strerror(errno));
		break;
	}
}

int handle_redirection(struct command *cmd)
{
	int i;
	int fd;
	for (i = 0; i < cmd->nargs; ++i)
	{
		if (!strcmp(cmd->argv[i], "<"))
		{
			if (cmd->fd_in != STDIN_FILENO)
			{
				fprintf(stderr, "[REDIRECT ERROR] Multiple input redirections\n");
				return -1;
			}
			if (i + 1 >= cmd->nargs)
			{
				fprintf(stderr, "[REDIRECT ERROR] No input file specified\n");
				return -1;
			}
			fd = open(cmd->argv[i + 1], O_RDONLY);
			if (fd == -1)
			{
				show_error(cmd->argv[i + 1]);
				return -1;
			}
			cmd->fd_in = fd;
			cmd->nargs -= 2;
			// Left shift the arguments
			for (int j = i; j < cmd->nargs; ++j)
			{
				cmd->argv[j] = cmd->argv[j + 2];
			}
			cmd->argv = realloc(cmd->argv, (size_t)(cmd->nargs + 1) * sizeof(char *));
			cmd->argv[cmd->nargs] = NULL; // Null-terminate the arguments
		}
		else if (!strcmp(cmd->argv[i], ">"))
		{
			if (cmd->fd_out != STDOUT_FILENO)
			{
				fprintf(stderr, "[REDIRECT ERROR] Multiple output redirections\n");
				return -1;
			}
			if (i + 1 >= cmd->nargs)
			{
				fprintf(stderr, "[REDIRECT ERROR] No output file specified\n");
				return -1;
			}
			fd = open(cmd->argv[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				show_error(cmd->argv[i + 1]);
				return -1;
			}
			cmd->fd_out = fd;
			cmd->nargs -= 2;
			// Left shift the arguments
			for (int j = i; j < cmd->nargs; ++j)
			{
				cmd->argv[j] = cmd->argv[j + 2];
			}
			cmd->argv = realloc(cmd->argv, (size_t)(cmd->nargs + 1) * sizeof(char *));
			cmd->argv[cmd->nargs] = NULL; // Null-terminate the arguments
		}
		else if (!strcmp(cmd->argv[i], "2>"))
		{
			if (cmd->fd_err != STDERR_FILENO)
			{
				fprintf(stderr, "[REDIRECT ERROR] Multiple error redirections\n");
				return -1;
			}
			if (i + 1 >= cmd->nargs)
			{
				fprintf(stderr, "[REDIRECT ERROR] No error file specified\n");
				return -1;
			}
			fd = open(cmd->argv[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				show_error(cmd->argv[i + 1]);
				return -1;
			}
			cmd->fd_err = fd;
			cmd->nargs -= 2;
			// Left shift the arguments
			for (int j = i; j < cmd->nargs; ++j)
			{
				cmd->argv[j] = cmd->argv[j + 2];
			}
			cmd->argv = realloc(cmd->argv, (size_t)(cmd->nargs + 1) * sizeof(char *));
			cmd->argv[cmd->nargs] = NULL; // Null-terminate the arguments
		}
	}
	return 0;
}
