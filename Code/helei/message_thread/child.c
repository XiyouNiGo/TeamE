/*************************************************************************
	> File Name: pipe.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月01日 星期三 08时56分43秒
 ************************************************************************/
//全双工通信，两个管道
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<wait.h>
#include<sys/stat.h>
#include<fcntl.h>


#define FIFO_NAME1 "myfifo1"
#define FIFO_NAME2 "myfifo2"
#define BUF_SIZE 1024

int main(void)
{
    int fd;
    char * Wmessage = "The message from child thread\n";
    char Rmessage[BUF_SIZE];

    
    
    if((fd = open(FIFO_NAME1,O_RDONLY) ) == -1)
    {
        perror("openfifo1 failed!\n");
        exit(2);
    }
    read(fd,Rmessage,BUF_SIZE);
    printf("I am child thread : %s",Rmessage);
    close(fd);




    umask(0);
    if(mkfifo(FIFO_NAME2,S_IFIFO | 0666) == -1)
    {
        perror("mkfifo2 failed!\n");
        exit(1);
    }
    if(( fd = open(FIFO_NAME2,O_WRONLY) ) == -1)
    {
        perror("openfifo2 failed!\n");
        exit(1);
    }
    write(fd,Wmessage,strlen(Wmessage) + 1);
    close(fd);


}

