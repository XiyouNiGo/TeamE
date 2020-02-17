/*************************************************************************
    > File Name: 4.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月14日 星期五 13时15分05秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
int main(int argc, char *argv[])
{
	FILE *fp;
	char buf[256] = { 0 };

	for (int i = 1; i<argc; i++)
	{
		if ((fp = fopen(argv[i], "r")) == NULL)
		{
			printf("Opening %s failed\n", argv[i]);
		}

		/*打开成功则向stdout输出文件内容*/
		printf("The contents of %s is :\n\n", argv[i]);

		while (fgets(buf, 256, fp) != NULL)
		{
			fputs(buf, stdout);
		}

		printf("\n");
		fclose(fp);
	}

	printf("\nDone!\n");
	exit(1);
}
