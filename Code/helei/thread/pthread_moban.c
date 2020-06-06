/*************************************************************************
	> File Name: pthread_cond.c
	> Author: 
	> Mail: 
	> Created Time: 2020年05月09日 星期六 20时34分20秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<signal.h>
#include<fcntl.h>

#define PTH_NUMS 2  //次线程数量

void signal_fun(int signo);
pthread_t pid;              //pid[PTH_NUMS];
void *pth_fun(void *pth_arg);
void print_err(char *str,int line,int err_no);
int main(void)
{
    signal(SIGINT,signal_fun);

    
}
void signal_fun(int signo)  //信号捕捉函数
{
    pthread_cancel(tid);
    exit(0);
}

void *pth_fun(void *pth_arg)  //线程函数
{
    return NULL;
}

void print_err(char *str,int line,int err_no)  //出错函数 错误号
{
    printf("%d , %s : %s",line,str,strerror(err_no));
    exit(-1);
}
