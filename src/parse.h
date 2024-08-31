#ifndef PARSE_H
#define PARSE_H

#include <stddef.h>

// macros here

#define C_SHELL_TOK_DELIM " \t\r\n"
#define JOB_DELIM ";"
#define PIPE_DELIM "|"

// global variables here

// structs here

// functions here

/**
 * @brief Parse a command string into a command struct
 *
 * @param command The command string
 * @param command_len The length of the command string
 * @return The parsed command struct
 */
struct command *parse_command(char *command, size_t command_len);

/**
 * @brief Parse a command string containing pipes into a job struct.
 * The string should not have any semicolons.
 *
 * @param job_str The command string
 * @return The parsed job struct
 */
struct job *parse_job(char *job_str);

/**
 * @brief Parse a line into a list of jobs.
 *
 * @param line The line to parse
 * @return A pointer to the first job in a linked list
 */
struct job *parse_line_to_jobs(char *line);

#endif
