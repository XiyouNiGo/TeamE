#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "debug.h"

int main()
{
	int fd;
	char buf[32];
	if ((fd = open("temp", O_RDWR|O_CREAT|O_TRUNC, S_IRWXU) < 0))
	{
		my_err("open", __LINE__);
	}
	if (unlink("temp") < 0)
	{
		my_err("unlink", __LINE__);
	}
	puts("file unlinked");

	return 0;
}
