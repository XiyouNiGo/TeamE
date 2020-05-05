/*************************************************************************
	> File Name: getpid.c
	> Author: 
	> Mail: 
	> Created Time: 2020年04月26日 星期日 21时52分34秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>

int main(void)
{
    pid_t pid;
    if((pid = fork()) == -1)
    {
        printf("fork error!\n");
        exit(1);
    }
    if(pid == 0)
    {
        printf("getpid return %d\n",getpid());
    }
    exit(0);
}
