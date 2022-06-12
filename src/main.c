#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "finder.h"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("%s %s %s\n", "Usage:", argv[0], "[start folder name]");
		exit (EXIT_SUCCESS);
	}

	if (chdir(argv[1]))
	{
		print_error();
		return 1;
	}

	if (list_files("."))
		exit(EXIT_FAILURE);
	exit (EXIT_SUCCESS);
}

char *concat_path(const char *path_preffix, const char *path_postfix)
{
	char *concated_path = malloc(sizeof(char)
								 * ((strlen(path_preffix)) + 1 +
									strlen(path_postfix) + 1));
	if (!concated_path)
	{
		fprintf(stderr, "Error:\nAllocation problem occurred.\n");
		return NULL;
	}
	strcpy(concated_path, path_preffix);
	strcat(concated_path, SYSTEM_SEPARATOR);
	strcat(concated_path, path_postfix);
	return concated_path;
}

int list_files(const char *dir_path)
{
	DIR		*dir;
	struct	dirent *entity;
	char	*concated_path;
	int		status;

	if ((dir = opendir(dir_path)) == NULL)
	{
		if (errno == EACCES) /* continue execution even if dir permissions denied, stderr message shown anyway */
		{
			fprintf(stderr, "%s: %s: %s\n",
					"finder", dir_path, "permissions denied");
			return 0;
		}
		print_error();
		return 1;
	}
	entity = readdir(dir);
	while (entity != NULL)
	{
		concated_path = concat_path(dir_path, entity->d_name);
		if (concated_path == NULL)
		{
			closedir(dir);
			return 1;
		}
		if (strcmp(entity->d_name, "..") != 0 &&
			strcmp(entity->d_name, ".") != 0)
			printf("%s\n", concated_path);
		if (entity->d_type == DT_DIR && strcmp(entity->d_name, "..") != 0 &&
			strcmp(entity->d_name, ".") != 0)
		{
			status = list_files(concated_path);
			if (status)
			{
				free(concated_path);
				closedir(dir);
				return 1;
			}
		}
		free(concated_path);
		entity = readdir(dir);
	}
	closedir(dir);
	return 0;
}

void print_error()
{
	if (errno == ENOMEM)
		fprintf(stderr, "%s\n", "Error:\nOut of memory.");
	else if (errno == ENOENT)
		fprintf(stderr, "%s\n", "Error:\nDirectory does not exist, or an empty string.");
	else if (errno == ENOTDIR)
		fprintf(stderr, "%s\n", "Error:\nis not a directory.");
	else if (errno == EBADF)
		fprintf(stderr, "%s\n", "Error:\nfd is not a valid file descriptor opened for reading.");
	else if (errno == EMFILE)
		fprintf(stderr, "%s\n", "Error:\nThe per-process limit on the number of open file descriptors has\n"
								"been reached.");
	else if (errno == EFAULT)
		fprintf(stderr, "%s\n", "Error:\npath points outside your accessible address space");
	else
		fprintf(stderr, "%s\n", "Error:\n");
}
