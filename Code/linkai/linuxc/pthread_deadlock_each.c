
/*************************************************************************
    > File Name: pthread_deadlock_each.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年05月16日 星期六 12时24分05秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>

pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

void my_err(const char *str)
{
     perror(str);
     exit(1);
}

void *thread_function(void *arg)
{
    pthread_mutex_lock(&mutex2);

    printf("thread : possess mutex2, then I wanna mutex1\n");

    sleep(3);

    pthread_mutex_lock(&mutex1);

    printf("thread : get mutex1 successfully\n");

    return NULL;
}
int main(int argc, char *argv[])
{
    pthread_t tid;
    
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);

    pthread_create(&tid, NULL, thread_function, NULL);
    
    pthread_mutex_lock(&mutex1);

    printf("main : possess mutex1, then I wanna mutex2\n");

    sleep(3);

    pthread_mutex_lock(&mutex2);

    printf("main : get mutex2 successfully\n");

    pthread_exit( (void*)0);
}
