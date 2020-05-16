
/*************************************************************************
    > File Name: pthread_create.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年05月06日 星期三 11时48分08秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

void my_err(const char *str)
{
     perror(str);
     exit(1);
}

void *thread_function(void *arg)
{
    printf("thread : pid is %d, tid is %lu\n", getpid(), pthread_self());
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t tid;
    
    printf("main : pid is %d, tid is %lu\n", getpid(), pthread_self());
    if (pthread_create(&tid, NULL, thread_function, NULL) != 0)
    {
        my_err("pthread_create error");
    }
    //因为线程共用一块地址空间，主线程退出，其余线程也一起退出
    //所以等1s
    sleep(1);

    return 0;
}
