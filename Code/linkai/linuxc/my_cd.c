#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "debug.h"

int main(int argc, char *argv[])
{
	char *path;
	if (argc < 2)
	{
		puts("Usage: ./my_cd <target path>");
		exit(0);
	}
	if (chdir(argv[1]) < 0)
	{
		my_err("chdir", __LINE__);
	}
	if ((path = getcwd(NULL, 0)) == NULL)
	{
		my_err("getcwd", __LINE__);
	}
	printf("%s\n", path);
	free(path);
	path = NULL;

	return 0;
}
