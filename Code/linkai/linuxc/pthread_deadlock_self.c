
/*************************************************************************
    > File Name: pthread_deadlock.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年05月16日 星期六 12时19分52秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>

void my_err(const char *str)
{
     perror(str);
     exit(1);
}

int main(int argc, char *argv[])
{
    pthread_mutex_t mutex;

    pthread_mutex_init(&mutex, NULL);

    pthread_mutex_lock(&mutex);

    printf("deadlock\n");

    pthread_mutex_lock(&mutex);

    printf("no deadlock\n");

    pthread_exit( (void*)0);
}
