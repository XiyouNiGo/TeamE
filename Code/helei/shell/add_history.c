/*************************************************************************
	> File Name: add_history.c
	> Author: 
	> Mail: 
	> Created Time: 2020年05月05日 星期二 09时55分00秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<readline/history.h>
int main(void)
{
    while(1)
    {
        char *str = readline("MyShell $ ");
        add_history(str);
        free(str);
    }

}
