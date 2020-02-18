/*************************************************************************
    > File Name: 16.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月18日 星期二 21时21分12秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include"s_gets.h"
#define STRSIZE 256

int main(int argc, char *argv[])
{
	char str[STRSIZE];
	int len;

	puts("Enter the string you wanna dispose");
	if (s_gets(str, STRSIZE) == NULL)
	{
		fprintf(stderr, "Writing data failed\n");
		exit(EXIT_FAILURE);
	}
	puts("The string after disposed is:");
	len = strlen(str);
	if (strcmp(argv[1], "-p") == 0 || argc == 1)
		puts(str);
	if (strcmp(argv[1], "-u") == 0)
	{
		for (int i = 0; i < len; i++)
			putchar(toupper(str[i]));
		putchar('\n');
	}
	if (strcmp(argv[1], "-l") == 0)
	{
		for (int i = 0; i < len; i++)
			putchar(tolower(str[i]));
		putchar('\n');
	}
	return 0;
}
