/*************************************************************************
    > File Name: 6.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月18日 星期二 14时51分47秒
 ************************************************************************/

int is_within(char ch, char *str)
{
	int len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        if (ch == str[i])
			return 1;
    }

    return 0;
}
