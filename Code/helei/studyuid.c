/*************************************************************************
	> File Name: studyuid.c
	> Author: 
	> Mail: 
	> Created Time: 2020年04月26日 星期日 21时58分29秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>

extern int errno;
int main()
{
    int fd;
    printf("uid study:\n");
    printf("Process's uidd = %d,euid = %d\n",getuid(),geteuid());
    if(fd = open("test.c",O_RDWR) == -1)
    {
        printf("Open failure,errno is %d : %s\n",errno,strerror(errno));
        exit(1);
    }
    else 
    {
        printf("Open successfully!\n");
    }
    close(fd);
    exit(0);
}
//对于调用setuid函数的程序要格外小心，当进程的有效用户ID即euid是root用户时，如果调用setuid函数使euid为其他非root用户，则该进程从此不具有超级用户权限了
//
