
/*************************************************************************
  > File Name: pthread_mutex.c
  > Author: NiGo
  > Mail: nigo@xiyoulinux.org
  > Created Time: 2020年05月15日 星期五 21时44分24秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>
#include <string.h>

pthread_mutex_t mutex;  //线程共享全局变量

void my_err(const char *str)
{
    perror(str);
    exit(1);
}

void *thread_function(void *arg)
{
    srand((unsigned int)time(NULL));
    while (1)
    {
        pthread_mutex_lock(&mutex);
        printf("hello ");
        sleep(rand() % 3);  //长时间操作共享资源，易使CPU易主，产生与时间有关的错误
        printf("world\n");
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 3);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t tid;
    int ret;

    srand((unsigned int)time(NULL));    //保证随机性
    if ( (ret = pthread_mutex_init(&mutex, NULL)) != 0)
    {
        fprintf(stderr, "pthread_mutex_init error : %s\n", strerror(ret));
        exit(1);
    }
    if ( pthread_create(&tid, NULL, thread_function, NULL) != 0)
    {
        fprintf(stderr, "pthread_create error : %s\n", strerror(ret));
        exit(1);
    }
    while (1)
    {
        pthread_mutex_lock(&mutex);
        printf("HELLO ");
        sleep(rand() % 3);  
        printf("WORLD\n");
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 3);
    }

    pthread_join(tid, NULL);
    pthread_exit( (void*)0 );
}
