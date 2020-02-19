/*************************************************************************
    > File Name: 3.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月18日 星期二 22时15分22秒
 ************************************************************************/

#include<stdio.h>
#include<ctype.h>
int main()
{
    char ch = 0;
    int upper = 0;
    int lower = 0;

    while ((ch = getchar()) != EOF)
    {
        if (islower(ch))
        {
            lower++;
        }
        else if (isupper(ch))
        {
            upper++;
        }
    }
    printf("There are %d upper characters and %d lower charecters.\n", upper, lower);

    return 0; 
}
