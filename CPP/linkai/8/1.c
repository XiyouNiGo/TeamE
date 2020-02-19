/*************************************************************************
    > File Name: 1.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月18日 星期二 22时09分30秒
 ************************************************************************/

#include<stdio.h>
int main()
{
    char ch = 0;
    int count = 0;

    while ((ch = getchar()) != EOF)
    {
        count++;
    }

    printf("There are %d characters in the file!\n", n_char);

    return 0; 
}
