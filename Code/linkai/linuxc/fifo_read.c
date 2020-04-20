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
		printf("Usage: ./fifo_read fifoname\n");
		return -1;
	}

	if ((fd = open(argv[1], O_RDONLY)) == -1)
	{
		perror("open error");
		exit(1);
	}

	while (1)
	{
		int i = 1, len;
		len = read(fd, buf, sizeof(buf));
		write(STDOUT_FILENO, buf, len);
		sleep(1);
	}
	close(fd);

	return 0;
}
