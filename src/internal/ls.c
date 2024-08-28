#include <dirent.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <pwd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "../internal_commands.h"

extern int opterr, optind, optopt;
extern char *optarg;

char *get_details(const char *const path)
{
	struct stat file_stat;
	if (stat(path, &file_stat) == -1)
	{
		fprintf(stderr, "[ERROR] %d %s\n", errno, strerror(errno));
		return NULL;
	}

	char file_mode[11];
	if (file_mode == NULL)
	{
		fprintf(stderr, "[ERROR] %d %s\n", errno, strerror(errno));
		return NULL;
	}

	// file type
	switch (file_stat.st_mode & S_IFMT)
	{
	case S_IFBLK:
		file_mode[0] = 'b';
		break;
	case S_IFCHR:
		file_mode[0] = 'c';
		break;
	case S_IFDIR:
		file_mode[0] = 'd';
		break;
	case S_IFIFO:
		file_mode[0] = 'p';
		break;
	case S_IFLNK:
		file_mode[0] = 'l';
		break;
	case S_IFREG:
		file_mode[0] = '-';
		break;
	case S_IFSOCK:
		file_mode[0] = 's';
		break;
	default:
		file_mode[0] = '?';
		break;
	}

	// user permissions
	file_mode[1] = (file_stat.st_mode & S_IRUSR) ? 'r' : '-';
	file_mode[2] = (file_stat.st_mode & S_IWUSR) ? 'w' : '-';
	file_mode[3] = (file_stat.st_mode & S_IXUSR) ? 'x' : '-';

	// group permissions
	file_mode[4] = (file_stat.st_mode & S_IRGRP) ? 'r' : '-';
	file_mode[5] = (file_stat.st_mode & S_IWGRP) ? 'w' : '-';
	file_mode[6] = (file_stat.st_mode & S_IXGRP) ? 'x' : '-';

	// other permissions
	file_mode[7] = (file_stat.st_mode & S_IROTH) ? 'r' : '-';
	file_mode[8] = (file_stat.st_mode & S_IWOTH) ? 'w' : '-';
	file_mode[9] = (file_stat.st_mode & S_IXOTH) ? 'x' : '-';

	file_mode[10] = '\0';

	char *details;

	char *owner_name = getpwuid(file_stat.st_uid)->pw_name;
	if (owner_name == NULL)
	{
		fprintf(stderr, "[ERROR] %d %s\n", errno, strerror(errno));
		return NULL;
	}

	char *group_name = getpwuid(file_stat.st_gid)->pw_name;
	if (group_name == NULL)
	{
		fprintf(stderr, "[ERROR] %d %s\n", errno, strerror(errno));
		return NULL;
	}

	char time_format[] = "%b %d %H:%M";
	size_t formatted_time_size = 12; // Found by counting the characters in the output of the ls binary
	char *mtime_str = calloc(formatted_time_size + 1, sizeof(*mtime_str));
	strftime(mtime_str, formatted_time_size + 1, time_format, localtime(&file_stat.st_mtime));
	// printf("%s\n", mtime_str); // debugging only

	int n = asprintf(&details,
					 "%s %ld %s %s %ld\t%s",
					 file_mode,
					 file_stat.st_nlink,
					 owner_name,
					 group_name,
					 file_stat.st_size,
					 mtime_str);

	if (n < 0)
	{
		fprintf(stderr, "[ERROR] %d %s\n", errno, strerror(errno));
		return NULL;
	}

	return details;
}

int print_dir_entries(const char *const path, bool show_hidden, bool show_details)
{
	DIR *dir;
	struct dirent *entry;

	if ((dir = opendir(path)) == NULL)
	{
		return -1;
	}

	while ((entry = readdir(dir)) != NULL)
	{
		if (show_hidden || entry->d_name[0] != '.')
		{
			if (show_details)
			{
				char *full_path;
				if (asprintf(&full_path, "%s/%s", path, entry->d_name) < 0)
				{
					fprintf(stderr, "[ERROR] %d %s\n", errno, strerror(errno));
					return -1;
				}
				char *details = get_details(full_path);
				printf("%s %s\n", details, entry->d_name);
				free(details);
			}
			else
			{
				printf("%s\t", entry->d_name);
			}
		}
	}
	printf("\n");

	closedir(dir);
	return 0;
}

int ls(int nargs, char **args)
{
	opterr = 0;
	optind = 0;
	int c = -1;

	const char *const default_path = ".";
	option show_hidden = false;
	option show_details = false;

	// deal with the options
	while ((c = getopt(nargs, args, "al")) != -1)
		switch (c)
		{
		case 'a':
			show_hidden = true;
			break;
		case 'l':
			show_details = true;
			break;
		case '?':
			if (isprint(optopt))
				fprintf(stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
			return 1;
		default:
			fprintf(stderr, "Aborting in ls...");
			abort();
		}

	errno = 0;
	if (optind < nargs)
	{
		for (size_t i = optind; i < nargs; ++i)
		{
			print_dir_entries(args[optind], show_hidden, show_details);
		}
	}
	else
	{
		print_dir_entries(default_path, show_hidden, show_details);
	}
	switch (errno)
	{
	case 0:
		// do nothing
		break;
	default:
		fprintf(stderr, "[ERROR] %d %s\n", errno, strerror(errno));
		break;
	}

	return 0;
}
