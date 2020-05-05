/*************************************************************************
	> File Name: gte_ids.c
	> Author: 
	> Mail: 
	> Created Time: 2020年04月08日 星期三 08时52分16秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
//让父子进程做不同的事情
int main(void)
{
    pid_t pid;
    pid = getpid();
    int ret;
    ret = fork();
    if(ret > 0)
    {
        printf("parent PID = %d\n",getpid());
        printf("parent ret = %d\n",ret);
    }
    else
    {
        printf("child PID = %d\n",getpid());
        printf("child ret = %d\n",ret);
    }
    printf("------------------\n");    
}
