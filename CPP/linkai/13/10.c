/*************************************************************************
    > File Name: 10.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月16日 星期日 15时47分30秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#define NAMSIZE 41
#define BUFSIZE 256

int main()
{
	FILE * fp;
	char filename[NAMSIZE];
	char buf[BUFSIZE];
	long location;

	puts("Please enter the filename.");
	scanf("%s", filename);
	if ((fp = fopen(filename, "r")) != NULL)
	{
		fprintf(stderr, "Opening %s failed\n", filename);
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		puts("Please enter the location you need to seek.");
		puts("(negative number to quit)");
		if (fscanf(stdin, "%ld", &location) != 1 || location < 0)
			break;
		while (getchar() != '\n')
			continue;
		fseek(fp, location, SEEK_SET);
		fgets(buf, BUFSIZE, fp);
		fputs(buf, stdout);
	}

	fclose(fp);
	return 0;
}

