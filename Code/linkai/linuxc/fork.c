#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	pid_t pid;
	printf("Process Creation Study\n");
	pid = fork();
	switch (pid)
	{
		case 0:
			printf("Child Process is running, ChildPid is %d, ParentPid is %d\n", pid, getppid());
			break;
		case -1:
			perror("Process Creation failed\n");
			break;
		default:
			printf("Parent Process is running, ChildPid is %d, ParentPid is %d\n", pid, getpid());
			break;
	}
	return 0;
}
