/*************************************************************************
    > File Name: 8.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月18日 星期二 15时46分35秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#define STRSIZE 256
char * string_in(char * str1, char * str2);

int main()
{
	char str1[STRSIZE];
	char str2[STRSIZE];
	char * p = NULL;

	printf("Enter the string you wanna analyse\n");
	scanf("%s", str1);
	printf("Enter the string you wanna find\n");
	scanf("%s", str2);
	if(p = string_in(str1, str2))
	{
		printf("The address of %s is %p\n", str2, p);
	}
	else
		printf("%s is not included in %s\n", str2, str1);

	return 0;
}

char * string_in(char * str1, char * str2)
{
	size_t len1 = strlen(str1);
	size_t len2 = strlen(str2);
	
	for (size_t i = 0; i < len1; i++)
	{
		size_t n = i;
		size_t m = 0;
		while (str1[n++] == str2[m++])
		{
			if (m == len2)
				return &str1[i];
		}
	}

	return NULL;
}
