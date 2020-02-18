/*************************************************************************
    > File Name: 5.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月18日 星期二 13时47分53秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"s_gets.h"
#define ARRSIZE 256
char * mychr(char * str, char ch);

int main()
{
	char arr[ARRSIZE];
	char ch;
	char * temp = NULL;

	puts("Enter the string you wanna analyse");
	if (s_gets(arr, ARRSIZE) == NULL)
	{
		fprintf(stderr, "Input failed\n");
		exit(EXIT_FAILURE);
	}
	puts("Enter the character you wanna search (# to quit)");
	while (scanf("%c%*c", &ch) == 1 && ch != '#')
	{
		if (temp = mychr(arr, ch))
			printf("The address of %c is %p\n", ch, temp);
		else
			printf("Can't find %c\n", ch);
	puts("Enter the next character you wanna search (# to quit)");
	}

	puts("Done!\n");
	return 0;
}

char * mychr(char * str, char ch)
{
	char * p = str;

	while (*p != ch && *p)
		p++;
	if (*p)
		return p;
	else
		return NULL;
}
