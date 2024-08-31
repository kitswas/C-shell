#include "parse.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "shelltypes.h"

struct command *parse_command(char *command, size_t command_len)
{
	struct command *cmd = malloc(sizeof(*cmd));
	cmd->next = NULL;
	cmd->nargs = 0;
	cmd->argv = NULL;
	cmd->stdin = STDIN_FILENO;
	cmd->stdout = STDOUT_FILENO;
	cmd->stderr = STDERR_FILENO;

	char **tokens = calloc(command_len, sizeof(*tokens));
	char *token = strtok(command, C_SHELL_TOK_DELIM);
	int position = 0;

	do
	{
		asprintf(&tokens[position], "%s", token); // deep copy
		++position;
		token = strtok(NULL, C_SHELL_TOK_DELIM);
	} while (token != NULL);
	tokens[position] = NULL; // NULL-terminate the list of strings

	cmd->nargs = position;
	cmd->argv = tokens;

	return cmd;
}

struct job *parse_job(char *job_str)
{
	struct job *j = malloc(sizeof(*j));
	j->first_command = NULL;
	j->pgid = 0;
	asprintf(&j->user_command, "%s", job_str); // deep copy
	j->notified = 0;
	j->next = NULL;

	char *rem; // strtok_r needs this for re-entry
	char *piped = strtok_r(job_str, PIPE_DELIM, &rem);
	struct command *cmd = parse_command(piped, strlen(piped));
	j->first_command = cmd;

	while ((piped = strtok_r(NULL, PIPE_DELIM, &rem)) != NULL)
	{
		cmd->next = parse_command(piped, strlen(piped));
		cmd->stdout = cmd->next->stdin;
		cmd = cmd->next;
	}

	return j;
}

struct job *parse_line_to_jobs(char *line)
{
	char *temp = strdup(line);
	if (temp == NULL)
	{
		fprintf(stderr, "Failed to allocate memory for line parsing.\n");
		abort();
	}
	char *rem; // strtok_r needs this for re-entry

	char *job_str = strtok_r(temp, JOB_DELIM, &rem);
	if (job_str == NULL)
	{
		free(temp);
		return NULL;
	}
	struct job *first_job = parse_job(job_str);
	struct job *j = first_job;

	while ((job_str = strtok_r(NULL, JOB_DELIM, &rem)) != NULL)
	{
		j->next = parse_job(job_str);
		j = j->next;
	}

	free(temp);
	return first_job;
}
