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

void child_rw_pipe(int writefd,int readfd)
{
    char *Wmessage = "world(from child_rw_pipe)\n";
    char Rmessage[100];
    write(writefd,Wmessage,strlen(Wmessage ) + 1);
    read(readfd,Rmessage,100);
    printf("child process read from pipe : %s\n",Rmessage );
}
void parent_rw_pipe(int writefd,int readfd)
{
    char *Wmessage = "hello(from parent_rw_pipe)";
    char Rmessage[100];
    write(writefd,Wmessage,strlen(Wmessage ) + 1);
    read(readfd,Rmessage,100);
    printf("parent process read from pipe : %s\n",Rmessage );
    
}
int main(void)
{
    int stat_val;
    int pipe1[2],pipe2[2];
    pid_t pid;
    printf("readlize full_duplex communication:\n\n");
    if(pipe(pipe1))
    {
        printf("pipe failed\n");
        exit(1);

    }
    if(pipe(pipe2))
    {
        printf("pipe2 failed\n");
        exit(1);
    }
    pid = fork();
    switch(pid)
    {
        case -1 : 
            printf("fork failed\n");
            exit(0);
        case 0 :
            close(pipe2[0]);
            close(pipe1[1]);
            child_rw_pipe(pipe2[1],pipe1[0]);
            exit(0);
        default :
            close(pipe1[0]);
            close(pipe2[1]);
            parent_rw_pipe(pipe1[1],pipe2[0]);
            wait(&stat_val);
            exit(0);
    }

}
