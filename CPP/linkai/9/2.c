/*************************************************************************
    > File Name: 2.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月20日 星期四 11时08分49秒
 ************************************************************************/
#include<stdio.h>
void chline(char ch, int i, int j);

int main()
{
	char ch;
	int i, j;

	puts("Enter the character you wanna print");
	scanf("%c", &ch);
	puts("Enter the rows you wanna print");
	scanf("%d", &i);
	puts("Enter the col you wanna print");
	scanf("%d", &j);
	chline(ch, i, j);

	return 0;
}

void chline(char ch, int i, int j)
{
	int col, row;
	for (row = 0; row < i ; row++)
	{
		for (col = 0; col < j; col++)
			putchar(ch);
		putchar('\n');
	}

	return;
}
