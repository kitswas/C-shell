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

	int stdin, stdout, stderr;
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
	/** True if user has been notified about stopped job. */
	char notified;
};

/** A process represents a single external process. */
struct process
{
	/** Arguments for execution. */
	char **argv;
	/** Process ID. */
	pid_t pid;
	/** True if process has completed. */
	char completed;
	/** True if process has stopped. */
	char stopped;
	/** Reported status value. */
	int status;
};

// functions here

#endif
