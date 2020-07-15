/*************************************************************************
	> File Name: ctrlprocess.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月01日 星期三 09时59分27秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>

int main(int arg,char *argv[])
{
    int n;
    char buf[1024];
    while(1)
    {
        if((n = read(0,buf,1024)) > 0)
        {
            buf[n] = '\0';
            printf("ctrlprocess receive : %s\n",buf);

            if(!strcmp(buf,"exit"))
                exit(0);
            if(!strcmp(buf,"getpid"))
            {
                printf("My pid : %d\n",getpid());
                sleep(3);
                exit(0);
            }
        }
    }
}
