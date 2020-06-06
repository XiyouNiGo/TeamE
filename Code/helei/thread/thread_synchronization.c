/*************************************************************************
	> File Name: two_pth.c
	> Author: 
	> Mail: 
	> Created Time: 2020年05月07日 星期四 21时40分30秒
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
#include<semaphore.h>
//  
//  如果主线程结束，次线程自动终止
//  所以设置主线程为死循环
//  线程依赖进程

#define SECON_PTH_NUMS 2

typedef struct pthread_arg
{
    pthread_t tid;
    int pthno;
    int fd;
}ptharg;
sem_t sem[3];
void print_err(char *,int ,int );
void *pth_fun(void *);
void signal_fun(int );
int main(void)
{
    signal(SIGINT,signal_fun);
    sem_init(&sem[0],0,1);
    sem_init(&sem[1],0,0);
    sem_init(&sem[2],0,0);
    ptharg pth_arg[SECON_PTH_NUMS];


    int i = 0 ;
    int ret = 0;
    //线程共享相同的文件描述符
    for(i = 0; i < SECON_PTH_NUMS ; i++)
    {
        pth_arg[i].pthno = i;
// int pthread_create (pthread_t *thread,constpthread_attr_t *attr,
        //              void *(*start_routine )(void *),void *arg);
        ret = pthread_create(&pth_arg[i].tid,NULL,pth_fun,(void *)&pth_arg[i]);
        //函数成功返回0 ，错误返回错误号  但不是errno
        if(ret != 0)
            print_err("pthread_create failed",__LINE__,ret);
    }

    int t = -1;
    while(1)
    {
        sem_wait(&sem[2]);

        printf("33333\n");
        sem_post(&sem[0]);
    }
}
void print_err(char *buf,int line,int err_no)
{
    printf("%d : %s , %s\n",line,buf,strerror(err_no));
    exit(-1);
}
void *pth_fun(void *pth_arg)
{
    int flag = ((ptharg * )pth_arg) ->pthno;
    int i = -1;
    while(1)
    {
        if(flag == 0)
        {
            sem_wait(&sem[0]);
            printf("11111\n");
            sem_post(&sem[1]);
       
        }
        if(flag == 1)
        {
            sem_wait(&sem[1]);
            printf("22222\n");
            sem_post(&sem[2]);

        }
    }
    return NULL;
}
void signal_fun(int signo)
{
    for(int i =0 ;i <3 ; i++)
    {
        sem_destroy(&sem[i]);
    }
    exit(0);
}
