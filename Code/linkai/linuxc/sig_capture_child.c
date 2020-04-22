
/*************************************************************************
    > File Name: sig_capture_son.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年04月23日 星期四 00时53分04秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void capture_child(int signo)
{
    pid_t wpid;
    int status;

    //while ((wpid = wait(NULL)) == -1)
    while ((wpid = waitpid(-1, &status, 0)) != -1)
    {
        if (WIFEXITED(status))
        {
            printf("------wait child pid %d, return %d\n", wpid, WEXITSTATUS(status));
        }
    }

    return;
}
int main(int argc, char *argv[])
{
    pid_t pid;

    int i;
    for (i = 0; i < 15; i++)
    {
        if ((pid = fork()) == 0)
        {
            break;
        }
    }
    if (i == 15)
    {
        struct sigaction act;

        act.sa_flags = 0;
        act.sa_handler = capture_child;
        sigemptyset(&act.sa_mask);

        sigaction(SIGCHLD, &act, NULL);

        printf("Parent: pid = %d\n", getpid());

        while (1);
    }
    else
    {
        printf("Child: pid = %d\n", getpid());
        return i;
    }
}
