/*************************************************************************
    > File Name: 1.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月13日 星期四 15时13分58秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
int main(int argc, char * argv[])
{
	int ch;
    FILE *fp;
    unsigned long count = 0;
    char filename[256];

    printf("Please enter the filename: ");
    scanf("%s", filename);

    if ((fp = fopen(filename, "r")) == NULL) 
    {                                   
        printf("Can't not open %s\n", filename);
    }
    while ((ch = getc(fp)) != EOF)
    {
        putc(ch, stdout);
        count++;
    }
    fclose(fp);
    printf("File %s has %lu characters\n", filename, count);

	exit(0);
}
