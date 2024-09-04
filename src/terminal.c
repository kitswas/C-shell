#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "internal_commands.h"
#include "internal/history.h"
#include "terminal.h"

int starts_with(const char *restrict string, const char *restrict prefix)
{
	while (*prefix)
	{
		if (*prefix++ != *string++)
			return 0;
	}
	return 1;
}

static struct filter_context
{
	const char *buffer;
} *filter_ctx;

// Check if the entry name starts with the buffer
static int filter(const struct dirent *entry, const char *buffer)
{
	return starts_with(entry->d_name, buffer);
}

static int filter_wrapper(const struct dirent *entry)
{
	return filter(entry, filter_ctx->buffer);
}

/**
 * @brief Prints a 'backspace' n times.
 */
void backspace(size_t n)
{
	for (size_t i = 0; i < n; ++i)
	{
		// \b is the backspace character
		// it moves the cursor one character to the left
		// but does not delete the character
		// so we need to print a space to overwrite the character
		// and then move the cursor back again

		putchar('\b');
		putchar(' ');
		putchar('\b');
	}
}

size_t read_line(char *buffer, size_t buffer_size)
{
	set_term_flag(STDIN_FILENO, ICANON | ECHO, false);

	// for history navigation
	const struct history_list *h_list = get_history();
	int pos = h_list->count;
	size_t len = 0;

	int ch = 0;
	size_t i = 0;
	do
	{
		ch = getchar();
		switch (ch)
		{
		case 27: // escape sequence
			ch = getchar();
			if (ch == '[')
			{
				ch = getchar();
				switch (ch)
				{
				case 'A': // up arrow
					if (h_list->count == 0)
						break;
					if (pos - 1 >= 0)
						--pos;
					backspace(i);
					i = 0;
					len = strlen(h_list->hist[pos]);
					strcpy(buffer, h_list->hist[pos]);
					printf("%s", buffer);
					i = len;
					break;
				case 'B': // down arrow
					if (pos + 1 < h_list->count)
						++pos;
					else if (pos + 1 == h_list->count) // if we are at the end of the history list
					{
						backspace(i);
						i = 0;
						buffer[0] = '\0';
						break;
					}
					if (pos == h_list->count)
						break;
					backspace(i);
					i = 0;
					len = strlen(h_list->hist[pos]);
					strcpy(buffer, h_list->hist[pos]);
					printf("%s", buffer);
					i = len;
					break;
				case 'C': // right arrow
					// do nothing
					break;
				case 'D': // left arrow
					// do nothing
					break;
				}
			}
			// Consume the rest of the escape sequence
			while (ch < 0x40 || ch > 0x7E) // Look for the 'final byte'
										   // https://en.wikipedia.org/wiki/ANSI_escape_code#CSI
				ch = getchar();
			break;
		case 8:
		case 127: // backspace
			if (i > 0)
			{
				buffer[i - 1] = '\0';
				--i;
				backspace(1);
			}
			continue;
		case 3: // ctrl-c
			buffer[0] = '\0';
			return 0;
		case 4: // ctrl-d
			quit_shell();
			break;
		case '\t': // tab completion
			;
			int entries = 0;
			struct dirent **namelist;
			buffer[i + 1] = '\0'; // null terminate the input string for filtering
			char *last_space = strrchr(buffer, ' ');
			filter_ctx = &(struct filter_context){.buffer = last_space ? last_space + 1 : buffer};
			// See https://www.gnu.org/software/libc/manual/pdf/libc.pdf#Simple%20Directory%20Lister%20Mark%20II
			entries = scandir(".", &namelist, filter_wrapper, alphasort);
			if (entries < 0)
			{
				perror("[ERROR] scandir");
				break;
			}
			else if (entries == 1)
			{
				// Append the completion to the buffer if it fits
				size_t len = strlen(namelist[0]->d_name);
				size_t chars_to_delete = last_space ? (size_t)(buffer + i - last_space - 1) : i;
				// printf("%ld\n", chars_to_delete); // debugging only
				if (i + len - chars_to_delete < buffer_size - 1)
				{
					strcpy(buffer + i - chars_to_delete, namelist[0]->d_name);
					i += len - chars_to_delete;
					// Echo the completion
					printf("%s", namelist[0]->d_name + chars_to_delete);
				}
				free(namelist[0]);
			}
			else if (entries > 1 && entries <= MAX_TAB_COMPLETION_ENTRIES)
			{
				printf("\n");
				for (int j = 0; j < entries; ++j)
				{
					printf("\t%s", namelist[j]->d_name);
					free(namelist[j]);
				}
				free(namelist);
				printf("\n");

				// Cancel the input line
				buffer[0] = '\0';
				return 0;
			}

			break;
		default:
			buffer[i] = (char)ch;
			++i;
			putchar(ch);
		}
	} while (ch != '\n' && ch != EOF && i < buffer_size - 1);
	buffer[i - 1] = '\0';			// null terminate the string
	while (ch != '\n' && ch != EOF) // discard the rest of the input line
		ch = getchar();

	set_term_flag(STDIN_FILENO, ICANON | ECHO, true);

	return i;
}

int set_term_flag(int term_des, tcflag_t flag, bool set)
{
	struct termios settings;
	int result;
	result = tcgetattr(term_des, &settings);
	if (result != 0)
	{
		perror("[ERROR] tcgetattr");
		return result;
	}
	settings.c_lflag &= ~flag;
	if (set)
		settings.c_lflag |= flag;
	result = tcsetattr(term_des, TCSANOW, &settings); // TCSANOW tells tcsetattr to change attributes immediately.
	if (result != 0)
	{
		perror("[ERROR] tcsetattr");
		return result;
	}
	return 0;
}
