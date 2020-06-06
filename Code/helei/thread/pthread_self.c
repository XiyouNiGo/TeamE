/*************************************************************************
	> File Name: two_pth.c
	> Author: 
	> Mail: 
	> Created Time: 2020年05月07日 星期四 21时40分30秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<signal.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int main(void)
{
    printf("main tid : %lu\n",pthread_self());
    //编译时加上 -pthread 
    //pthread_self 返回值为线程tid，为*****无符号长整形*****
}
