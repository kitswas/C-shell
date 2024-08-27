/**
 * @file main.h
 * @brief This header file contains almost everything needed for this project
 * @author Swastik Pal
 * @version 0.1
 * @date 2022-11-03
 */

#ifndef PARSE_H
#define PARSE_H

#include <stddef.h>

// macros here

#define C_SHELL_TOK_DELIM " \t\r\n"

// global variables here

// structs here

// functions here

/**
 * @brief Tokenizes the command.
 *
 * @param line A string.
 * @param line_size Length of the string.
 * @return char** An array of tokens.
 */
char **parse_command(char *command, size_t command_size, int *nargs);

#endif
