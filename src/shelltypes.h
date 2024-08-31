#ifndef SHELLTYPES_H
#define SHELLTYPES_H

#include <sys/types.h>

// macros here

// global variables here

struct command
{
	/** The next command in the pipeline. */
	struct command *next;

	/** The actual executable can be a shell built-in or a program. */
	union
	{
		/** The process to start. */
		char **argv;
		/** For the shell's internal use. */
		char *internal_command;
	} execution_unit;

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

// structs here

// functions here

#endif
