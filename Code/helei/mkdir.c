/*************************************************************************
	> File Name: mkdir.c
	> Author: 
	> Mail: 
	> Created Time: 2020年03月26日 星期四 10时03分33秒
 ************************************************************************/

#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<errno.h>
int main(void)
{
    umask(0);
    int t;
    char buf[100]={0};
    printf("请输入要创建目录名字：");
    scanf("%s",buf);
    t=mkdir(buf,777);
    if(t<0)
    {
        perror("mkdir fail:");
        return 0;
    }
}
