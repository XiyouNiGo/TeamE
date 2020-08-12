#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <readline/readline.h>
#include "../../inc/client/interface.h"
//getch()替代函数(非标库,linux下没有)
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
//安全的取代gets()
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
//隐藏式输入密码
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
}
//返回键入的第一个字母字符并清空缓存区
char get_first_alpha()
{
    char ch = 0;

    while (!isalpha(ch = getchar()))
        continue;
	while (getchar() != '\n')
		continue;

    return tolower(ch);	 //返回小写值
}
//模拟任意键继续
void press_to_continue()
{
	puts("\n请按任意键继续...");
	getch();
}
//展示某句话
void show_line(const char *line)
{
	system("clear");	//清屏函数
	for (int i =0; i < 40;i++)
		putchar('*');
	putchar('\n');
	printf("\n%s\n\n", line);
	for (int i =0; i < 40;i++)
		putchar('*');
	putchar('\n');
}
//获取文件名(附带补全功能)
char *get_filename(char *prompt)
{
    static char *line_read = NULL;
    if (line_read)
    {
        free (line_read);
        line_read = (char *)NULL;
    }
    return line_read = readline (prompt);
}
