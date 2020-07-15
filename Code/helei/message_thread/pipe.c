/*************************************************************************
	> File Name: pipe.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月01日 星期三 08时56分43秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<wait.h>

void read_from_pipe(int fd)
{
    char message[100];
    read(fd,message,100);
    printf("read from pipe : %s ",message);
}
void write_to_pipe(int fd)
{
    char *message = "Hello Pipe\n";
    write(fd,message,strlen(message) +  1);
}
int main(void)
{
    int fd[2];
    pid_t pid;
    int stat_val;
    if(pipe(fd))
    {
        printf("Create pipe failed\n");
        exit(1);
    }
    pid = fork();
    switch(pid)
    {
        case -1:
            printf("fork failed\n");
            exit(1);
        case 0:
        //关闭写端
            close(fd[1]);
            read_from_pipe(fd[0]);
            exit(0);
        default:
        //关闭读端
            close(fd[0]);
            write_to_pipe(fd[1]);
        //父进程一旦调用了wait就立即阻塞自己，由wait自动分析是否当前进程的某个子进程已经退出，如果让它找到了这样一个已经变成僵尸的子进程，wait就会收集这个子进程的信息，并把它彻底销毁后返回；如果没有找到这样一个子进程，wait就会一直阻塞在这里，直到有一个出现为止。
        //
        //
            wait(&stat_val);
            exit(0);
    }
}
