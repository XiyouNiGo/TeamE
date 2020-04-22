
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
    printf("Hello sigaction, signo : %d\n", signo);
    sleep(10);  //此时生效的是sa_mask，函数结束时再次捕捉，且仅一次（常规信号不支持排队且信号最多产生一次
}

int main()
{
    struct sigaction act, oldact;

    act.sa_flags = 0;   //默认模式
    act.sa_handler = myfunc;    //设置回调函数
    sigemptyset(&(act.sa_mask));    //只在回调函数内生效

    if (sigaction(SIGINT, &act, &oldact) == -1) //注册信号捕捉函数
    {
        perror("sigaction error");
        exit(1);
    }

    while (1);
}
