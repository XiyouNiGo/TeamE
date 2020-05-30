
/*************************************************************************
    > File Name: philosopher.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年05月30日 星期六 22时09分36秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM 5                   //5位哲学家
sem_t max[2];                 //最多允许4位哲学家同时拿起左手或右手筷子(0为右手，1为左手)
sem_t chopsticks[NUM];          //信号量模拟筷子

void my_err(const char *str)
{
     perror(str);
     exit(1);
}

void *philosopher(void *arg)
{
    int i = (long)arg;
    int left = i;               //左手筷子定义为i
    int right = (i+1) % NUM;    //右手筷子定义为i+1,若i == 4, right = 0
    int first;                  //第一次是左手还是右手？
    int second;                 //第二次？
    int is_first_left;          

    while (1)
    {
        //思考时间
        printf("-----哲学家%d思考中\n", i);
        sleep(rand() % 5 + 1);  //让他思考那么1-5s
        //随机左右手
        if (is_first_left = (rand() % 2))
        {
            first = left;
            second = right;
        }
        else
        {
            first = right;
            second = left;
        }
        //拿第一只筷子
        sem_wait(&max[is_first_left]);
        sem_wait(&chopsticks[first]);
        if (is_first_left)         
            printf("=====第一次，哲学家%d拿起了左手的筷子%d\n", i, left);
        else
            printf("=====第一次，哲学家%d拿起了右手的筷子%d\n", i, right);
        //第二只
        sem_wait(&max[!is_first_left]);
        sem_wait(&chopsticks[second]);
        if (is_first_left)         
            printf("=====第二次，哲学家%d拿起了右手的筷子%d\n", i, right);
        else
            printf("=====第二次，哲学家%d拿起了左手的筷子%d\n", i, left);
        //用餐时间
        printf("-----哲学家%d用餐中\n", i);
        sleep(rand() % 5 + 1);
        //放筷子吧
        sem_post(&chopsticks[left]);
        sem_post(&max[1]);
        printf("=====哲学家%d放下了左手的筷子%d\n", i, left);
        sem_post(&chopsticks[right]);
        sem_post(&max[0]);
        printf("=====哲学家%d放下了右手的筷子%d\n", i, right);
    }
}

int main(int argc, char *argv[])
{
    srand( time(NULL) );

    pthread_t pid[NUM];
    //初始化
    for (int i = 0; i < 2; i++)
        sem_init(&max[i], 0, 4);
    for (int i = 0; i < 5; i++)
    {
        sem_init(&chopsticks[i], 0, 1);
    }
    //创建哲学家
    for (int i = 0; i < 5; i++)
    {
        pthread_create(&pid[i], NULL, philosopher, (void*)i);
    }
    //销毁
    for (int i = 0; i < 5; i++)
    {
        pthread_join(pid[i], NULL);
        sem_destroy(&chopsticks[i]);
    }
    for (int i = 0; i < 2; i++)
        sem_destroy(&max[i]);

    return 0;
}
