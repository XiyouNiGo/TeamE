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
    int ret;
    int fd;
    ret = fork();
    if(ret>0)
    {
        fd = open("file.txt",O_RDWR | O_CREAT);
        write(fd,"helloasdasdasd\n",strlen("helloasdasdasd\n"));
    }
    else 
    {
        
        fd = open("file.txt",O_RDWR | O_CREAT);
        write(fd,"world\n",strlen("world\n"));
        close(fd);
    }
}
