#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
	pid_t pid = fork();
	int status, wpid;

	if (pid == 0)
	{
		printf("---child, my parent = %d, going to sleep 10s\n", getppid());
		sleep(10);
		printf("---child die---\n");
		return 10;	//特殊值
	}
	else if (pid > 0)
	{
		if ((wpid = wait(&status)) ==-1)	//if ((wpid = wait(NULL)) ==-1) 传NULL代表不关心子进程结束原因
		{
			perror("wait error");
			exit(1);
		}
		if (WIFEXITED(status))	//为真说明子进程正常终止
		{
			printf("child exit with %d\n", WEXITSTATUS(status));
		}
		if (WIFSIGNALED(status))	//为真说明子进程被信号终止
		{
			printf("child kill with signal %d\n", WTERMSIG(status));
		}
		printf("------parent wait finish\n");
	}
	else
	{
		perror("fork");
		return 1;
	}

	return 0;
}
