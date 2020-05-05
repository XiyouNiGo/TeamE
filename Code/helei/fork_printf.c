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
//利用printf函数 ，挤压数据的特点
//两个printf证明了fork复制了内存，并且复制了完整的代码段
int main(void)
{
    pid_t pid;
    pid = getpid();
    int ret;
    //printf("开始执行");        如果不带\n 会将开始执行存储在printf函数里，当再次执行printf时，遇到\n 或缓存区满会输出该句话
    //printf("开始执行\n");      带上\n 在子进程中，因为\n 让开始执行已经消失，而不是积压在printf函数里
    ret = fork();
    //printf("开始执行\n");
    //两个printf的位置不同，结果不同
    if(ret > 0)
    {
        printf("parent PID = %d\n",getpid());
        printf("parent ret = %d\n",ret);
    }
    else
    {
        printf("child PID = %d\n",getpid());
        printf("child ret = %d\n",ret);
    }
    printf("------------------\n");    
    printf("结束执行\n");
}
//子进程：
//1、fork前的代码：尽管子进程复制了这段代码，但是子进程不会执行，子进程只会从开始执行
//2、子进程调用fork时，返回值为0，注意0不是pid
//   进入if(ret == 0){  }   执行大括号里的内容
//3、执行fork后的代码
//   fork函数是子进程的起点
//
