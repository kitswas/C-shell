#include <stdio.h>
#include <stdlib.h>
#include "history.h"

void quit_shell_with_status(int status)
{
	write_history_to_file();
	printf("\033[0m"); // reset all terminal attributes
	exit(status);
}

void quit_shell()
{
	quit_shell_with_status(EXIT_SUCCESS);
}
