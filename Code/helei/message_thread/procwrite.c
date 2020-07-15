/*************************************************************************
	> File Name: procread.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月01日 星期三 10时52分27秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/types.h>
#include<string.h>

#define FIFO_NAME "myfifo"
#define BUF_SIZE 1024

int main(void)
{
    int fd;
    char buf[BUF_SIZE] = "Hello procwrite,\nI come from process named procread!";
    umask(0);
    if(mkfifo(FIFO_NAME,S_IFIFO | 0666) == -1)
    {
        perror("mkfifo error!");
        exit(1);
    }
    if(fd = open(FIFO_NAME,O_WRONLY) == -1)
    {
        perror("fopen error!");
        exit(1);
    }
    write(fd,buf,strlen(buf) + 1);
    close(fd);
    remove(FIFO_NAME);
    exit(0);
}
