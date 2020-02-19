/*************************************************************************
    > File Name: 2.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月19日 星期三 21时24分48秒
 ************************************************************************/

#include<stdio.h>

int main()
{
    char ch;
    int count = 0;
    while ((ch = getchar()) != EOF)
	{
        if (ch < ' ')
        {
            if (ch == '\t')
                printf("\\t : %d ", ch);
            else if (ch == '\n')
                printf("\\n : %d ", ch);
            else
                printf("^%c : %d ", ch+64, ch);
        }
        else
            printf("%c : %d ", ch, ch);
        if (++count % 10 == 0)
        {
            printf("\n");
        }
    }

	return 0;
}
