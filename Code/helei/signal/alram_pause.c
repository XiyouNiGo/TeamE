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
//alarm : 返回上一次调用alarm时所设置时间的剩余值，如果之前没有调用过alarm，又或者之前调用alarm所设置的时间早就到了，那么返回的剩余值为0
//pause：
//只要一直处于休眠状态，表示pause函数一直是调用成功的
//当贝信号唤醒后返回-1，表示失败了，errno的错误号被设置EINTR（表示函数被信号中断）
void signal_fun(int signo)
{
    printf("time up\n");
}
int main(void)
{
    unsigned int ret ;
    signal(SIGALRM,signal_fun);
    ret = alarm(5);
    sleep(2);
    ret = alarm(6);
    printf("ret = %d \n",ret);
    //ret 的值为3是因为调用alarm开始计时，休眠两秒就会运行下面的语句，所以ret = 3
    while(1);
}
