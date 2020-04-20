#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
	char *p = NULL;
	int fd, len;
	if ((fd = open("test_map", O_RDWR|O_CREAT|O_TRUNC, 0644)) == -1)
	{
		perror("open error");
		exit(1);
	}
	ftruncate(fd, 20);
/*	lseek(fd, 19, SEEK_SET);
	write(fd, "", 1);
*/
	len = lseek(fd, 0, SEEK_END);	//获取文件长度
	if ((p = (char*)mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
	{
		perror("mmap error");
		exit(1);
	}
	//使用p对文件进行操作
	strcpy(p, "Hello mmap\n");	//相当于写操作

	int ret;
	if ((ret = munmap(p, len)) == -1)
	{
		perror("munmap error");
		exit(1);
	}

	return 0;
}
