#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int my_readdir(const char *path);

int main(int argc, char *argv[])
{
	if (argc < 2)	
	{
		puts("Usage: ./show_files <filename>");
	}
	for (int i = 1; i < argc; i++)
	{
		printf("Directory %s content:\n\n", argv[i]);
		if (my_readdir(argv[i]) == -1)
		{
			printf("Invalid reading in %s\n", argv[i]);
		}
		putchar('\n');
	}

	return 0;
}

int my_readdir(const char *path)
{
	DIR *dir;
	int i = 0;
	struct dirent *ptr;
	if ((dir = opendir(path)) == NULL)
	{
		perror("opendir");
		return -1;
	}
	while ((ptr = readdir(dir)) != NULL)
	{
		printf("file%d: %s\n", ++i, ptr->d_name);
	}
	closedir(dir);

	return 0;
}

