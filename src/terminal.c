#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "terminal.h"

size_t read_line(char *buffer, size_t buffer_size)
{
	set_term_flag(STDIN_FILENO, ICANON | ECHO, false);

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
					// implement history
					break;
				case 'B': // down arrow
					// do nothing
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
				putchar('\b');
				// \b is the backspace character
				// it moves the cursor one character to the left
				// but does not delete the character
				// so we need to print a space to overwrite the character
				// and then move the cursor back again
				putchar(' ');
				putchar('\b');
			}
			continue;
		case 3: // ctrl-c
			buffer[0] = '\0';
			return 0;
		case 4: // ctrl-d
			if (i == 0)
			{
				buffer[0] = '\0';
				return 0;
			}
			break;
		default:
			buffer[i] = (char)ch;
			++i;
			putchar(ch);
		}
	} while (ch != '\n' && ch != EOF && i < buffer_size);
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
