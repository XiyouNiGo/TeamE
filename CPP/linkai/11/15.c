/*************************************************************************
    > File Name: 15.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月18日 星期二 21时02分23秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include"s_gets.h"
#define STRSIZE 256
int myatoi(char * str);

int main()
{
	char str[STRSIZE];
	int result;

	puts("Enter an integer");
	if (s_gets(str, STRSIZE) == NULL)
	{
		fprintf(stderr, "Writing data failed\n");
		exit(EXIT_FAILURE);
	}
	if (result = myatoi(str) == 0)
	{
		fprintf(stderr, "Invalid input!\n");
		exit(EXIT_FAILURE);
	}
	printf("The integer you key in is %d", myatoi(str));

	return 0;
}

int myatoi(char * str)
{
	int len = strlen(str);
	int result = 0;

	for (int i = 0; i < len; i++)
	{
		if (isdigit(str[i]) == 0)
			return 0;
		else
			result = result * 10 + str[i] - '0';
	}

	return result;
}
