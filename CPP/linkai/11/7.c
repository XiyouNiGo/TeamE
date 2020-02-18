/*************************************************************************
    > File Name: 7.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月18日 星期二 15时00分56秒
 ************************************************************************/
//怎么又是段错误
#include<stdio.h>
#include<string.h>
#include"s_gets.h"
#define STRSIZE 256
char *mystrncpy(char *str1, char *str2, int n);

int main()
{
	char str1[STRSIZE];
	char str2[STRSIZE];
	int n;

	puts("Enter the source string");
	s_gets(str2, STRSIZE);
	puts("Enter the target string");
	s_gets(str1, STRSIZE);
	puts("Enter the max number you wanna copy");
	scanf("%d", &n);

    printf("The source string is %s\n", str2);
	printf("The target string is %s\n", str1);
	mystrncpy(str1, str2, n);
	printf("The string after copying is %s", str1);

	return 0;
}

char *mystrncpy(char *str1, char *str2, int n)
{
    int len1 = strlen(str1);
	int len2 = strlen(str2);
	int maxlen = (len1 > len2 ? len1 : len2);
	int max = (maxlen > n ? maxlen : n);


	for (int i = 0; i < max; i++)
	{
		str1[i] = str2[i];
	}
	if (maxlen < n)
		str1[maxlen] = '\0';

	return str1;
}
