/*************************************************************************
	> File Name: chdir.c
	> Author: 
	> Mail: 
	> Created Time: 2020年03月26日 星期四 10时30分10秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<linux/limits.h>
int main(int argc,char *argv[])
{
    if(argc<2)
    {
        printf("my_chdir <target path>\n");
        return 0;
    }
    if(chdir(argv[1])<0)
    {
        perror("chdir fail :");
        return 0;
    }
    char *buf=getcwd(0,0);
    if(buf==NULL)
    {
        perror("getcwd fail :");
        return 0;
    }
    printf("%s\n",buf);
    return 0;
}
