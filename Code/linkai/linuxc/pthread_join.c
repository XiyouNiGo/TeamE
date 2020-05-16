
/*************************************************************************
    > File Name: pthread_join.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年05月11日 星期一 17时52分06秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>

struct thread
{
    int val;
    char str[256];
};

void my_err(const char *str)
{
     perror(str);
     exit(1);
}

void *thread_function(void *arg)
{
    struct thread *retval;
    retval = (struct thread*)malloc(sizeof(retval));

    retval->val = 100;
    strcpy(retval->str, "hello thread");

    return (void*)retval;
}

int main(int argc, char *argv[])
{
    pthread_t tid;
    struct thread *retval;
    int ret;
    if ( (ret = pthread_create(&tid, NULL, thread_function, NULL)) == -1)
    {
        my_err("pthread_create error");
    }
    if ( (ret = pthread_join(tid, (void**)&retval)) == -1)
    {
        my_err("pthread_join error");
    }
    
    printf("thread exit with val = %d, str = %s\n", retval->val, retval->str);

    pthread_exit(NULL);
}
