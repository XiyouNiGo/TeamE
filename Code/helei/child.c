/*************************************************************************
	> File Name: share_file.c
	> Author: 
	> Mail: 
	> Created Time: 2020年04月13日 星期一 11时10分21秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<sys/stat.h>
#include<fcntl.h>
int main(void)
{
    int ret = fork();
    if(ret>0)
    {
        system("./chdir");
    }
    else 
    {
        system("./chmod");
    }
}
