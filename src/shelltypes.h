#ifndef SHELLTYPES_H
#define SHELLTYPES_H

#include <stdbool.h>
#include <sys/types.h>

// macros here

// global variables here

// structs here

struct command
{
	/** The next command in the pipeline. */
	struct command *next;

	int nargs;
	char **argv;

	pid_t pid;

	int fd_in, fd_out, fd_err;
};

/** A job is a pipeline of commands. */
struct job
{
	/** Next active job */
	struct job *next;
	/** The command that created this job. */
	char *user_command;
	/** List of commands in this job. */
	struct command *first_command;
	/** Process group ID. */
	pid_t pgid;
	/** True if background job. */
	bool background;
};

// functions here

#endif
