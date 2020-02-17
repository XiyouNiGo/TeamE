/*************************************************************************
    > File Name: 2.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月13日 星期四 15时13分58秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
int main(int argc, char * argv[])
{
    FILE *fp1;
    FILE *fp2;
    int ch = 0;

	if (argc != 3)
	{
		printf("Usage: %s filename1 filename2\n",argv[0]);
		exit(EXIT_FAILURE);
	}
    if ((fp1 = fopen(argv[1], "rb")) == NULL)
    {
        printf("Can't open %s\n", argv[1]);
    }
    if ((fp2 = fopen(argv[2], "wb")) == NULL)
    {
        printf("Can't open %s\n", argv[2]);
    }
    while ((ch = fgetc(fp1)) != EOF)
    {
        fputc(ch, fp2);
	}

	exit(0);
}
