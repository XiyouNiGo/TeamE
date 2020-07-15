/*************************************************************************
	> File Name: kill_raise.c
	> Author: 
	> Mail: 
	> Created Time: 2020年06月30日 星期二 01时15分58秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<signal.h>

int main(void)
{
    //kill(getpid(),SIGUSR1);
    //杀死当前进程

    //raise(SIGKILL);
    //杀死当前进程

    kill(getppid(),SIGTERM);
    //要杀死当前终端，不能在当前终端下运行改程序

    kill(getppid(),SIGUSR1);
    //可在当前终端下，杀死该终端
}
