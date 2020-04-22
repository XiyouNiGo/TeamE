#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main()
{
	pid_t pid = fork();

	if (pid > 0)
	{
		printf("Parent: pid = %d\n", getpid());
		while (1);
	}
	else if (pid == 0)
	{
		printf("Child: pid = %d, ppid = %d\n", getpid(), getppid());
		sleep(2);
		kill(getppid(), SIGKILL);
	}

	return 0;
}
