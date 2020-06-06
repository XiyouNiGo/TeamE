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
#include<semaphore.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<signal.h>
#include<fcntl.h>

#define PTH_NUMS 2  //次线程数量
#define PTHEXIT -1
typedef struct pthread_arg
{
    pthread_t tid;
    int pthno;
    int fd;
}ptharg;
int pth_exit_flag[PTH_NUMS];
sem_t sem[3];

pthread_mutex_t mutex;
ptharg pth_arg[PTH_NUMS];
void signal_fun(int signo);
pthread_t pid;              //pid[PTH_NUMS];
void *pth_fun(void *pth_arg);
void print_err(char *str,int line,int err_no);
int main(void)
{
    sem_init(&sem[0],0,1);
    for(int i =1;i<3;i++)
        sem_init(&sem[i],0,0);

    pthread_mutex_init(&mutex,NULL);
    signal(SIGINT,signal_fun);
    int fd = 0;
    fd = open("./file.txt",O_CREAT | O_TRUNC | O_RDWR,0664);
    for(int i=0; i< PTH_NUMS;i++)
    {
        pth_arg[i].fd = fd;
        pth_arg[i].pthno = i;
        int ret = 0;
        ret = pthread_create(&pth_arg[i].tid,NULL,pth_fun,(void *)&pth_arg[i]);
        if(ret != 0)
        {
            print_err("pthread create failed",__LINE__,ret);
            exit(-1);
        }
    }
    signal(SIGALRM,signal_fun);
    alarm(1);

    while(1)
    {
        //pthread_mutex_lock(&mutex);
        sem_wait(&sem[2]);
        printf("3333\n");
        write(fd,"hello ",6);
        write(fd,"world\n",6);
        //pthread_mutex_unlock(&mutex);
        sem_post(&sem[0]);
    }


    
}
void signal_fun(int signo)  //信号捕捉函数
{
    if(SIGALRM == signo)
    {
        for(int i =0;i <PTH_NUMS;i++)
        {
            pthread_mutex_destroy(&mutex);
         //   pthread_cancel(pth_arg[i].tid);
            pth_exit_flag[i] = PTHEXIT;
        }
        
    }
    exit(0);
}

void *pth_fun(void *pth_arg)  //线程函数
{
    int fd = ((ptharg *)pth_arg)->fd;
    int pthno = ((ptharg *)pth_arg)->pthno;
    while(1)
    {
        //pthread_mutex_lock(&mutex);
        //sem_wait(&sem);
        //printf("-");
        if(pthno == 0)
        {
            sem_wait(&sem[0]);
            printf("1111\n");
            write(fd,"hello ",6);
            write(fd,"world\n",6);
            sem_post(&sem[1]);
        }
        else if(pthno == 1)
        {
            sem_wait(&sem[1]);
            printf("2222\n");
            write(fd,"hello ", 6);
            write(fd,"world\n",6);
            sem_post(&sem[2]);
        }
        if(pth_exit_flag[pthno] == PTHEXIT)
            break;

        //pthread_mutex_unlock(&mutex);
        //sem_post(&sem);
    }
    return NULL;
    //pthread_exit(NULL);
}

void print_err(char *str,int line,int err_no)  //出错函数 错误号
{
    printf("%d , %s : %s",line,str,strerror(err_no));
    exit(-1);
}
