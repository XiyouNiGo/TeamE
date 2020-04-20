//用于测试管道文件
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fd;
	char buf[4096];

	if (argc != 2)
	{
		printf("Usage: ./file_write fifoname\n");
		return -1;
	}

	if ((fd = open(argv[1], O_WRONLY)) == -1)
	{
		perror("open error");
		exit(1);
	}

	while (1)
	{
		static int i = 1;
		sprintf(buf, "Hello fifo %d\n", i++);
		write(fd, buf, strlen(buf));
		sleep(1);
	}
	close(fd);

	return 0;
}
