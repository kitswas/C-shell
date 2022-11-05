/**
 * @file echo.c
 * @author Swastik Pal
 * @brief
 * @version 0.1
 * @date 2022-11-05
 */

#include <stdio.h>

int echo(int nargs, char **args)
{
	for (int i = 1; i < nargs; i++)
	{
		printf("%s ", args[i]);
	}
	printf("\n");
	return 0;
}
