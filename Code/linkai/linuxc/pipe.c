#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

int main()
{
	int ret;
	int fd[2];
	pid_t pid;

	const char *str = (const char*)"Hello pipe";
	char buf[1024];

	if ((ret = pipe(fd)) == -1)
	{
		perror("pipe error");
		exit(1);
	}

	pid = fork();
	if (pid > 0)
	{
		close(fd[0]);	//关闭读端
		write(fd[1], str, strlen(str));
		sleep(1);	//防止父进程先结束，使输出到下一行
		close(fd[1]);
	}
	else if (pid == 0)
	{
		close(fd[1]);	//关闭写端
		ret = read(fd[0], buf, sizeof(buf));
		write(STDOUT_FILENO, buf, ret);
	}

	return 0;
}
