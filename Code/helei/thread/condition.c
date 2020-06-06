/*************************************************************************
	> File Name: condition.c
	> Author: 
	> Mail: 
	> Created Time: 2020年05月19日 星期二 09时27分41秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

void * thread1(void *arg)
{
    pthread_cleanup_push(pthread_mutex_unlock,NULL);
    //从pthread_cleanup_push(  )调用点到pthread_cleanup_pop()之间的函数段中的终止动作都将执行pthread_cleanup_push()所指定的清理函数
    

    while(1)
    {
        printf("thread1 is running\n");
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond,&mutex);
        printf("thread1 applied the condition\n");
        pthread_mutex_unlock(&mutex);
        sleep(4);
    }
    pthread_cleanup_pop(0);
}

void *thread2 (void * arg)
{
    while(1)
    {
        printf("thread2 is running\n");
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond,&mutex);
        printf("thread2 applied the condition\n");
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
}

int main(void)
{
    pthread_t tid1,tid2;
    printf("condition variable study!\n");
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
    pthread_create(&tid1,NULL,(void *)thread1,NULL);
    pthread_create(&tid2,NULL,(void *)thread2,NULL);
    do{
        pthread_cond_signal(&cond);

    }while(1);
    sleep(50);
    pthread_exit(0);
}
