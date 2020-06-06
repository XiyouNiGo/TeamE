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
pthread_mutex_t mutex;
pthread_cond_t cond;
void signal_fun(int signo);
void *pth_fun(void *pth_arg);
void print_err(char *str,int line,int err_no);
#define PTH_NUMS 2   //次线程数量
int main(void)
{
    signal(SIGINT,signal_fun);
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
    int i = 0;
    int ret = 0;
    ret = pthread_create(&tid,NULL,pth_fun,NULL);
    if(ret != 0)
        print_err("pthread_create failed",__LINE__,ret);
    
    while(1)
    {
        pthread_mutex_lock(&mutex);
        va =va +1;
        if(va == 3)
        {
            pthread_cond_signal(&cond);
        }
        pthread_mutex_unlock(&mutex);
        sleep(1);

    }

}
void signal_fun(int signo)  //信号捕捉函数
{
    pthread_cancel(tid);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    exit(0);
}

void *pth_fun(void *pth_arg)  //线程函数
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        //之所以将mutex传递给该函数，是因为害怕休眠后导致锁没有解开
        //其他的线程就无法使用该把互斥锁
        //传递mutex的目的就是希望该函数休眠的时候能够解锁，
        //让其他线程能够使用该锁
        pthread_cond_wait(&cond,&mutex);
        printf("va = %d\n",va);
        va = 0;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
void print_err(char *str,int line,int err_no)  //出错函数 错误号
{
    printf("%d , %s : %s",line,str,strerror(err_no));
    exit(-1);
}
