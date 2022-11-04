/**
 * @file main.c
 * @brief Just trying out this setup
 * @author Swastik Pal
 * @version 0.1
 * @date 2022-11-03
 *
 * @copyright Copyright (c) 2022
 *
 * Comments _must_ be written in [**Markdown**](https://www.markdownguide.org/cheat-sheet/).
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "main.h"

/**
 * @brief Execution starts here
 */
int main()
{
	cls;
	// load config files, if any.
	load_settings();
	// loop
	loop();
	// cleanup
	return 0;
}

int load_settings()
{
}

int loop()
{
	while (true)
	{
		print_prompt();
		size_t line_size = LINE_BUFFER_SIZE;
		char *line = malloc(line_size * sizeof(*line));
		read_line(line, line_size);
		printf("You entered: %s\n", line); // debuggging only

		free(line);
	}
}

int print_prompt()
{
	char hostname[MAXHOSTNAMELEN];
	gethostname (hostname, MAXHOSTNAMELEN);
	printf("%s@%s$ ", getenv("USER"), hostname);
}

int read_line(char *buffer, int buffer_size)
{
	int ch = 0;
	int i = 0;
	do
	{
		ch = getchar();
		buffer[i] = ch;
		++i;
	} while (ch != '\n' && ch != EOF && i < buffer_size);
	buffer[i - 1] = '\0'; // null terminate the string
	while (ch != '\n' && ch != EOF) // discard the rest of the input line
		ch = getchar();
	return i;
}
