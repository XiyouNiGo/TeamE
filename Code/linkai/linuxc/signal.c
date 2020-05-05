
/*************************************************************************
    > File Name: sigaction.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年04月22日 星期三 15时55分31秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>

void myfunc(int signo)
{
    printf("Hello signal, signo : %d\n", signo);
    sleep(10);
}

int main()
{
    if (signal(SIGINT, myfunc) == SIG_ERR) //注册信号捕捉函数
    {
        perror("signal error");
        exit(1);
    }

    while (1);
}
