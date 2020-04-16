#ifndef _PRE_H_
#define _PRE_H_

#include<string.h>

int max_val(int a, int b)
{
	return a > b ? a : b;
}

int min_val(int a, int b)
{
	return a < b ? a : b;
}

char *s_gets(char *str, int n)
{
	int i = 0;
	char *ret_val;

	if (ret_val = fgets(str, n, stdin))
	{
		while (str[i] != '\0' && str[i] != '\n')
			i++;
		if (str[i] == '\n')
			str[i] = '\0';
		else
			while (getchar() != '\n')
				continue;
	}

	return ret_val;
}

#endif
