/*************************************************************************
    > File Name: 4.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月19日 星期三 16时03分56秒
 ************************************************************************/

#include<stdio.h>
#include<ctype.h>

int main()
{
	char ch;
	float charcount = 0.0;
	int isprechar = 0;	//为0代表前一格为空格,为1则为字母
	float wordcount = 0.0;

	puts("Enter the string you wanna analyse");
	while((ch = getchar()) != EOF)
	{
		if (ispunct(ch) || isspace(ch))
			isprechar = 0;
		else
		{
			if (isprechar == 0)
				wordcount++;
			isprechar = 1;
			charcount++;
		}
	}
	printf("Total words: %.0f, Total characters: %.0f, Characters per word: %.2lf", wordcount, charcount, charcount/wordcount);

	return 0;
}
