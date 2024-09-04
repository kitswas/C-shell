#ifndef HISTORY_H
#define HISTORY_H

// macros here

/** The default number of items to print when the user does not specify a number. */
#define DEFAULT_HIST_ITEMS 10
#define MAX_HISTORY_ITEMS 20

// global variables here

// structs here
struct history_list
{
	char *hist[MAX_HISTORY_ITEMS];
	int count;
};

// functions here

/**
 * @brief Adds a line to the history.
 *
 * @param line The command to add.
 */
void add_to_history(char *line);

/**
 * @brief Clears the history.
 */
void clear_history();

/**
 * @brief Gets the history list.
 *
 * @return Pointer to the history list.
 */
const struct history_list *get_history();

/**
 * @brief Prints the last `n` commands in the history.
 * @param n The number of commands to print.
 */
void print_history(int n);

int read_history_from_file();

int write_history_to_file();

#endif
