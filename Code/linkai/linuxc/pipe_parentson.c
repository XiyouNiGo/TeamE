#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

//利用父子进程实现ls | wc -l命令

int main()
{
	int fd[2];
	int ret;
	pid_t pid;

	if ((ret = pipe(fd)) == -1)
	{
		perror("pipe error");
		exit(1);
	}

	if ((pid = fork()) == -1)
	{
		perror("fork error");
		exit(1);
	}
	
	if (pid > 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		execlp("ls", "ls", NULL);
		perror("execlp error");
		exit(1);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		execlp("wc", "wc", "-l", NULL);
		perror("execlp error");
		exit(1);
	}

	return 0;
}

