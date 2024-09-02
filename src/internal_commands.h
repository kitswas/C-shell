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
 * @brief Print the history of commands.
 *
 * @details
 * If no option is specified, prints the last 5 commands.
 *
 * Options:
 * - `-c` - clear the history
 * - `-r` - read the history file and write to the history list
 * - `-w` - write the history list to the history file
 */
int history(int nargs, char **args);

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
 * Sample output:
 *
 * ```
 * pid -- 231
 * Process Status -- {R/S/S+/Z}
 * memory -- 67854 {Virtual Memory}
 * Executable Path -- ./cshell
 * ```
 *
 * Process status codes:
 * 1. R/R+: Running
 * 2. S/S+: Sleeping in an interruptible wait
 * 3. Z: Zombie
 * 4. T: Stopped (on a signal)
 *
 * Note: “+” must be added to the status code if the process is in the foreground.
 *
 * @param pid pid of the process, -1 for the parent process, 0 for the current process
 * @return int 0 on success, -1 on failure
 */
int pinfo(pid_t pid);

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
