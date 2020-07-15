/*************************************************************************
	> File Name: newprocess.c
	> Author: 
	> Mail: 
	> Created Time: 2020年06月30日 星期二 00时39分18秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>
typedef void (*sighandler_t)(int);
/*void signal_fun(int signo)
{
    printf("new_pro pid : %d , signo = %d\n",getpid(),signo);
}*/
int main(void)
{
    while(1);
}
