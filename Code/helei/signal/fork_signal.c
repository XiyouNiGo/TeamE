/*************************************************************************
	> File Name: my_signal.c
	> Author: 
	> Mail: 
	> Created Time: 2020年06月29日 星期一 11时17分31秒
 ************************************************************************/

#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
typedef void (*sighandler_t)(int);
void signal_fun1(int signo)
{
    printf("\nrecv sigint signal signo= %d\n",signo);
}
int main(int argc,char **argv,char **environ)
{
    sighandler_t ret = NULL;
    /*
     *
     * ret = signal(SIGQUIT,SIG_IGN);
    ret = signal(SIGINT,SIG_IGN);
    //如果是忽略或默认信号时，子进程会继承父进程的信号
*/
    ret = signal(SIGINT,signal_fun1);
    //如果是捕获信号，子进程会修改处理方式为默认处理方式
    pid_t pid;
    pid = fork();
    if(pid > 0)
    {

        
    }
    else 
    {
        execve("./new_pro",argv,environ);
    }
    while(1);
}



