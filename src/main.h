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
 * @brief Prints username\@machinename.
 */
int print_prompt();

/**
 * @brief Reads a line from stdin.
 *
 * @return The number of characters read.
 */
int read_line(char *buffer, int buffer_size);

#endif
