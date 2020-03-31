#include <stdio.h>
#include <stdlib.h>
#include "debug.h"

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		printf("Usage: ./my_mv <old file> <new file>\n");
		exit(0);
	}
	if (rename(argv[1], argv[2]) == -1)
	{
		my_err("rename", __LINE__);
	}

	return 0;
}
