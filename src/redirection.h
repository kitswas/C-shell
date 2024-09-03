#ifndef REDIRECTION_H
#define REDIRECTION_H

#include "shelltypes.h"

// macros here

// global variables here

// structs here

// functions here

/**
 * @brief Handles redirections for a command.
 *
 * Parses the command's arguments and sets the file descriptors for input, output, and error.
 * If a file is not found, the function will print an error message and return -1.
 *
 * Command arguments are modified in place to remove the redirection symbols and the file names.
 * The file descriptors must be standard input/output/error.
 * Otherwise, the function will print an error message and return -1.
 *
 * @param cmd
 * @return 0 on success, -1 on error
 */
int handle_redirection(struct command *cmd);

#endif
