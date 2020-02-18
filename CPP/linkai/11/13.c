/*************************************************************************
    > File Name: 13.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月18日 星期二 20时42分02秒
 ************************************************************************/

#include<stdio.h>

int main(int argc, char * argv[])
{
    for (int i = argc - 1; i > 0; i--)
    {
        printf("%s ", argv[i]);
    }

	putchar('\n');
    return 0;
}
