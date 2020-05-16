
/*************************************************************************
    > File Name: pthread_end.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年05月13日 星期三 10时50分27秒
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

void *function1(void *arg)
{
    printf("thread 1 return\n");
    return (void*)111;
}

void *function2(void *arg)
{
    printf("thread 2 return\n");
    pthread_exit((void*)222);
}

void *function3(void *arg)
{
    while (1)
    {
        /*printf("thread 3 still alive\n");
        sleep(1);*/
        pthread_testcancel();   //添加取消点
    }
}
int main(int argc, char *argv[])
{
    pthread_t tid;
    void *ret = NULL;

    pthread_create(&tid, NULL, function1, NULL);
    pthread_join(tid, &ret);
    printf("thread 1 exit code = %d\n", (long)ret);

    pthread_create(&tid, NULL, function2, NULL);
    pthread_join(tid, &ret);
    printf("thread 2 exit code = %d\n", (long)ret);

    pthread_create(&tid, NULL, function3, NULL);
    sleep(3);
    pthread_cancel(tid);
    pthread_join(tid, &ret);
    printf("thread 3 exit code = %d\n", (long)ret);  //成功被pthread_cancle杀死，返回-1
}
