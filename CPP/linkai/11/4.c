/*************************************************************************
    > File Name: 4.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月18日 星期二 13时41分00秒
 ************************************************************************/

#include<stdio.h>
#define ARRSIZE 256
int getword(char * arr, int n);
int isBlank(char ch);

int main()
{
	char arr[ARRSIZE];
	int n;

	puts("Please enter the number you wanna input");
	scanf("%d%*c", &n);
	if (getword(arr, n))
		puts("Done\n");
	else
		puts("Word-getting failed");

	 return 0;
}

int getword(char * arr, int n)
{
	char ch;
	char *p = arr;
	int count = 1;

	while ((ch = getchar()) != EOF && isBlank(ch))
		continue;
	if (ch == EOF)
		return 0;
	*p++ = ch;
	while ((ch = getchar()) != EOF && isBlank(ch) == 0)
	{
		*p++ = ch;
		count++;
		if (count == n)
			break;
	}
	*p = '\0';
	if (ch == EOF)
		return 0;

	while (getchar() != '\n')
		continue;

	return 1;
}
int isBlank(char ch)
{
	int ret_val = 0;
	if (ch == '\n' || ch == ' ' || ch == '	')
		ret_val = 1;

	return ret_val;
}
