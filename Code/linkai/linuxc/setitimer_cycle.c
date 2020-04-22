#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>

void myfunc(int signo)
{
	printf("Hello signo");
}
int main()
{
	struct itimerval new_val, old_val;

    new_val.it_value.tv_sec = 2;
    new_val.it_value.tv_usec = 0;

    new_val.it_interval.tv_sec = 5;
    new_val.it_interval.tv_usec = 0;

	signal(SIGALRM, myfunc);	//捕捉信号

    if (setitimer(ITIMER_REAL, &new_val, &old_val)  == -1)
    {
        perror("setitimer error");
        exit(1);
    }

    while (1);

    return 0;
}
