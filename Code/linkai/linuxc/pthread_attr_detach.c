
/*************************************************************************
    > File Name: pthread_attr_detach.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年05月15日 星期五 15时07分50秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>
#include <string.h>

void my_err(const char *str)
{
     perror(str);
     exit(1);
}

void *function(void *arg)
{
    printf("thread : pid is %d, tid is %lu\n", getpid(), pthread_self());
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t tid;
    pthread_attr_t attr;
    int ret;
    if ( (ret = pthread_attr_init(&attr)) != 0)
    {
        fprintf(stderr, "pthread_attr_init error: %s\n", strerror(ret));
        exit(1);
    }

    if ( (ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)) != 0)
    {
        fprintf(stderr, "pthread_attr_setdetachstate error: %s\n", strerror(ret));
        exit(1);
    }

    if ( (ret = pthread_create(&tid, &attr, function, NULL)) != 0)
    {
        fprintf(stderr, "pthread_create error: %s\n", strerror(ret));
        exit(1);
    }
    //pthread_detach(tid);
    if ( (ret = pthread_attr_destroy(&attr)) != 0)
    {
        fprintf(stderr, "pthread_attr_destroy error: %s\n", strerror(ret));
        exit(1);
    }
    //设置成分离态不用回收
    if ( (ret = pthread_join(tid, NULL)) != 0)
    {
        fprintf(stderr, "pthread_join error: %s\n", strerror(ret));
        exit(0);
    }
    printf("main : pid is %d, tid is %lu\n", getpid(), pthread_self());

    pthread_exit((void*)0);
}
