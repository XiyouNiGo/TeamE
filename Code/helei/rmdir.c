/*************************************************************************
	> File Name: rmdir.c
	> Author: 
	> Mail: 
	> Created Time: 2020年03月26日 星期四 10时09分25秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<errno.h>
int main(void)
{
    int t;
    char buf[100]={0};
    printf("请输入要删除的目录名：");
    scanf("%s",buf);
    t=rmdir(buf);
    if(t<0)
    {
        perror("rmdir fail");
        return 0;
    }
}
//remove 函数即可以删除目录，也可以删除文件
//unlink函数只能用于删除文件
//rmdir 函数只能用于删除目录
