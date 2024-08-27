/**
 * @file internal_commands.h
 * @brief This header file contains declarations for the internal commands
 */

#ifndef INTERNAL_COMMANDS_H
#define INTERNAL_COMMANDS_H

#include <stdbool.h>

// macros here

// global variables here
typedef bool option;

// structs here

// functions here

/**
 * @brief Change the shell working directory.
 *
 * @details
 * If no path is provided,
 * changes the current directory to `/`.
 */
int cd(int nargs, char **args);

/**
 * @brief Write arguments to the standard output.
 *
 * @details
 * Display the ARGs, separated by a single space character
 * and followed by a newline, on the standard output.
 *
 * Options:
 * - `-c` - separate arguments with commas
 * - `-n` - no newline at the end
 * - `-l` - format output as a list
 */
int echo(int nargs, char **args);

/**
 * @brief List directory contents.
 *
 * @details
 * List information about the FILEs (the current directory by default).
 *
 * Options:
 * - `-a` - do not ignore entries starting with `.`
 * - `-l` - use a long listing format
 */
int ls(int nargs, char **args);

/**
 * @brief Print the full filename of the current working directory.
 *
 * @details
 * If no option is specified, -P is assumed. Option `-L` overrides
 * `-P` when both are used together.
 *
 * Options:
 * - `-L` - use PWD from environment
 * - `-P` - resolves symlinks to get the current working directory.
 */
int pwd(int nargs, char **args);

#endif
