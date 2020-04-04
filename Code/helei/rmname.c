/*************************************************************************
	> File Name: rmname.c
	> Author: 
	> Mail: 
	> Created Time: 2020年03月26日 星期四 09时38分42秒
 ************************************************************************/

#include<stdio.h>
#include<time.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>
int main(int argc,char *argv[])
{
    if(argc<3)
    {
        printf("my_mv<old file> <new file> \n");
        return 0;
    }
    if(rename(argv[1],argv[2])==-1)   
    //系统函数 rename(老文件名，新文件名)
    {
        perror("rename : "); 
        //如果该函数不需要提示语也必须要 “” 括号里的内容会输出
        return 0;
    }
}
