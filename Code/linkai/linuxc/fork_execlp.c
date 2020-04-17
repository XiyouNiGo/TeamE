#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	pid_t pid = fork();
	switch (pid)
	{
		case 0:
			printf("Child Process is running, ChildPid is %d, ParentPid is %d\n", pid, getppid());
			execlp("date", "date", NULL);	//NULL为哨兵
			perror("execlp error");			//execlp函数成功时无返回值,失败时返回-1
			break;
		case -1:
			perror("Process Creation failed\n");
			break;
		default:
			printf("Parent Process is running, ChildPid is %d, ParentPid is %d\n", pid, getpid());
			sleep(1);
			break;
	}
	return 0;
}
