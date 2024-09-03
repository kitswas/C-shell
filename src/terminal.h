#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdbool.h>
#include <stddef.h>
#include <termios.h>

// macros here

/** Clears the screen. Requires terminal support for ANSI escape sequences. */
#define cls printf("\033[2J\033[1;1H");
#define MAX_TAB_COMPLETION_ENTRIES 5

// global variables here

// structs here

// functions here

/**
 * @brief Reads a line from stdin.
 *
 * @param buffer The buffer to write into.
 * @param buffer_size The size of the provided buffer.
 * @return The number of characters read.
 */
size_t read_line(char *buffer, size_t buffer_size);

/**
 * @brief Sets a terminal flag.
 *
 * @param term_des The terminal descriptor.
 * @param flag The flag to modify.
 * @param set Whether to set or unset the flag.
 * @return 0 on success.
 */
int set_term_flag(int term_des, tcflag_t flag, bool set);

#endif
