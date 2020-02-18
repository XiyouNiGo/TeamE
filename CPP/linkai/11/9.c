/*************************************************************************
    > File Name: 9.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月18日 星期二 16时17分46秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#define STRSIZE 256
void Reverse(char * str);

int main()
{
	char str[STRSIZE];

	puts("Enter the string you wanna reverse");
	scanf("%s", str);
	printf("The string before reversed is %s\n", str);
	Reverse(str);
	printf("The string after reversed is %s\n", str);

	puts("Done!\n");
	return 0;
}

void Reverse(char * str)
{
	size_t len = strlen(str);
	char ch;

	for (size_t i = 0; i <= len/2-1; i++)
	{
		ch = str[i];
		str[i] = str[len-1-i];
		str[len-1-i]=ch;
	}

	return;
}
