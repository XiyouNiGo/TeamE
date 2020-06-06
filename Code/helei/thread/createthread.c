/*************************************************************************
	> File Name: createthread.c
	> Author: 
	> Mail: 
	> Created Time: 2020年05月06日 星期三 23时24分41秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

int * thread(void *arg)
{
    pthread_t newthid;
    newthid = pthread_self();   
    //获得线程id
    printf("this is a new thread, thread ID = %u\n",newthid);
    return NULL;
}

int main(void)
{
    pthread_t thid;
    printf("main thread, ID is %u\n",pthread_self());
    if(pthread_create(&thid,NULL,(void *)thread,NULL) != 0) 
    {
        //pthread_create 线程创建函数，成功返回0
        printf("thread creation failed\n");
        exit(1);
    }
    sleep(1);
    exit(0);
}
