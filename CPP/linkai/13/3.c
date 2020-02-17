/*************************************************************************
    > File Name: 3.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月13日 星期四 15时13分58秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
int main(int argc, char * argv[])
{
    FILE *fp1;
    FILE *fp2;
    int ch = 0;
	char filename[40];

	printf("Please enter the filename:\n");
	scanf("%s",filename);
	if ((fp1 = fopen(filename, "r")) == NULL || (fp2 = fopen("file_out", "w")) == NULL)
	{
		printf("Open failed!\n");
		exit(EXIT_FAILURE);
	}
    while ((ch = fgetc(fp1)) != EOF)
    {
		ch = toupper(ch);
        fputc(ch, fp2);
	}

	exit(0);
}
