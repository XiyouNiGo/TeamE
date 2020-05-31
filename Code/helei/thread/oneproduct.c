/*************************************************************************
	> File Name: oneproduct.c
	> Author: 
	> Mail: 
	> Created Time: 2020年05月30日 星期六 11时09分30秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<semaphore.h>

//如果线程在该时间片复内完成，线程就结束了，如果未完成，系统会将该线程放入就绪队列，等待下制CPU再度调度运行它，而且是接着上次执行的状态接着执行，
//用goto 语句 去防止不断的刷新缓冲区空 或缓冲区满

typedef  int semaphore;

typedef int item;

#define true 1
#define N 3
#define sleepTime 1

item buffer[N] = { 0 };
int in = 0;
int out = 0;
int proCount = 1;

semaphore mutex = 1,empty = N, full = 0,proCmutex = 1;
void * producer(void * a)
{
    while(true)
    {
        while(empty <= 0)
        {
            printf("缓冲区已满！\n");
            goto a;
        }
        empty --;
       while(mutex <= 0); 
        mutex --;
        proCount ++;
        buffer[in] = proCount;
        in = (in + 1) % N;
        
        printf("生产的产品ID %d，缓冲区位置 %d\n",proCount,in);
        
        mutex++;
        full ++;
        a:sleep(2);
    }
}
void * consumer(void * b)
{
    while(true)
    {
        while(mutex <= 0);
        while(full <= 0)
        {
            printf("缓冲区为空！\n");
            goto a;
        }
        full --;
        mutex --;
        int nextc = buffer[out];
        buffer[out] = 0;
        out = (out + 1) % N;
        
        printf("\t\t\t\t消费的产品ID %d,缓冲区位置为%d\n",nextc,out);
        mutex++;
        empty ++;
        a:sleep(sleepTime);
    }
}

int main(void)
{
    pthread_t proID,conID;

    if(pthread_create(&proID,NULL,producer,NULL) == -1)
    {
        
        printf("%d : create producer pthread failed \n",__LINE__);
        exit(1);
    }
    if(pthread_create(&conID,NULL,consumer,NULL) == -1)
    {
        printf("%d : create consumer pthread failed\n",__LINE__);
        exit(2);
    }
    void * result;
    if(pthread_join(proID,&result) == -1)
    {
        printf("%d : recollect producer failed\n",__LINE__);
        exit(3);
    }
    if(pthread_join(conID,&result) == -1)
    {
        printf("%d : recollect consumer failed\n",__LINE__);
        exit(4);
    }
    return 0;
}
