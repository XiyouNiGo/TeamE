/*************************************************************************
	> File Name: philosopher.c
	> Author: 
	> Mail: 
	> Created Time: 2020年05月30日 星期六 23时51分09秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<time.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>

//原子操作： 指的是有多部操作组成的一个操作。如果该操作不能原子的进行，则要么执行完所有步骤，要么一步也不执行，不可能只执行所有步骤的一个子集
//现代操作系统中，一般都提供了原子操作来实现一些同步操作，所谓原子操作，也就是一个独立而不可分割的操作，在单核环境中，一般意义下原子操作中线程不会被切换，线程切换要么在原子操作之前，要么在原子操作之后。更广泛的意义下原子操作是指一系列必须整体完成的操作步骤，如果任何一步操作没有完成，那么所有完成的步骤，都必须回滚，这样就可以保证要么所有操作步骤都未完成，要么所有操作步骤都被完成
#define N 5
#define sleepTime 2
//信号量使用的参数
pthread_mutex_t chopsticks [N];
pthread_mutex_t mutex;
sem_t r;
int philosophers[N] = { 0,1,2,3,4 };
//swap指令需要的参数
int islocked[N] = { 0 };
//互斥量使用的参数
pthread_mutex_t chops[N];

//交换函数 ，汇编指令

void xchg(int *x,int *y)
{
    __asm__("xchgl %0,%1":"=r" (*x) : "m" (*y));
}

//这个函数使用的解决方法是最多容许四个哲学家拿起左筷子

void philosopher(void * arg)
{
    int i = *(int *)arg;
    int left = i;
    int right = (i + 1) % N;
    int leftkey;
    int rightkey;

    while(1)
    {
        leftkey = 1;
        rightkey = 1;
        printf("哲学家 %d正在思考问题\n",i);
        sleep(sleepTime);
        printf("哲学家 %d饿了\n",i);
        sem_wait(&r);
        //56-60行   如果想要的筷子被占用，让该线程挂起，陷入死循环
        do{
            xchg(&leftkey,&islocked[left]);
        }while(leftkey);
        //筷子属于共享资源，不能同时被拿起，要加锁
        pthread_mutex_lock(&chopsticks[left]);
        printf("哲学家%d拿起了 %d号筷子，现在只有一支筷子，不能进餐\n",i,left);
        do{
            xchg(&rightkey,&islocked[right]);
        }while(rightkey);
        pthread_mutex_unlock(&chopsticks[right]);
        printf("哲学家%d拿起了 %d号筷子，现在有两只筷子，开始进餐\n",i,right);
        sleep(sleepTime);
        islocked[left] = 0;
        printf("哲学家%d放下了 %d号筷子\n",i,left);
        pthread_mutex_unlock(&chopsticks [left]);

        islocked[right] = 0;
        printf("哲学家%d放下了 %d号筷子\n",i,right);
        pthread_mutex_unlock(&chopsticks[right]);
        sem_post(&r);

    }
}
void philosopher2(void *arg)
{
    int i = *(int *)arg;
    int left = i;
    int right = ( i + 1 ) % N;
    while(1)
    {
        printf("哲学家%d 正在思考问题\n",i);
        sleep(sleepTime);
        printf("哲学家%d 饿了\n",i);
        if(i % 2 == 0 )   //偶数哲学家，先右后左
        {
            pthread_mutex_lock(&chopsticks [right]);
            printf("哲学家%d拿起了%d号筷子，现在有一支筷子，不能进餐\n",i,right);
            pthread_mutex_lock(&chopsticks[left]);
            printf("哲学家%d拿起了%d号筷子，现在有两支筷子，开始进餐\n",i,left);
            sleep(sleepTime);
            pthread_mutex_unlock(&chopsticks [right]);
            printf("哲学家%d放下了%d号筷子\n",i,right);
            pthread_mutex_unlock(&chopsticks [left]);
            printf("哲学家%d放下%d号筷子\n",i,left);
        }
        else    //奇数哲学家，先右后左
        {
            pthread_mutex_lock(&chopsticks[left]);
            printf("哲学家%d拿起了%d号筷子，现在有一支筷子，不能进餐\n",i,left);
            pthread_mutex_lock(&chopsticks[right]);
            printf("哲学家%d拿起了%d号筷子，现在有两支筷子，开始进餐\n",i,right);
            sleep(sleepTime);
            pthread_mutex_unlock(&chopsticks[right]);
            printf("哲学家%d放下了%d号筷子\n",i,right);
            pthread_mutex_unlock(&chopsticks [left]);
            printf("哲学家%d放下了%d号筷子\n",i,left);
        }

    }

}
//加入一把全局锁，是为了能让某一个线程得到想要的资源，尽可能满足这一个线程
//注意解锁位置
void philosopher3(void * arg)
{

    int i = *(int *)arg;
    int left = i;
    int right = (i + 1) % N;
    while(1)
    {
        printf("哲学家%d正在思考问题\n",i);
        sleep(sleepTime);
        printf("哲学家%d饿了\n",i);
        pthread_mutex_lock(&mutex);
        pthread_mutex_lock(&chopsticks[left]);
        printf("哲学家%d拿起了%d号筷子，现在只有一支筷子，不能进餐\n",i,left);
        pthread_mutex_lock(&chopsticks[right]);
        printf("哲学家%d拿起了%d号筷子，现在有两支筷子，开始进餐\n",i,right);
        pthread_mutex_unlock(&mutex);
        sleep(sleepTime);
        pthread_mutex_unlock(&chopsticks[left]);
        printf("哲学家%d放下了%d号筷子\n",i,left);
        pthread_mutex_unlock(&chopsticks[right]);
        printf("哲学家%d放下了%d号筷子\n",i,right);
    }
}
int main(void)
{
    pthread_t PHD[N];
    pthread_mutex_init(&mutex,NULL);
    sem_init(&r,0,4);
    for(int i = 0;i<N;i++)
    {
        pthread_mutex_init(&chopsticks[i],NULL);
    }
    for(int i = 0; i< N ;i++)
    {
        pthread_create(&PHD[i],NULL,(void *)philosopher3,&philosophers[i]);
    }
    for(int i = 0 ;i < N;i++)
    {
        pthread_join(PHD[i],NULL);
    }
    sem_destroy(&r);
    for(int i = 0;i < N;i++)
    {
        pthread_mutex_destroy(&chopsticks[i]);
    }
    pthread_mutex_destroy(&mutex);
}
//sem_init(&r,0,4)
//设置信号量的初始值为4，保证最多有4个线程并行，第5个线程会阻塞
//p操作                                     v操作
//s = s - 1                                 s = s + 1;
//if(s >= 0)                                if(s > 0)
//    线程继续执行                                线程继续执行
//if(s < 0)                                 if(s <= 0)
//    线程阻塞                                    该线程去唤醒另一个在该信号量上等待的线程，然后继续执行
