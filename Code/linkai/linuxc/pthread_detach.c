
/*************************************************************************
    > File Name: pthread_detach.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年05月13日 星期三 11时32分54秒
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

int main(int argc, char *argv[])
{

}
