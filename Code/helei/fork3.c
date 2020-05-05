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
int globvar = 5;
int main(void)
{
    pid_t pid;
    int var = 1,i;
    printf("fork is diffirent with vfork\n");
    pid = fork();
    //pid = vfork();
    switch(pid)
    {
        case 0:
            i = 3;
        while(i-->0)
        {
            printf("Child process is running \n") ;
            globvar ++;
            var ++;
            sleep(1);
        }
        printf("Child's globvar = %d,var = %d\n",globvar,var);
            break;
        case -1:
            perror("Process creation failed\n");
            exit(0);
        default:
            i = 5;
            while(i-->0)
        {
            printf("Parent process is running\n");
            globvar ++;
            var ++;
            sleep(1);
        }
        printf("Parent's globvar = %d,var = %d\n",globvar,var);
            break;
    }
    exit(0);
}
//用fork创建子进程后，父子进程的执行顺序是不确定的，父子进程的而输出是混杂在一起的，而使用vfork创建子进程后，打印的结果为子进程在前，父进程在后，说明vfork保证子进程先执行，
//fork的子进程有自己独立的地址空间，不管是全局变量还是局部变量，子进程与父进程对他们的修改互不影响，vfork的子进程共享父进程的地址空间，子进程修改变量对父进程是可见的
