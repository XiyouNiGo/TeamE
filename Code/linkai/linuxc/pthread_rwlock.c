
/*************************************************************************
    > File Name: ptrhead_rwlock.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年05月16日 星期六 12时01分05秒
 ************************************************************************/

//锁只有一把
//读锁共享，写锁复制
//写锁优先级高
//相较于mutex，读线程多时，访问效率高

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>

pthread_rwlock_t rwlock;
int count = 0;

void *thread_write(void *arg)
{
    int i = (long)arg;
    while (1)
    {
        pthread_rwlock_wrlock(&rwlock);
        printf("==========write %d : tid = %lu, ++count = %d\n", i, pthread_self(), ++count);
        pthread_rwlock_unlock(&rwlock);
        usleep(10000);
    }

    return NULL;
}

void *thread_read(void *arg)
{
    int i = (long)arg;
    while (1)
    {
        pthread_rwlock_rdlock(&rwlock);
        printf("----------read %d : tid = %lu, count = %d\n", i, pthread_self(), count);
        pthread_rwlock_unlock(&rwlock);
        usleep(2000);
    }

    return NULL;
}

void my_err(const char *str)
{
     perror(str);
     exit(1);
}

int main(int argc, char *argv[])
{
    int i;
    pthread_t tid[8];

    pthread_rwlock_init(&rwlock, NULL);

    for (i = 0; i < 3; i++)
    {
        pthread_create(&tid[i], NULL, thread_write, (void*)i);
    }

    for (i = 0; i < 5; i++)
    {
        pthread_create(&tid[i+3], NULL, thread_read, (void*)i);
    }

    for (i = 0; i < 8; i++)
    {
        pthread_join(tid[i], NULL);
    }

    pthread_rwlock_destroy(&rwlock);

    return 0;
}
