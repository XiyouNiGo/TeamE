/*************************************************************************
    > File Name: 12.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月16日 星期日 16时43分46秒
 ************************************************************************/

//把含有一列600个数字，每行未一个'\n'的文件转成20列*30行的文件

#include<stdio.h>
int main()
{
	FILE * fps, * fpt;
	fps = fopen("12_need", "r");
	fpt = fopen("12_input", "w");
	int i = 0;
	char ch;

	while ((ch = getc(fps)) != EOF)
	{
		++i;
		if (i == 60)
		{
			i = 0;
			putc(ch, fpt);
		}
		else
		{
			if (ch == '\n')
				putc(' ', fpt);
			else
				putc(ch, fpt);
		}
	}

	fclose(fpt);
	fclose(fps);
	return 0;
}
