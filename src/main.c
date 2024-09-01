/**
 * @file main.c
 * @brief The entrypoint to the program.
 * @author Swastik Pal
 * @date 2022-11-03
 *
 * @copyright Copyright (c) 2022
 */

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "executor.h"
#include "internal/history.h"
#include "job_store.h"
#include "main.h"
#include "parse.h"
#include "shelltypes.h"
#include "terminal.h"

/**
 * @brief Execution starts here
 */
int main()
{
	cls;
	// load config files, if any.
	load_settings();
	read_history_from_file();
	// loop
	init_shell();
	loop();
	// cleanup
	return 0;
}

int load_settings()
{
	// not yet implemented
	return 0;
}

int loop()
{
	while (true)
	{
		print_prompt();

		size_t line_size = LINE_BUFFER_SIZE;
		char *line = calloc(line_size, sizeof(*line));
		read_line(line, line_size);
		// printf("You entered: %s\n", line); // debuggging only

		if (line[0] == '\0')
			continue; // ignore empty lines

		add_to_history(line);

		struct job *j = parse_line_to_jobs(line);
		struct job *first_job = j;
		while (j)
		{
			// printf("Job: %s\n", j->user_command); // debugging only
			// struct command *c = j->first_command;
			// while (c)
			// {
			// 	printf("Command: %s\n", c->argv[0]); // debugging only
			// 	c = c->next;
			// }
			launch_job(j);
			j = j->next;
		}
		add_jobs_to_store(first_job);
		// list_jobs(); // debugging only

		free(line);
	}
}

int print_prompt()
{
	char hostname[MAXHOSTNAMELEN];
	gethostname(hostname, MAXHOSTNAMELEN);
	char *current_dir = getcwd(NULL, 0);
	return printf("%s@%s:%s> ", getenv("USER"), hostname, current_dir);
}
