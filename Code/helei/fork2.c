/*************************************************************************
	> File Name: fork1.c
	> Author: 
	> Mail: 
	> Created Time: 2020年04月25日 星期六 09时50分04秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
int main(void)
{
    pid_t pid;
    char * msg;
    int k;
    printf("Process Creation Study\n");
    pid = fork();
    switch(pid)
    {
        case 0:
            msg = "Child Process is running";
            k = 3;
            break;
        case -1:
            perror("Process creation failed\n");
            break;
        default:
        msg = "Parent process is running ";
            k = 5;
            break;
    }
    while(k>0)
    {
        printf("%s\n",msg);
        //puts(msg);
        sleep(1);
        k--;
    }
    exit(0);
}
//一般来说，fork之后是父进程先执行还是子进程先执行是不确定的，这取决于内核使用的调度算法，操作系统一般让所有进程都拥有同等执行权，除非某进程的优先级比其他的高
//子进程会继承父进程的很多属性，包括用户id，组id，当前工作目录，根目录，打开的文件，创建文件时使用的屏蔽字，信号屏蔽字，上下文环境，共享的存储段，资源限制等，
//子进程有他自己唯一的进程id
//fork的返回值不同，父进程返回子进程的id，子进程的则为0
//不同的父进程id。紫禁城的父进程id为创建它的父进程id，就是说子进程完全运行在父进程的地址空间上，紫禁城对该地址空间中任何数据的修改同样为父进程缩减
//使用fork创建一个子进程时，拿给你成先运行取决于系统的调度算法，而vfork 一个进程时，vfork保证子进程先运行，当他调用exec或exit之后，父进程才可能被调度运行。如果在调用exec或exit之前子进程要依赖父进程的某个行为，就会导致死锁’
//
