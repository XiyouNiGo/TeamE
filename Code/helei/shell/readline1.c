/*************************************************************************
	> File Name: readline1.c
	> Author: 
	> Mail: 
	> Created Time: 2020年05月05日 星期二 09时44分31秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<readline/readline.h>
#define CLOSE "\033[0m"     //关闭所有属性
#define BLOD "\033[1m"      //强调，加粗，加亮
#define BEGIN(x,y) "\033["#x";"#y"m"  //x：背景   y：前景

int main(void)
{
    while(1)
    {
        char *str = readline(BEGIN(49,34)"MyShell $ "CLOSE);
        free(str);
    }
}
