#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
	//读设备或网络文件时才可能阻塞
	char buf[10];
	int ret = read(STDIN_FILENO, buf, 10);
	if (ret < 0)
	{
		perror("read STDIN_FILENO");
		exit(1);
	}
	write(STDOUT_FILENO, buf, 10);

	return 0;
}
