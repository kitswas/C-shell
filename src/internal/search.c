#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "../internal_commands.h"

bool find_key_recursively(char *path, char *key)
{
	errno = 0;
	bool found = false;

	DIR *dir;
	struct dirent *entry;
	dir = opendir(path);
	if (dir != NULL)
	{
		while ((entry = readdir(dir)))
		{
			if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
				continue;
			// printf("%s\n", entry->d_name); // debugging only
			if (!strcmp(entry->d_name, key))
			{
				found = true;
				break;
			}
			if (DT_DIR == entry->d_type)
			{
				char *subdir_path = NULL;
				asprintf(&subdir_path, "%s/%s", path, entry->d_name);
				// printf("%s\n", subdir_path); // debugging only
				found = find_key_recursively(subdir_path, key);
				free(subdir_path);
				if (found)
					break;
			}
		}
		closedir(dir);
	}
	else

		switch (errno)
		{
		case 0:
			// do nothing
			break;
		default:
			fprintf(stderr, "[ERROR] %d %s\n", errno, strerror(errno));
			break;
		}

	return found;
}

int search(int nargs, char **args)
{
	if (nargs != 2)
	{
		fprintf(stderr, "[ERROR] Search: Too %s arguments.\n", nargs > 2 ? "many" : "few");
		return -1;
	}
	char *key = args[1];

	bool found = find_key_recursively(".", key);
	printf("%s\n", found ? "True" : "False");

	return 0;
}
