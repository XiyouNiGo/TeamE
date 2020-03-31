#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include "debug.h"

int main(int argc, char **argv)
{
	char *path;
	int fd;
	int mode;
	char *end;
	if (argc < 3)
	{
		puts("chmod: 缺少操作数");
		exit(0);
	}
	path = argv[2];
	fd = open(path, O_RDONLY);
	mode = (int)strtol(argv[1], &end, 8);
	if (mode > 777 || mode < 0)
	{
		puts("Invalid mode number!");
		exit(0);
	}
//	if (chmod(path, mode) == -1)
//	{
//		my_err("chmod error", __LINE__);
//	}
	if (fchmod(fd, mode) == -1)
	{
		my_err("fchmod error", __LINE__);
	}
	return 0;
}

