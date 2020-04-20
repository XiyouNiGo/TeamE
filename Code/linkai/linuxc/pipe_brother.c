#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

//利用兄弟进程实现ls | wc -l命令

int main()
{
	int fd[2];
	int ret;
	int i;
	pid_t pid;

	if ((ret = pipe(fd)) == -1)
	{
		perror("pipe error");
		exit(1);
	}

	for (i = 0; i < 2; i++)
	{
		if ((pid = fork())== -1)
		{
			perror("fork error");
			exit(1);
		}
		if (pid == 0)
		{
			break;
		}
	}

	if (i == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		execlp("ls", "ls", NULL);
		perror("execlp error");
		exit(1);
	}
	else if (i == 1)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		execlp("wc", "wc", "-l", NULL);
		perror("execlp error");
		exit(1);
	}
	else
	{
		close(fd[0]);	//一个写端多个读端不行，多个读端一个写端可以
		close(fd[1]);
		for (int i = 0; i < 2; i++)
		{
			wait(NULL);
		}
	}

	return 0;
}

