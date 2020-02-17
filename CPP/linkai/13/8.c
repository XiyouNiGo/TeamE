/*************************************************************************
    > File Name: 8.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月15日 星期六 22时22分24秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include"conio.h"
int get_times(FILE * fp, char target);

int main(int argc, char *argv[])
{
    FILE * fp;
	char ch;
	char target = argv[1][0];
	int times;

	if (argc < 2)
	{
		fprintf(stderr, "Usage: %s ch filename ...\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	if (argc == 2)
	{
		puts("Please enter the string\n");
		while ((ch = getchar()) != '\n')
		{
			if (ch == target)
				times++;
		}
		printf("The character %c appears %d times in stdin\n", target, times);
	}
	else
	{
		for (int i = 2; i < argc; i++)
		{
			if ((fp = fopen(argv[i], "r")) != NULL)
			{
				fprintf(stderr, "Opening %s failed\n", argv[i]);
				continue;
			}
			times = get_times(fp, target);
			printf("The character %c appears %d times in %s\n", target, times, argv[i]);
		}
	}

	puts("Done!\n");
	return 0;
}

int get_times(FILE * fp, char target)
{
	int times = 0;
	char ch;

	while ((ch = getc(fp)) != EOF)
	{
		if (ch == target)
			times++;
	}

	return times;
}

