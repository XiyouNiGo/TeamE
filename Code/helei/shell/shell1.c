/*************************************************************************
	> File Name: myshell.c
	> Author: 
	> Mail: 
	> Created Time: 2020年05月01日 星期五 22时19分25秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<signal.h>
//屏蔽ctrl + c 终止进程
#include<dirent.h>


#define normal         0
#define out_redirect   1
#define in_redirect    2
#define have_pipe      3

int main(void)
{
    int fd ;
    char *arg[] = { "wc","-l" ,NULL};
    fd = open("file.txt",O_RDONLY);
    dup2(fd,0);
    execvp(arg[0],arg);
}
