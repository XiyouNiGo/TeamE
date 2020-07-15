/*************************************************************************
	> File Name: my_signal.c
	> Author: 
	> Mail: 
	> Created Time: 2020年06月29日 星期一 11时17分31秒
 ************************************************************************/

#include<stdio.h>
#include<signal.h>
void signal_fun1(int signo)
{
    printf("\nrecv sigint signal signo= %d\n",signo);
}
void signal_fun2(int signo)
{
    printf("\nrecv sigquit signal signal = %d\n",signo);
}
int main(void)
{
    //signal(SIGQUIT,signal_fun2);
    //signal(SIGINT,signal_fun1);
    //signal函数的信号捕捉函数（signal_fun1）需要接受要得到的信号编号
    //存放在signo中，
    // sigint 信号编号为2 sigquit 信号编号为3

    //signal(SIGQUIT,SIG_IGN);
    //signal(SIGINT,SIG_IGN);
    //忽略掉ctrl+c 和ctrl+\信号进程无法终止，可以使用ps命令，查看进程
    //利用pkill a.out杀死线程
    


    //signal(SIGQUIT,SIG_DFL);
    //SIG_DFl函数会恢复SIGQUIT 原有的功能


    signal(SIGINT,SIG_DFL);
    signal(SIGINT,SIG_IGN);
    signal(SIGINT,signal_fun1);
    //一个信号做多次的处理，无论顺序如何，都会冲掉前几个操作，起作用的就是最后一次的调用，即使去忽略掉了SIGINT信号，但是最后还是成功调用了signal_fun1函数

    while(1);
}



