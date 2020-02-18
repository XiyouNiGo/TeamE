/*************************************************************************
    > File Name: 1.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月18日 星期二 10时31分23秒
 ************************************************************************/

#include<stdio.h>
#define BUFSIZE 256
char * inputnch(char * arr, int n);

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
		if ((ch = getchar()) == EOF || ch == '#')
		{
			fprintf(stderr, "\nInput teminated!\n");
			return NULL;
		}

		arr[i] = ch;
	}

	arr[n] = '\0';
	return arr;
}
