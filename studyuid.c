
/*************************************************************************
    > File Name: studyuid.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年04月24日 星期五 22时39分13秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

void my_err(const char *str)
{
     perror(str);
     exit(1);
}

int main(int argc, char *argv[])
{
    int fd;

    printf("uid study: \n");
    printf("Process's uid = %d, euid = %d\n", getuid(), geteuid());

    if ((fd = open("test.c", O_RDWR)) == -1)
    {
        printf("Open failure, errno is %d: %s\n", errno, strerror(errno));
        exit(1);
    }
    else
    {
        printf("Open successfully!\n");
    }

    close(fd);
    exit(0);
}
