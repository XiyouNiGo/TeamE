
/*************************************************************************
    > File Name: mynice.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年04月24日 星期五 23时38分56秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>

void my_err(const char *str)
{
     perror(str);
     exit(1);
}

//int nice(int increment)
//{
//  int oldpro = getpriority(PRIO_PROCESS, getpid());
//  return setpriority(PRO_PROCESS, getpid(), oldpro + increment);
//}

int main(int argc, char *argv[])
{
    pid_t pid;
    int stat_val = 0;
    int oldpri, newpri;

    printf("nice study:\n");

    if ((pid = fork()) == -1)
    {
        my_err("fork error");
    }
    else if (pid == 0)
    {
        printf("Child is running, pid is %d, ppid is %d\n", getpid(), getppid());

        oldpri = getpriority(PRIO_PROCESS, 0);
        printf("Old priority is %d\n", oldpri);

        newpri = nice(2);
        printf("New priority is %d\n", newpri);
    }
    else
    {
        printf("Parent is running, child is %d, pid is %d\n", pid, getpid());
    }

    return 0;
}
