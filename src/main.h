#ifndef MAIN_H
#define MAIN_H

// macros here

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
 * @brief I keep looping, looping till the user exits.
 */
int loop();

/**
 * @brief Prints `username@machinename:path>`.
 * @return The number of characters printed.
 */
int print_prompt();

#endif
