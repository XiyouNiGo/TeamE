/*************************************************************************
	> File Name: readline.c
	> Author: 
	> Mail: 
	> Created Time: 2020年05月04日 星期一 11时26分13秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<readline/history.h>

int main()
{
    while(1)
    {
        char * str = readline("MyShell $ ");
        free(str);
    }
    return 0;
}
