
/*************************************************************************
    > File Name: signal_inherit.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年04月23日 星期四 22时30分06秒
 ************************************************************************/

//测试子进程是否继承父进程signal注册的信号

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void my_err(const char *str)
{
     perror(str);
     exit(1);
}

void how(int signo)
{
    printf("----inherit success!\n");
    exit(0);
}

int main(int argc, char *argv[])
{
    pid_t pid;

    //注册
    signal(SIGINT, how);

    if ((pid = fork()) == -1)
    {
        my_err("fork error");
    }
    else if (pid > 0)
    {
        printf("Parent: I has died\n");
        exit(1);
    }
    else
    {
        printf("Child: This is my show time\n");
        while (1)
        {
            sleep(5);
            printf("Child: Yeah, I'm still alive\n");
        }

        return 0;
    }
}
