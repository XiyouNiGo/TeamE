/*************************************************************************
	> File Name: monitor.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月01日 星期三 09时55分20秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<wait.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
//子进程不是外面的程序，就可以与父进程共享文件描述符，但是如果子进程调用exec函数执行另一个应用程序时，就不再共享了。
//由于管道是保存在内存中的一个文件，所以父进程去向文件写内容，子进程由于无法共享文件描述符，需要将键盘输入（标准输入）改为从文件获取输入
int main(int arg,char * argv[],char ** environ)
{
    int fd[2];
    pid_t pid;
    int stat_val;
    if(arg<2)
    {
        printf("wrong parameters \n");
        exit(0);
    }
    if(pipe(fd))
    {
        perror("pipe failed");
        exit(1);
    }

    pid = fork();
    switch(pid)
    {
        case -1:
            perror("fork failed\n");
            exit(1);
        case 0:
            close(0);
            dup(fd[0]);
            execve("ctrlprocess",(void *)argv,environ);
            exit(0);
        default:
            close(fd[0]);
            write(fd[1],argv[1],strlen(argv[1]));
            break;
    }
    wait(&stat_val);
    exit(0);
}
