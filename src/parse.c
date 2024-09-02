#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "parse.h"
#include "shelltypes.h"

bool is_blank(const char *str)
{
	if (str == NULL)
	{
		return true;
	}
	while (*str != '\0')
	{
		if (!isspace(*str))
		{
			return false;
		}
		++str;
	}
	return true;
}

struct command *parse_command(char *command, size_t command_len)
{
	struct command *cmd = malloc(sizeof(*cmd));
	cmd->next = NULL;
	cmd->nargs = 0;
	cmd->argv = NULL;
	cmd->pid = -1; // not yet executed
	cmd->fd_in = STDIN_FILENO;
	cmd->fd_out = STDOUT_FILENO;
	cmd->fd_err = STDERR_FILENO;

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
	j->pgid = -1;
	asprintf(&j->user_command, "%s", job_str); // deep copy
	j->next = NULL;
	j->background = false;

	// Look for & at the end of the line to determine if the job is a background job
	char *ampersand = strrchr(job_str, '&');
	if (ampersand != NULL)
	{
		// Check if the string is blank after the & character
		j->background = is_blank(ampersand + 1);
		if (j->background)
		{
			*ampersand = ' '; // remove the & character
							  //! Using '\0' instead of ' ' will cause a segfault with strlen.
							  // TODO Investigate why.
		}
	}

	char *rem; // strtok_r needs this for re-entry
	char *piped = strtok_r(job_str, PIPE_DELIM, &rem);
	struct command *cmd = parse_command(piped, strlen(piped));
	j->first_command = cmd;

	while ((piped = strtok_r(NULL, PIPE_DELIM, &rem)) != NULL)
	{
		cmd->next = parse_command(piped, strlen(piped));
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
