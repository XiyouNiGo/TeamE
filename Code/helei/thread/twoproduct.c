/*************************************************************************
	> File Name: product1.c
	> Author: 
	> Mail: 
	> Created Time: 2020年05月30日 星期六 10时34分13秒
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

#define N 100
#define true 1
#define producerNum 10
#define consumerNum 5
#define sleepTime 1

//对于线程函数即使不需要参数，也不能写成void 要写成void * a ,在线程创建函数处写NULL
//
typedef int semaphore;
typedef int item;

//
//该缓冲区类似于队列，in 和 out 分别代表位置，入队和出对
item buffer[N] = { 0 };
int in = 0;
int out = 0;
int proCount = 0;    //记录产品ID
//empty 和 full 去记录缓冲区内产品数量，
//使用极端例子，假设只生产不消费，empty不断减小，减为0 时 缓冲区满，停止写入数据

semaphore mutex = 1,empty = N, full = 0, proCmutex = 1;

//用锁防止线程运行到中途时间片到
void * producer(void * a)
{
    while(true)
    {
        while(mutex <= 0);
        
        while(empty <= 0)
        {
            printf("缓冲区已满！\n");
        }
        empty --;
        mutex--;
        proCount ++;
        buffer[in] = proCount;
        in = (in + 1) % N;
        printf("生产的产品ID %d,缓冲区位置 %d\n",proCount,in);
        mutex ++;
        full ++;
        sleep(1);
    }
}
void *consumer(void * b)
{
    while(true)
    {
        while(mutex <= 0);
        while(full <= 0)
        {
            printf("缓冲区为空\n");
        }
        full --;
        mutex --;
        int nextc = buffer[out];
        buffer[out] = 0;
        out = (out + 1) % N;
        printf("\t\t\t\t消费的产品ID %d，缓冲区位置为%d\n",nextc,out);
        mutex ++;
        empty ++;
        sleep(1);

    }
}
int main(void)
{
    pthread_t pthreadPool[producerNum + consumerNum];
    for(int i = 0;i< producerNum;i++)
    {
        pthread_t temp;
        if(pthread_create(&temp,NULL,producer,NULL) == -1)
        {
            printf("%d : create producer pthread failed\n",__LINE__);
            exit(1);
        }
        pthreadPool[i] = temp;
    }
    for(int i = 0;i < consumerNum;i++ )
    {
        pthread_t temp;
        if(pthread_create(&temp,NULL,consumer,NULL) == -1)
        {
            printf("%d : create consumer pthread failed\n",__LINE__);
            exit(2);
        }
    }
    void *result;
    //使用pthread_join结束线程是因为线程运行会产生碎片垃圾，系统无法自动回收
    for(int i = 0;i < producerNum + consumerNum ;i++)
    {
        if(pthread_join(pthreadPool[i],&result) == -1)
        {
            printf("failed to recollect\n");
            exit(3);
        }
    }
    return 0;

}
