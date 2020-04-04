/*************************************************************************
	> File Name: mask_ctrl_c.c
	> Author: 
	> Mail: 
	> Created Time: 2020年04月04日 星期六 00时12分33秒
 ************************************************************************/

#include<stdio.h>
#include<signal.h>
int main(void)
{
    char name[50];
    signal(SIGINT,SIG_IGN);
    while(1)
    {
        scanf("%s",name);
        printf("%s\n",name);
    }
}
