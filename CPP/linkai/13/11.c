/*************************************************************************
    > File Name: 11.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月16日 星期日 16时16分29秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define BUFSIZE 256

int main(int argc, char *argv[])
{
	FILE * fp;
	char buf[BUFSIZE];

	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s string filename\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if ((fp = fopen(argv[2], "r")) == NULL)
	{
		fprintf(stderr, "Opening %s failed\n", argv[2]);
		exit(EXIT_FAILURE);
	}
	while (fgets(buf, BUFSIZE, fp) != NULL)
	{
		if (strstr(buf, argv[1]) != NULL)
			fputs(buf, stdout);
	}

	fclose(fp);
	return 0;
}
