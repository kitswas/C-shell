/**
 * @file main.c
 * @brief This header file contains almost everything needed for this project
 * @author Swastik Pal
 * @version 0.1
 * @date 2022-11-03
 */

#ifndef MAIN_H
#define MAIN_H

// macros here
#define cls printf("\033[2J\033[1;1H"); // Clears the screen on both Windows 10 and Linux. Doesn't work on old Windows OSes
#define LINE_BUFFER_SIZE 1024
#define MAXHOSTNAMELEN 32
#define C_SHELL_TOK_DELIM " \t\r\n"

// global variables here

// structs here

// functions here

/**
 * @brief Will load configuration file(s).
 *
 * Not yet implemented.
 */
int load_settings();

/**
 * @brief I keep looping, looping till it the user exits
 *
 * Implementation incomplete.
 */
int loop();

/**
 * @brief Tokenizes the line.
 *
 * @param line A string.
 * @param line_size Length of the string.
 * @return char** An array of tokens.
 */
char **parse_line(char *line, size_t line_size, int *nargs);

/**
 * @brief Prints username\@machinename.
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
