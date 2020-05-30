#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM 5
int queue[NUM];                     //全局数组实现环形队列
sem_t blank_number, product_number; //空格信号量和产品信号量

void my_err(const char *str)
{
     perror(str);
     exit(1);
}

void *producer(void *arg)
{
    int i = 0;

    while (1)
    {
        sem_wait(&blank_number);    //生产者将空格子数--，为0则阻塞等待
        queue[i] = rand()%1000 + 1;
        printf("=====Product %d\n", queue[i]);
        sem_post(&product_number);  //产品数++
        i = (i+1)%NUM;
        sleep(rand() % 1);  //方便另一个获得CPU
    }
}

void *consumer(void *arg)
{
    int i = 0;

    while (1)
    {
        sem_wait(&product_number);
        printf("-----Consume %d\n", queue[i]);
        queue[i] = 0;
        sem_post(&blank_number);
        i = (i+1)%NUM;
        sleep(rand() % 3);
    }
}

int main(int argc, char *argv[])
{
    pthread_t pid, cid;
    
    sem_init(&blank_number, 0, NUM);    //0表示在线程间
    sem_init(&product_number, 0, 0);    //0表示在线程间
    
    pthread_create(&pid, NULL, producer, NULL);
    pthread_create(&cid, NULL, consumer, NULL);

    pthread_join(pid, NULL);
    pthread_join(cid, NULL);

    sem_destroy(&blank_number);
    sem_destroy(&product_number);

    return 0;
}
