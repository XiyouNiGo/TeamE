/*************************************************************************
    > File Name: 12.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月18日 星期二 16时49分44秒
 ************************************************************************/

#include<stdio.h>
#include<ctype.h>
#include<string.h>
#define STRSIZE 256

int main()
{
    char str[STRSIZE];
    size_t len;
    size_t word = 0;
    size_t upper = 0;
    size_t lower = 0;
    size_t punch = 0;
    size_t num = 0;

    printf("Enter the string you wanna analyse\n");
    scanf("%s", str);
    len = strlen(str);
    for (size_t i = 0; i <= len; i++)
    {
        if (isspace(str[i]) || str[i] == '\0')
        {
            word++;
        }
        else if (isupper(str[i]))
        {
            upper++;
        }
        else if (islower(str[i]))
        {
            lower++;
        }
        else if (ispunct(str[i]))
        {
            punch++;
        }
        else if (isdigit(str[i]))
        {
            num++;
        }
    }

    printf("The string contains %zd word, %zd upper character, %zd lower character, %zd punchation, %zd digital!\n", word,
            upper, lower, punch, num);

    return 0;
}
