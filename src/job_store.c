#include <stdlib.h>
#include <stdio.h>
#include "job_store.h"

/**
 * @brief This is the head of the linked list of jobs.
 *
 */
struct job *job_store = NULL;

struct job *get_job(pid_t pgid)
{
	struct job *current = job_store;
	while (current && current->pgid != pgid)
	{
		current = current->next;
	}
	if (current->pgid == pgid)
		return current;
	else
		return NULL;
}

void add_jobs_to_store(struct job *j)
{
	if (job_store == NULL)
	{
		job_store = j;
		return;
	}

	struct job *current = job_store;
	while (current->next)
	{
		current = current->next;
	}
	current->next = j;
}

/**
 * @brief Free all allocated memory for a job struct.
 *
 */
void free_job(struct job *j)
{
	free(j->user_command);
	while (j->first_command)
	{
		struct command *cmd = j->first_command;
		j->first_command = cmd->next;
		for (int i = 0; i < cmd->nargs; ++i)
		{
			free(cmd->argv[i]);
		}
		free(cmd);
	}
	free(j);
}

void list_jobs()
{
	struct job *current = job_store;
	while (current)
	{
		printf("Job: %s\tPID: %d\n", current->user_command, current->pgid);
		current = current->next;
	}
}

void remove_job_from_store(pid_t pgid)
{
	struct job *current = job_store;
	struct job *prev = NULL;

	while (current && current->pgid != pgid)
	{
		prev = current;
		current = current->next;
	}
	if (current && current->pgid == pgid)
	{
		if (prev)
		{
			prev->next = current->next;
		}
		else
		{
			job_store = current->next;
		}
		free_job(current);
	}
}
