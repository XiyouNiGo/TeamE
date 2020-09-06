#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <readline/readline.h>
#include "../../inc/client/interface.h"
#include "../../inc/client/md5.h"

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
    char temp[64];

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
			temp[i++] = ch;
		}
	}
	temp[i] = '\0';
	putchar('\n');
    //MD5加密
	MD5Change(pwd, temp);
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
    CLEAR();
	for (int i =0; i < WIDTH;i++)
		putchar('*');
	putchar('\n');
	printf("\n%s\n\n", line);
	for (int i =0; i < WIDTH;i++)
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
//获取int类型正确选项
int get_choice_int(int min, int max)
{
	int input;
	char ch;

	while ( (scanf("%d", &input) != 1) || (input < min) || (input > max))
	{
		while ((ch =getchar()) != '\n')
            continue;
		printf("请输入正确选项：\n");
	}

    while ((ch =getchar()) != '\n')
            continue;

	return input;
}
//获取int类型的正数
int get_int()
{
	int input;
	char ch;

	while (scanf("%d%*c", &input) != 1)
	{
		while ((ch =getchar()) != '\n')
			putchar(ch);
		printf(" 不是一个正整数！\n请重新输入：\n");
	}

	return input;
}
//初始动画
void initial_animation()
{
    HIDE_CURSOR();
    BEGIN(31, 48);
    printf(" _______  .__  ________         /\\ _________ .__            __                                  \n");
    printf(" \\      \\ |__|/  _____/  ____   )/ \\_   ___ \\|  |__ _____ _/  |________  ____   ____   _____    \n");
    printf(" /   |   \\|  /   \\  ___ /  _ \\     /    \\  \\/|  |  \\\\__  \\\\   __\\_  __ \\/  _ \\ /  _ \\ /     \\   \n");
    printf("/    |    \\  \\    \\_\\  (  <_> )    \\     \\___|   Y  \\/ __ \\|  |  |  | \\(  <_> |  <_> )  Y Y  \\  \n");
    printf("\\____|__  /__|\\______  /\\____/      \\______  /___|  (____  /__|  |__|   \\____/ \\____/|__|_|  /  \n");
    printf("        \\/           \\/                    \\/     \\/     \\/                                \\/   \n");
    CLOSE();
    HIDE_CURSOR();
    sleep(2);
    printf("\n\n");
    FLASH();
    BEGIN(34, 48);
    printf("                                                                    Press any key to continue \n");
    getch();
    CLOSE();
    SHOW_CURSOR();
}
