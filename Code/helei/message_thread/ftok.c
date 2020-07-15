/*************************************************************************
	> File Name: ftok.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月10日 星期五 08时55分48秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>

int main(void)
{
    for(int i = 0;i<5;i++)
    {
        printf("key[%d] = %lu \n",i,ftok(".",i));
    }
    while(1);
}
