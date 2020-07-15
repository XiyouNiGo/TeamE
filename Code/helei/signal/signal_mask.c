/*************************************************************************
	> File Name: my_signal.c
	> Author: 
	> Mail: 
	> Created Time: 2020年06月29日 星期一 11时17分31秒
 ************************************************************************/

#include<stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
void signal_fun1(int signo)
{
    printf("hello \n");
    //sleep(2);
    //printf("world\n");
}
void signal_fun2(int signo)
{
    printf("\nrecv sigquit signal signal = %d\n",signo);
}
int main(void)
{
    signal(SIGINT,signal_fun1);

    while(1);
}

//信号发生源发送ctrl+c 来查看信号处理方式，是递送还是不处理
//若为递送，则传给信号屏蔽字，递送成功，存放该位的信号屏蔽字设为1
//捕获函数调用完，会处理信号屏蔽字的该位为0
//当该位的信号屏蔽字仍为1时（信号捕获函数还没有执行完毕），则意味递送失败，该信号作为未决信号被设置到未处理信号集中，
//当该位的信号屏蔽字仍为1时，无论你传递多少信号，都只会存放一个信号到未处理信号集，没有排队机制

