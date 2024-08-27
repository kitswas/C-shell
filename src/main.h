#ifndef MAIN_H
#define MAIN_H

// macros here

#define cls printf("\033[2J\033[1;1H"); // Clears the screen. Requires support for ANSI escape sequences.
#define LINE_BUFFER_SIZE 1024
#define MAXHOSTNAMELEN 32

// global variables here

// structs here

// functions here

/**
 * @brief Executes the command.
 *
 * @param nargs Number of arguments to the command.
 * @param command The command to execute.
 * @param args The list of arguments to the command.
 * @return int
 *
 * @details
 * Available commands:
 * - cls - clears the screen
 * - exit - terminates the terminal
 */
int execute(int nargs, char *command, char **args);

/**
 * @brief Will load configuration file(s).
 *
 * Not yet implemented.
 */
int load_settings();

/**
 * @brief I keep looping, looping till the user exits.
 */
int loop();

/**
 * @brief Prints `username\@machinename\:path>`.
 */
int print_prompt();

/**
 * @brief Reads a line from stdin.
 *
 * @param buffer The buffer to write into.
 * @param buffer_size The size of the provided buffer.
 * @return int The number of characters read.
 */
int read_line(char *buffer, int buffer_size);

#endif
