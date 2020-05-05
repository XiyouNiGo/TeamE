
/*************************************************************************
    > File Name: try1.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年05月04日 星期一 20时07分08秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>


int main ()
{
   printf("PATH : %s\n", getenv("PATH"));
   printf("HOME : %s\n", getenv("HOME"));
   printf("ROOT : %s\n", getenv("ROOT"));

   return(0);
}
