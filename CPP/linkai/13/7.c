/*************************************************************************
    > File Name: 7.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月15日 星期六 21时00分16秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include"s_gets.h"
#define NAMSIZE 40
#define BUFSIZE 256
int is_mode_a(void);
int main()
{
	FILE * fp1, * fp2;
	char filename1[NAMSIZE];
	char filename2[NAMSIZE];
	char buf1[BUFSIZE] = { 0 };
	char buf2[BUFSIZE] = { 0 };
	int mode = is_mode_a();
	printf("Please enter filename1:\n");
	s_gets(filename1, BUFSIZE);
	printf("Please enter filename2:\n");
	s_gets(filename2, BUFSIZE);
	if ((fp1 = fopen(filename1, "r")) == NULL)
	{
		fprintf(stderr, "Opening %s failed\n", filename1);
		exit(EXIT_FAILURE);
	}
	if ((fp2 = fopen(filename2, "r")) == NULL)
	{
		fprintf(stderr, "Opening %s failed\n", filename2);
		exit(EXIT_FAILURE);
	}
	while ((feof(filename1) == 0) || (feof(filename2) == 0))
    {
        if (fgets(buf1, BUFSIZE, filename1) != NULL)
        {
            if (! mode)
			{
				int length = strlen(buf1);
                if (buf1[length - 1] == '\n')
                {
                    buf1[length - 1] = '\0';
                }
			}
            fputs(buf1, stdout);
        }
        if (fgets(buf2, BUFSIZE, filename2) != NULL)
        {
            fputs(buf2, stdout);
        }
    }

    return 0;
}

int is_mode_a()
{
	puts("Please choose your mode :\n");
	int ret = 0;
	char ch;
	if((ch = getche()) == 'a')
		ret = 1;
	while (getch() != '\n')
		continue;
	return ret;
}
