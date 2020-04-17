#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int glo_var = 5;

int main()
{
	pid_t pid;
	int var = 1;
	int i;

	printf("fork is diffirent with vfork\n");
	pid = fork();	//pid = fork();
	switch (pid)
	{
		case 0:
			i = 3;
			while (i-- > 0)
			{
				printf("Child process is running\n");
				var++;
				glo_var++;
				sleep(1);
			}
			printf("Child' glo_var = %d, var = %d\n", glo_var, var);
			break;
		case -1:
			perror("fork");
			exit(1);
		default:
			while (i-- > 0)
			{
				printf("Parent process is running\n");
				glo_var++;
				var++;
				sleep(1);
			}
			printf("Parent's glo_var = %d, var = %d\n", glo_var, var);
			exit(0);
	}
}
