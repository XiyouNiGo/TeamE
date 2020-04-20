#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

int var = 100;
int main()
{	
	int *p;
	pid_t pid;
	int fd;

	if ((fd = open("temp", O_CREAT|O_TRUNC|O_RDWR)) == -1)
	{
		perror("open error");
		exit(1);
	}
	ftruncate(fd, 4);

	if ((p = (int*)mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
	{
		perror("mmap error");
		exit(1);
	}
	close(fd);	//创建完毕即可关闭描述符

	pid = fork();

	if (pid == 0)
	{
		*p = 2000;
		var = 1000;
		printf("child: *p = %d, var = %d\n", *p, var);
	}
	else if (pid > 0)
	{
		sleep(1);
		printf("parent: *p = %d, var = %d\n", *p, var);
		wait(NULL);
		int ret;
		if ((ret = munmap(p, 4)) == -1)
		{
			perror("munmap error");
			exit(1);
		}
	}

	return 0;
}
