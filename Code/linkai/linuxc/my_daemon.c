
/*************************************************************************
    > File Name: my_daemon.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年04月23日 星期四 12时04分44秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <syslog.h>
#include <sys/param.h>
#include <time.h>
#include <sys/stat.h>

void my_err(const char *str)
{
     perror(str);
     exit(1);
}

int init_daemon(void)
{
    pid_t pid;
    int i;

    //*忽略终端I/O信号
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    //signal(SIGHUP, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);   //防止产生僵尸进程
    
    if ((pid = fork()) == -1)
    {
        my_err("fork error");
    }
    else if (pid > 0)
    {
        //结束父进程，使子程序成为后台程序
        return -1;
    }
    else
    {
        //建立一个新的进程组，在新的进程组，子进程成为首进程，使其脱离终端
        setsid();
        //退出该进程，fork出非会话组长的子进程，让该进程无法打开新的终端
        if ((pid = fork()) == -1)
        {
            my_err("fork error");
        }
        else if (pid > 0)
        {
            return -1;
        }
        else
        {
            //关闭从父进程继承的不再需要的文件描述符
            for (i = 0; i < NOFILE; close(i++));
            //改变工作目录,使进程不再和任何文件系统联系
            chdir("/");
            //将文件屏蔽字设置为0
            umask(0);
            //模拟守护进程业务
            printf("I'm daemon\n");
            while (1);

            return 0;
        }
    }
}

int main(int argc, char *argv[])
{
    init_daemon();
    //这里我就不用什么syslog函数写日志了..直接while(1)模拟
    return 0;
}
