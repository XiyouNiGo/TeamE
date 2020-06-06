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


int va = 0;
pthread_t tid;
void signal_fun(int signo);
void *pth_fun(void *pth_arg);
void print_err(char *str,int line,int err_no);
#define PTH_NUMS 2   //次线程数量
int main(void)
{
    signal(SIGINT,signal_fun);
    int i = 0;
    int ret = 0;
    va =3;
    ret = pthread_create(&tid,NULL,pth_fun,NULL);
    if(ret != 0)
        print_err("pthread_create failed",__LINE__,ret);
    
    while(1)
    {
        va = va +1;
        sleep(1);
    }

    
}
void signal_fun(int signo)  //信号捕捉函数
{
    pthread_cancel(tid);
    exit(0);
}

void *pth_fun(void *pth_arg)  //线程函数
{
        if(va == 3)
        {
            printf("va == %d\n",va);
            va =0;
        }
    return NULL;
}

void print_err(char *str,int line,int err_no)  //出错函数 错误号
{
    printf("%d , %s : %s",line,str,strerror(err_no));
    exit(-1);
}
