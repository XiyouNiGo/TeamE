/*************************************************************************
    > File Name: 10.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月18日 星期二 17时00分07秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include"s_gets.h"
#include"stdlib.h"
#define STRSIZE 256
void Remove_space(char * str);

int main()
{
	char str[STRSIZE];

	puts("Enter the string you wanna dispose");
	if (s_gets(str, STRSIZE) == NULL)
	{
		fprintf(stderr, "Writing data faile\n");
		exit(EXIT_FAILURE);
	}
	printf("The string before disposed is %s\n", str);
	Remove_space(str);
	printf("The string after disposed is %s\n", str);

	puts("Done!\n");
	return 0;
}

void Remove_space(char * str)
{
	int len = strlen(str);
	int t, i;

	for (i = 0, t = 0; i < len; i++)
	{
		if (isspace(str[i]) == 0)
		{
			str[t++] = str[i];
		}
		else
			continue;
	}
	str[t] = '\0';

	return;
}
