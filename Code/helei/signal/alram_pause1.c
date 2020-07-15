/*************************************************************************
	> File Name: alram_pause.c
	> Author: 
	> Mail: 
	> Created Time: 2020年06月30日 星期二 01时26分04秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<signal.h>
#include<errno.h>
void signal_fun(int signo)
{
    printf("time up\n");
}
int main(void)
{
    int ret = 0;
    signal(SIGINT,signal_fun);
    //捕获ctrl + c 会自动终止pause函数
    //需要手动重启该函数
lable:  ret = pause();
    if(ret == -1 && errno == EINTR)
    {
        goto lable;
    }
    printf("hello \n");


    //休眠函数也会被ctrl + c 异常终止
    //查看sleep和pause的返回值，看是否goto
lable1:     ret = sleep(10);
    if(ret != 0)
    {
        goto : lable1;
    }
    while(1);
}
