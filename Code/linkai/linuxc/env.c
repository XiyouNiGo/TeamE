
/*************************************************************************
    > File Name: env.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年04月23日 星期四 22时04分21秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

extern char **environ;

void my_err(const char *str)
{
     perror(str);
     exit(1);
}
//int main(int argc, char *argv[], char **envp)
int main(int argc, char *argv[])
{
    int i;
    printf("Argument:\n");

    for (i = 0; i < argc; i++)
    {
        printf("argv[%d] is %s\n", i, argv[i]);
    }

    printf("Environment:\n");

    for (i = 0; environ[i] != NULL; i++)
    {
        printf("%s\n", environ[i]);
    }

    return 0;
}
