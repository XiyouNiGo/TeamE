/*************************************************************************
    > File Name: 登录系统.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月21日 星期五 20时02分33秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#define SIZE 50

char get_choice();
long get_tele();
char get_first();
char getch();
char * s_gets(char * st, int n);

void create_user_file();
void register_user();
int login();
void find_pwd();
typedef struct user
{
	char id[SIZE];
	char pwd[SIZE];
	long tele;
}user;

int main()
{
	char choice;
	int is_register = 0;

	create_user_file();
	while ((choice = get_choice()) != 'q')
	{
		switch(choice)
		{
			case 'a': register_user();
					  break;
			case 'b': is_register = login();
					  break;
			case 'c': find_pwd();
					  break;
		}
		if (is_register)
		{
			is_register = 0;
			system("clear");
			for (int i =0; i < 40;i++)
				putchar('*');
			putchar('\n');
			puts("\n登陆成功！\n");
			for (int i =0; i < 40;i++)
				putchar('*');
			putchar('\n');
			system("sleep 2s");
			break;
		}
	}
	puts("请按任意键继续...");
	getch();
	puts("欢迎下次使用！");

	return 0;
}

void create_user_file()
{
	FILE * fp;

	if ((fp = fopen("users", "r")) == NULL)
	{
		if ((fp = fopen("users", "w+")) == NULL)
		{
			fprintf(stderr, "无法打开用户信息!\n");
			system("sleep 3s");
			return;
		}
	}
	fclose(fp);
	return;
}

char get_choice(void)
{
	char ch;

	system("clear");
	for (int i =0; i < 40;i++)
		putchar('*');
	putchar('\n');
	putchar('\n');
	puts("欢迎使用Linux版登录系统!\n");
	for (int i =0; i < 40;i++)
		putchar('*');
	putchar('\n');
	putchar('\n');
    printf("%-20s%-20s\n%-20s%-20s\n\n", "a. 注册账号", "b. 登录账号", "c. 找回密码", "q. 退出登录");
	for (int i =0; i < 40;i++)
		putchar('*');
	putchar('\n');
	ch =get_first();
	while(ch != 'a' && ch != 'b' && ch != 'c' && ch != 'q')
	{
		puts("请输入正确选项");
		ch =get_first();
	}

	return ch;
}

char get_first(void)
{
    char ch = 0;

    while (!isalpha(ch = getchar()))
        continue;
	while (getchar() != '\n')
		continue;

    return tolower(ch);
}

void register_user()
{
	FILE * fp;
	user new_, old;
	char temp[SIZE];

	system("clear");
	for (int i =0; i < 40;i++)
		putchar('*');
	putchar('\n');
	puts("\n请开始注册您的账号！\n");
	for (int i =0; i < 40;i++)
		putchar('*');
	putchar('\n');
	if ((fp = fopen("users", "r")) == NULL)
	{
		fprintf(stderr, "读取用户信息失败！\n");
		system("sleep 3s");
		return;
	}
	puts("请输入您的账号：");
	s_gets(new_.id, SIZE);
	while (!feof(fp))
	{
		fread(&old, sizeof(user), 1, fp);
		if (!strcmp(new_.id, old.id))
		{
			fprintf(stderr, "账号重复！\n");
			system("sleep 3s");
			return;
		}
	}
	puts("请输入您的密码：");
	s_gets(new_.pwd, SIZE);
	puts("请再次输入您的密码：");
	s_gets(temp, SIZE);
	while (1)
	{
		if (!strcmp(new_.pwd, temp))
			break;
		puts("两次密码不一致！");
		puts("请重新输入您的密码：");
		s_gets(new_.pwd, SIZE);
		puts("请再次输入您的密码：");
		s_gets(temp, SIZE);
	}
	puts("请输入您的电话号码以便找回密码：");
	new_.tele = get_tele();
	fp = fopen("users", "a");
	fwrite(&new_, sizeof(user), 1, fp);
	printf("账号%s已注册成功！\n", new_.id);

	fclose(fp);
	return;
}

long get_tele()
{
	long input;
	char ch;

	while (scanf("%ld", &input) != 1)
	{
		while ((ch =getchar()) != '\n')
			putchar(ch);
		printf(" 不是一个电话号码！\n请您重新输入：\n");
	}

	return input;
}

int login()
{
	FILE * fp;
	user old;
	char temp[SIZE];

	system("clear");
	for (int i =0; i < 40;i++)
		putchar('*');
	putchar('\n');
	puts("\n请开始登录您的账号！\n");
	for (int i =0; i < 40;i++)
		putchar('*');
	putchar('\n');

	if ((fp = fopen("users", "r")) == NULL)
	{
		fprintf(stderr, "读取用户信息失败！\n");
		return 0;
	}
	puts("请输入您的账号：");
	s_gets(temp, SIZE);
	while (!feof(fp))
	{
		fread(&old, sizeof(user), 1, fp);
		if (!strcmp(temp, old.id))
		{
			puts("请输入您的密码：");
			s_gets(temp, SIZE);
			if (!strcmp(temp, old.pwd))
			{
				printf("账户%s登录成功\n", old.pwd);
				return 1;
			}
			else
			{
				puts("密码不匹配！");
				system("sleep 3s");
				return 0;
			}
		}
	}
	printf("账号%s不存在！\n", temp);
	system("sleep 3s");

	fclose(fp);
	return 0;
}

void find_pwd()
{
	FILE * fp;
	user old;
	char temp[SIZE];
	long key;

	system("clear");
	for (int i =0; i < 40;i++)
		putchar('*');
	putchar('\n');
	puts("请开始找回您的密码！\n");
	for (int i =0; i < 40;i++)
		putchar('*');
	putchar('\n');

	if ((fp = fopen("users", "r")) == NULL)
	{
		fprintf(stderr, "读取用户信息失败！\n");
		return;
	}

	puts("请输入您的账号：");
	s_gets(temp, SIZE);
	while (!feof(fp))
	{
		fread(&old, sizeof(user), 1, fp);
		if (!strcmp(temp, old.id))
		{
			puts("请输入您的电话号码：");
			if ((key = get_tele()) == old.tele)
				printf("您的密码是：%s\n", old.pwd);
			else
				puts("电话号码不匹配！");
			system("sleep 3s");
			return;
		}
	}
	printf("账号%s不存在！\n", temp);
	system("sleep 3s");

	fclose(fp);
	return;
}

char * s_gets(char * st, int n)
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

char getch()
{
	char c;
    system("stty -echo");
    system("stty -icanon");
    c=getchar();
    system("stty icanon");
    system("stty echo");
    return c;
}
