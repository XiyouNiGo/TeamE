
/*************************************************************************
    > File Name: sig_func.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年04月22日 星期三 12时10分56秒
 ************************************************************************/

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void print_set(sigset_t *set)
{
    for (int i = 0; i < 32; i++)
    {
        printf("%d", sigismember(set, i));
    }
    putchar('\n');
}
int main()
{
    sigset_t set, oldset, pending;
    
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    if (sigprocmask(SIG_BLOCK, &set, &oldset) == -1)
    {
        perror("sigprocmask error");
        exit(1);
    }

    //重复打印未决信号集
    while (1)
    {
        if (sigpending(&pending) == -1)
        {
            perror("sigpending error");
            exit(1);
        }
        print_set(&pending);
        sleep(1);
    }

    return 0;
}
