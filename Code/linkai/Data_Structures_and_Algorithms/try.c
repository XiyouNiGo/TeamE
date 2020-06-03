
/*************************************************************************
    > File Name: try.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年06月03日 星期三 12时06分42秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
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
    int x = 0;
    memset(&x, -1, sizeof(x));
    printf("%d", x);
}
