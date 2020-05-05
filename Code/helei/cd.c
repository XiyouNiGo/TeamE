/*************************************************************************
	> File Name: cd.c
	> Author: 
	> Mail: 
	> Created Time: 2020年05月04日 星期一 08时58分41秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<linux/limits.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<dirent.h>

void my_err(const char *err_string,int line)
{
    fprintf(stderr,"line : %d ",line);
    perror(err_string);
    exit(1);
}

int main(int argc,char **argv)
{
    char buf[PATH_MAX + 1];
    int ret;
    ret = fork();
    if(fork == 0)
    {
        char *arg="cd";
        chdir(argv[1]);
       execvp(*arg,arg);
    }

}
