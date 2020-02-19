/*************************************************************************
    > File Name: 6.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月19日 星期三 10时43分38秒
 ************************************************************************/

#include<stdio.h>
#include<ctype.h>
char get_first(void);

int main()
{
	puts("Enter the character");
	char ch = get_first();
	printf("The character you key in is %c\n", ch);

	return 0;
}

char get_first()
{
	char ch;

	while(isalpha(ch = getchar()) == 0)
		continue;

	return ch;
}
