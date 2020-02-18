/*************************************************************************
    > File Name: 2.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月18日 星期二 11时03分31秒
 ************************************************************************/

#include<stdio.h>
#define BUFSIZE 256
char * inputnch(char * arr, int n);
int isBlank(char ch);

int main()
{
	char buf[BUFSIZE];
	int n;

	puts("Please enter the number you wanna input");
	scanf("%d%*c", &n);
	if (inputnch(buf, n))
		printf("%s has succeeded in saving in arr!\n", buf);
	
	return 0;
}

char * inputnch(char * arr, int n)
{
	char ch;

	puts("Enter # to teminate input in advance\n");
	for (int i = 0; i < n; i++)
	{
		ch = getchar();
		if (ch == EOF || ch == '#' || isBlank(ch))
		{
			fprintf(stderr, "\nInput teminated!\n");
			return NULL;
		}

		arr[i] = ch;
	}

	arr[n] = '\0';
	return arr;
}

int isBlank(char ch)
{
	int ret_val = 0;
	if (ch == '\n' || ch == ' ' || ch == '	')
		ret_val = 1;
	
	return ret_val;
}
