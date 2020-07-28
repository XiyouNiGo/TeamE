#include <stdio.h>
#include <stdlib.h>
#include "input.h"

char getch()
{
	char ch;

    system("stty -echo");
    system("stty -icanon");
    ch = getchar();
    system("stty icanon");
    system("stty echo");

    return ch;
}

char * s_gets(char *st, int n)
{
	char * ret_val;
	int i = 0;

	ret_val = fgets(st, n, stdin);
	if (ret_val != NULL)
	{
		while (st[i] != '\0' && st[i] != '\n')
			i++;
		if (st[i] == '\n')
			st[i] = '\0';
		else
			while (getchar() != '\n')
				continue;
	}
	return ret_val;
}

void secret_pwd(char *pwd)
{
	char ch;
	int i = 0;

	while ((ch = getch()) != '\n')
	{
		if (ch == '\x7F')
		{
			putchar('\b');
			putchar(' ');
			putchar('\b');
			if (i)
				i--;
			else
				putchar('\a');
		}
		else
		{
			putchar('*');
			pwd[i++] = ch;
		}
	}
	pwd[i] = '\0';
	putchar('\n');

	return;
}
