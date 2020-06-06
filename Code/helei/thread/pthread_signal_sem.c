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
#include<semaphore.h>
#include<signal.h>
//  
//  如果主线程结束，次线程自动终止
//  所以设置主线程为死循环
//  线程依赖进程

#define SECON_PTH_NUMS 2
#define SEM_NUMS       1
typedef struct pthread_arg
{
    pthread_t tid;
    int pthno;
    int fd;
}ptharg;
sem_t sem[SEM_NUMS];
void print_err(char *,int ,int );
void *pth_fun(void *);
void signal_fun(int );
int main(void)
{
    int fd = 0;
    signal(SIGINT,signal_fun);
    //捕捉ctrl+c 信号，进入signal_fun函数
    ptharg pth_arg[SECON_PTH_NUMS];

    fd = open("./file.txt",O_RDWR | O_CREAT | O_TRUNC, 0664);
    if(fd == -1)
        print_err("open ./file.txt",__LINE__ ,errno);
    for(int i=0 ;i < SEM_NUMS ; i++)
    {
        int ret = sem_init(&sem[i],0,1);
        if(ret != 0)
        {
            print_err("sem_init failed",__LINE__,errno);
        }
    }
    int i = 0 ;
    int ret = 0;
    //线程共享相同的文件描述符
    for(i = 0; i < SECON_PTH_NUMS ; i++)
    {
        pth_arg[i].fd = fd;
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
        sem_wait(&sem[0]);

        t++;
        printf("main thread : hello \n");

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
    int fd = ((ptharg *) pth_arg)->fd;
    int flag = ((ptharg * )pth_arg) ->pthno;
    while(1)
    {
        sem_wait(&sem[0]);
        write(fd,"hello ",6);
        write(fd,"world\n",6);
        sem_post(&sem[0]);
        printf("%d : hello\n",((ptharg *)pth_arg)->pthno);
    }
    return NULL;
}
void signal_fun(int signo)
{
    int ret;
    for(int i=0;i<SEM_NUMS;i++)
    {
        ret = sem_destroy(&sem[i]);
        if(ret != 0)
            print_err("sem destroy failed",__LINE__,errno);
    }
    exit(1);
}
