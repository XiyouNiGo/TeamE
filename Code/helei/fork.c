/*************************************************************************
	> File Name: gte_ids.c
	> Author: 
	> Mail: 
	> Created Time: 2020年04月08日 星期三 08时52分16秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
int main(void)
{
    pid_t pid;
    pid = getpid();
   // printf("pid = %u\n",pid);
    int ret;
    ret = fork();
    printf("%d , ret = %d\n",getpid(),ret);    
    while(1);
}
//使用fork函数，复制出子进程，被复制的进程被称为父进程
//复制后的结果：
//1、依照父进程的内存样子，原样开辟出子进程的内存空间
//2、子进程的代码数据和父进程完全相同
//父进程的fork，成功返回子进程的pid，失败返回-1，errno被设置
//子进程的fork，成功返回0，失败返回-1，errno被设置
