#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#define SIZE 50
//用户信息主体(结构体实现)
typedef struct user
{
	char id[SIZE];		//账号
	char pwd[SIZE];		//密码
	long tele;			//电话号码
}user;
//菜品信息主体(链表实现)
typedef struct Node
{
	char name[SIZE];		//名称
	int price;				//价格
	char restaurant[SIZE];	//菜馆名字
	int star;				//星级(好吃吗？)
	char comment[SIZE];		//评价
	struct Node * next;
}Node;
//为了方便操作
typedef struct Dish
{
	char name[SIZE];
	int price;
	char restaurant[SIZE];
	int star;
	char comment[SIZE];
}Dish;
//为了安全的输入（防止暴力输入）
char get_choice();
char get_manage();
long get_tele();
int get_int();
char get_first();
char getch();
char * s_gets(char * st, int n);
void secret_pwd(char * pwd);
//具体功能实现函数
void create_user_file();
void create_dish_file();
void register_user();
void login();
void find_pwd();
void show_author();
void press_to_continue();
void show_function();
void print_star(int n);
void add_dish(Node **head);
void delete_dish(Node *head);
void search_by_name(Node *head);
void search_by_price(Node *head);
void search_by_restaurant(Node *head);
void show_dish(Node *head);
Node * init_dish();
void free_dish(Node **head);
void save_dish(Node *head);
void modify_dish(Node *head);
//排序函数
Node * find_pmax_dish(Node *head);
Node * find_pmin_dish(Node *head);
Node * find_smax_dish(Node *head);
Node * find_smin_dish(Node *head);
void sort_sdown_dish(Node *head);
void sort_pdown_dish(Node *head);
void sort_sup_dish(Node *head);
void sort_pup_dish(Node *head);
void swap_dish(Node *p1, Node *p2);
//加密解密函数
void encipher(char *original);
void decipher(char *cipher);
//菜单函数
void show_line(const char *line);
void user_menu();
void internal_menu();
void dish_manage_menu();
void search_dish_menu(Node *head);
void sort_dish_menu(Node *head);
//看上去很短的主函数
int main()
{
	user_menu();
	press_to_continue();
	puts("欢迎下次使用！");
	return 0;
}
//保证文件users的存在，该文件存在时也不清空其中的数据
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
//同上
void create_dish_file()
{
	FILE * fp;

	if ((fp = fopen("dishes", "r")) == NULL)
	{
		if ((fp = fopen("dishes", "w+")) == NULL)
		{
			fprintf(stderr, "无法打开菜品信息!\n");
			system("sleep 3s");
			return;
		}
	}
	fclose(fp);

	return;
}
//获取用户键入的信息（一级&二级菜单使用）
char get_choice(void)
{
	char ch;

	ch =get_first();
	while(ch != 'a' && ch != 'b' && ch != 'c' && ch != 'q')
	{
		puts("请输入正确选项");
		ch =get_first();
	}

	return ch;
}
//同上(三级菜单使用)
char get_manage(void)
{
	char ch;

	ch =get_first();
	while(!(ch >= 'a' && ch <= 'g' || ch == 'q'))
	{
		puts("请输入正确选项");
		ch =get_first();
	}

	return ch;
}
//返回键入的第一个字母字符并清空缓存区
char get_first()
{
    char ch = 0;

    while (!isalpha(ch = getchar()))
        continue;
	while (getchar() != '\n')
		continue;

    return tolower(ch);	 //返回小写值
}
//注册函数
void register_user()
{
	FILE * fp;
	user new_, old;		//这里new后面多'_'的原因是c++有个关键字是new
	char temp[SIZE]; 

	show_line("          请开始注册您的账号!");
	
	if ((fp = fopen("users", "r")) == NULL)
	{
		fprintf(stderr, "读取用户信息失败！\n");
		system("sleep 3s");
		return;
	}
	puts("请输入您的账号：");
	s_gets(new_.id, SIZE);
	while (fread(&old, sizeof(user), 1, fp) == 1)
	{
		decipher(old.pwd);
		if (!strcmp(new_.id, old.id))
		{
			fprintf(stderr, "账号重复！\n");
			system("sleep 3s");
			return;
		}
	}
	puts("请输入您的密码：");
	secret_pwd(new_.pwd);
	puts("请再次输入您的密码：");
	secret_pwd(temp);
	while (1)
	{
		if (!strcmp(new_.pwd, temp))
			break;
		puts("两次密码不一致！");
		puts("请重新输入您的密码：");
		secret_pwd(new_.pwd);
		puts("请再次输入您的密码：");
		secret_pwd(temp);
	}
	puts("请输入您的电话号码以便找回密码：");
	new_.tele = get_tele();
	fp = fopen("users", "a");
	encipher(new_.pwd);
	fwrite(&new_, sizeof(user), 1, fp);
	printf("账号%s已注册成功！\n", new_.id);
	system("sleep 3s");
	
	fclose(fp);
	return;
}
//安全地获取电话号码
//64位架构下long最大值是9223372036854775807，用来存储电话号码很合适
long get_tele()
{
	long input;
	char ch;

	while (scanf("%ld%*c", &input) != 1) 
	{
		while ((ch =getchar()) != '\n')
			putchar(ch);
		printf(" 不是一个电话号码！\n请您重新输入：\n");
	}

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
		printf(" 不是一个正整数！\n请您重新输入：\n");
	}

	return input;
}
//登录函数
void login()
{
	FILE * fp;
	user old;
	char temp[SIZE];

	show_line("          请开始登录您的账号！");

	if ((fp = fopen("users", "r")) == NULL)
	{
		fprintf(stderr, "读取用户信息失败！\n");
		system("sleep 3s");
		return;
	}
	puts("请输入您的账号：");
	s_gets(temp, SIZE);
	while (!feof(fp))	//判断是否到达文件结尾
	{
		fread(&old, sizeof(user), 1, fp);
		decipher(old.pwd);
		if (!strcmp(temp, old.id))
		{
			puts("请输入您的密码：");
			secret_pwd(temp);
			if (!strcmp(temp, old.pwd))
			{
				show_line("              登录成功！");
				system("sleep 3s");	//休眠，用起来比较带感哈哈哈哈
				internal_menu();
				return;
			}
			else
			{
				puts("密码不匹配！");
				system("sleep 3s");
				return;
			}
		}
	}
	printf("账号%s不存在！\n", temp);
	system("sleep 3s");
	fclose(fp);

	return;
}
//找回密码函数
void find_pwd()
{
	FILE * fp;
	user old;
	char temp[SIZE];
	long key;

	show_line("          请开始找回您的密码!");

	if ((fp = fopen("users", "r")) == NULL)
	{
		fprintf(stderr, "读取用户信息失败！\n");
		system("sleep 3s");
		return;
	}

	puts("请输入您的账号：");
	s_gets(temp, SIZE);
	while (!feof(fp))
	{
		fread(&old, sizeof(user), 1, fp);
		decipher(old.pwd);
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
//自定义的getch()函数
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
//加密函数
void encipher(char *original)
{
    int key = 1;

    for (int i = 0; i < strlen(original); i++)
    {
		original[i] += key;
		key=-key;
    }

	return;
}
//解密函数
void decipher(char *cipher)
{
    int key = -1;
    for (int i = 0; i < strlen(cipher); i++)
    {
        cipher[i] += key;
        key = -key;
    }

	return;
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

	return;
}
//用户菜单（一级菜单）
void user_menu()
{
	char choice;			//记录get_choice()返回值

	show_line("       欢迎使用旭日苑菜品管理系统！");
//用字符串来输出是一个比较好用的技巧
    printf("\n      %-20s%-20s\n      %-20s%-20s\n\n", "a. 注册账号", "b. 登录账号", "c. 找回密码", "q. 退出系统");
	for (int i =0; i < 40; i++)
		putchar('*');
	putchar('\n');
	create_user_file();
	while ((choice = get_choice()) != 'q')
	{
		switch(choice)
		{
			case 'a': register_user();
					  break;
			case 'b': login();
					  break;
			case 'c': find_pwd();
					  break;
		}
		show_line("      欢迎使用旭日苑菜品管理系统！");
	    printf("\n     %-20s%-20s\n     %-20s%-20s\n\n", "a. 注册账号", "b. 登录账号", "c. 找回密码", "q. 退出系统");
		for (int i =0; i < 40; i++)
		putchar('*');
     	putchar('\n');
	}

	return;
}
//登录成功后的二级菜单
void internal_menu()
{
	char choice;

	show_line("      欢迎使用旭日苑菜品管理系统！");
    printf("\n     %-20s%-20s\n     %-20s%-20s\n\n", "a. 菜品管理", "b. 功能介绍", "c. 开发者介绍", " q. 注销账号");
	for (int i =0; i < 40; i++)
	putchar('*');
 	putchar('\n');

	while (choice = get_choice())
	{
		switch(choice)
		{
			case 'a': dish_manage_menu();
					  break;
			case 'b': show_function();
					  break;
			case 'c': show_author();
					  break;
			case 'q': show_line("              注销成功！");
					  system("sleep 3s");
					  return;
		}
		show_line("      欢迎使用旭日苑菜品管理系统！");
	    printf("\n     %-20s%-20s\n     %-20s%-20s\n\n", "a. 菜品管理", "b. 功能介绍", "c. 开发者介绍", " q. 注销账号");
		for (int i =0; i < 40; i++)
		putchar('*');
    	putchar('\n');
	}

	return;
}
//开发者介绍
void show_author()
{
	show_line("             开发者介绍");
	puts("    > Author: NiGo");
    system("sleep 1s");
	puts("    > Mail:   nigo@xiyoulinux.org");
	system("sleep 1s");
	puts("    > School: XUPT");
	system("sleep 1s");
	puts("    > Class:  WLW-1902");
	system("sleep 1s");
	press_to_continue();

	return;
}
//模拟任意键继续
void press_to_continue()
{
	puts("\n请按任意键继续...");
	getch();

	return;
}
//菜品管理菜单（三级菜单）
void dish_manage_menu()
{
	char manage;
	Node * head;

	create_dish_file();
	head = init_dish();
	
	show_line("              菜品管理");
	printf("\n     %-20s%-20s\n     %-20s%-20s\n", "a. 录入菜品", "b. 删除菜品", "c. 查找菜品", "d. 浏览菜品");
	printf("     %-20s%-20s\n     %-20s%-20s\n\n", "e. 修改菜品", "f. 排序菜品", "g. 保存信息", "q. 返回上级");
	for (int i =0; i < 40; i++)
		putchar('*');
 	putchar('\n');
	
	while ((manage = get_manage()) != 'q')
	{
		switch(manage)
		{
			case 'a': add_dish(&head);
					  break;
			case 'b': delete_dish(head);
					  break;
			case 'c': search_dish_menu(head);
					  break;
			case 'd': show_dish(head);
					  break;
			case 'e': modify_dish(head); 
					  break;
			case 'f': sort_dish_menu(head);
					  break;
			case 'g': save_dish(head);
					  break;
		}
		show_line("              菜品管理");
		printf("\n     %-20s%-20s\n     %-20s%-20s\n", "a. 录入菜品", "b. 删除菜品", "c. 查找菜品", "d. 浏览菜品");
		printf("     %-20s%-20s\n     %-20s%-20s\n\n", "e. 修改菜品", "f. 排序菜品", "g. 保存信息", "q. 返回上级");
		for (int i =0; i < 40; i++)
		putchar('*');
		putchar('\n');
	}
	free_dish(&head);

	return;
}
//功能介绍
void show_function()
{
	int ispull = -1;	//记录是否展开下拉式菜单
	char ch;

	show_line("              功能介绍");
	printf("%-20s%20s\n", "空格键展开/关闭", "Q键结束介绍");
	while (toupper(ch = getch()) != 'Q')
	{
		if (ch == ' ')
			ispull = -ispull;
		if (ispull == -1)
		{
			show_line("              功能介绍");
			printf("%-20s%20s\n", "空格键展开/关闭", "Q键结束介绍");
		}
		else
		{
			show_line("              功能介绍");
			printf("%-20s%20s\n", "空格键展开/关闭", "Q键结束介绍");
			puts("    > Function 1: 登录系统");
			puts("    > Function 2: 管理系统");
		}
	}

		return;
}
//打印星星(最多打印5颗星星)
void print_star(int n)
{
	if (n > 5)
		n = 5;
	for (int i = 1; i < n; i++)
		printf("* ");
	putchar('*');
	putchar('\n');

	return;
}
//初始化链表(菜品)
Node * init_dish()
{
	Node *p = NULL;
	FILE * fp;
	Dish temp;
	
	if ((fp = fopen("dishes", "r")) == NULL)
	{
		fprintf(stderr, "读取菜品信息失败！\n");
		free_dish(&p);
		system("sleep 3s");
		return NULL;
	}
	while (fread(&temp, sizeof(Dish), 1, fp) == 1)
	{
		Node * p1, * p2 = p;	//p1作为新指针，p2作为检索指针
		if ((p1 = (Node*)malloc(sizeof(Node))) == NULL)
		{
			fprintf(stderr, "内存空间不足！\n");
			free_dish(&p);
			system("sleep 3s");
			return NULL;
		}
		p1->next = NULL;
		strcpy(p1->name, temp.name);
		p1->price = temp.price;
		p1->star = temp.star;
		strcpy(p1->restaurant, temp.restaurant);
		strcpy(p1->comment, temp.comment);
		if (p2 != NULL)
		{
			while (p2->next != NULL)
				p2 = p2->next;
			p2->next = p1;
		}
		else
			p = p1;
	}
	fclose(fp);
	return p;
}
//释放链表(防止内存泄露)
void free_dish(Node **head)
{
	Node *temp;
	Node **p = head;
	while(*head != NULL)
	{
		temp = *head;
		*head = (*head)->next;
		free(temp);
	}
	*p = NULL;

	return;
}
//录入菜品(尾插)
void add_dish(Node **head)
{
	Node * p1, * p2 = *head;

	show_line("              录入菜品");
	if ((p1 = (Node*)malloc(sizeof(Node))) == NULL)
	{
		fprintf(stderr, "内存空间不足！\n");
		system("sleep 3s");
		return;
	}
	puts("请输入菜品名称：");
	s_gets(p1->name, SIZE);
	puts("请输入菜品价格：");
	p1->price = get_int();
	
	puts("请输入XUPTer们的评价:");
	s_gets(p1->comment, SIZE);
	puts("请输入菜馆名称：");
	s_gets(p1->restaurant, SIZE);
	puts("请输入该菜品的星级");
	p1->star = get_int();
	p1->next = NULL;
	if (*head != NULL)
	{
		while (p2->next != NULL) 
			p2 = p2->next;
		p2->next = p1;
	}
	else
		*head = p1;
	show_line("             录入成功！");
	system("sleep 3s");
	return;
}
//将菜品信息写入dishes
void save_dish(Node *head)
{
	Dish temp;
	FILE * fp;
	Node * p = head;

	show_line("          写入数据中...");
	if ((fp = fopen("dishes", "wb+")) == NULL)
	{
		fprintf(stderr, "写入菜品信息失败！\n");
		system("sleep 3s");
		return;
	}
	while (p)
	{
		strcpy(temp.name, p->name);
		temp.price = p->price;
		temp.star = p->star;
		strcpy(temp.restaurant, p->restaurant);
		strcpy(temp.comment, p->comment);
		if (fwrite(&temp, sizeof(Dish), 1, fp) != 1)
		{
			fprintf(stderr, "写入菜品信息失败！\n");
			system("sleep 3s");
			return;
		}
		p = p->next;
	}
	show_line("              保存成功!");
	system("sleep 3s");
	fclose(fp);
	return;
}
//打印报表
void show_dish(Node *head)
{
	Node * p = head;
	int count = 0;
	double total_p = 0.0, total_s = 0.0;

	show_line("          《旭日苑菜品一览》");
	while (p)
	{
		total_p += p->price;
		total_s += p->star;
		printf("    > 菜品 %-2d:  %s\n", ++count, p->name);
		printf("    > 价格:  %d\n", p->price);
		printf("    > 菜馆:  %s\n", p->restaurant);
		printf("    > 星级:  ");
		print_star(p->star);
		printf("    > 评价:  %s\n", p->comment);
		p = p->next;
		for (int i = 0; i < 40; i++)
			putchar('*');
		putchar('\n');
	}
	printf(" > 本系统共录入%d样菜品\n", count);
	printf(" > 菜品均价为%.2lf元，平均星级为%.2lf级\n", total_p/count, total_s/count);
	for (int i = 0; i < 40; i++)
		putchar('*');
	putchar('\n');
	press_to_continue();

	return;
}
//查找菜品菜单
void search_dish_menu(Node *head)
{
	Node * p = head;
	char choice;

	show_line("              查找菜品");
	printf("\n     %-20s%-20s\n     %-20s%-20s\n\n", "a. 按名称查找", "b. 按菜馆查找", "c. 按价格查找", "q. 返回上级");
	for (int i =0; i < 40; i++)
		putchar('*');
 	putchar('\n');

	while ((choice = get_choice()) != 'q')
	{
		switch (choice)
		{
			case 'a': search_by_name(p);
					  break;
			case 'b': search_by_restaurant(p);
					  break;
			case 'c': search_by_price(p);
					  break;
		}
		show_line("              查找菜品");
		printf("\n     %-20s%-20s\n     %-20s%-20s\n\n", "a. 按名称查找", "b. 按菜馆查找", "c. 按价格查找", "q. 返回上级");
		for (int i =0; i < 40; i++)
			putchar('*');
		putchar('\n');
	}

	return;
}
//按菜品名查找
void search_by_name(Node *head)
{
	Node * p = head;
	char temp[SIZE];

	puts("请输入需要查找的菜品名：");
	s_gets(temp, SIZE);
	while (p)
	{
		if (!strcmp(temp, p->name))
		{
			puts("查找成功！");
			for (int i = 0; i < 40; i++)
				putchar('*');
			putchar('\n');
			printf("    > 菜品:  %s\n", p->name);
			printf("    > 价格:  %d\n", p->price);
			printf("    > 菜馆:  %s\n", p->restaurant);
			printf("    > 星级:  ");
			print_star(p->star);
			printf("    > 评价:  %s\n", p->comment);
			for (int i = 0; i < 40; i++)
				putchar('*');
			putchar('\n');
			press_to_continue();
			return;
		}
		p = p->next;
	}
	puts("未查找到符合条件的菜品！");
	press_to_continue();

	return;
}
//按价格范围查找
void search_by_price(Node *head)
{
	Node * p = head;
	int min, max;
	int isfirst = 1;	//是否打印头信息
	puts("请输入需要查找的菜品价格范围：");
	puts("最小值：");
	min = get_int();
	puts("最大值：");
	max = get_int();
	while (p)
	{
		if (p->price <= max && p->price >= min)
		{
			if (isfirst)
			{
				puts("查找成功！");
				printf("下列是价格在%d和%d之间的菜品：\n", min, max);
				isfirst = 0;
				for (int i = 0; i < 40; i++)
					putchar('*');
				putchar('\n');
			}
			printf("    > 菜品:  %s\n", p->name);
			printf("    > 价格:  %d\n", p->price);
			printf("    > 菜馆:  %s\n", p->restaurant);
			printf("    > 星级:  ");
			print_star(p->star);
			printf("    > 评价:  %s\n", p->comment);
			for (int i = 0; i < 40; i++)
				putchar('*');
			putchar('\n');
		}
		p = p->next;
	}
	if (isfirst)	//如果没有找到过
		puts("未查找到符合条件的菜品！");
	press_to_continue();

	return;	
}
//按所属菜馆查找
void search_by_restaurant(Node *head)
{
	Node * p = head;
	char temp[SIZE];
	int isfirst = 1;	//是否打印头信息
	puts("请输入需要查找的菜馆名：");
	s_gets(temp, SIZE);
	while (p)
	{
		if (!strcmp(temp, p->restaurant))
		{
			if (isfirst)
			{
				puts("查找成功！");
				printf("下列是“%s”名下的菜品：\n", temp);
				isfirst = 0;
				for (int i = 0; i < 40; i++)
					putchar('*');
				putchar('\n');
			}
			printf("    > 菜品:  %s\n", p->name);
			printf("    > 价格:  %d\n", p->price);
			printf("    > 星级:  ");
			print_star(p->star);
			printf("    > 评价:  %s\n", p->comment);
			for (int i = 0; i < 40; i++)
				putchar('*');
			putchar('\n');
		}
		p = p->next;
	}
	if (isfirst)	//如果没有找到过
		puts("未查找到符合条件的菜品！");
	press_to_continue();

	return;
}
//删除菜品
void delete_dish(Node *head)
{
	Node * p = head, * prev = NULL;
	char temp[SIZE];

	show_line("              删除菜品");
	puts("请输入需要删除的菜品名称：");
	s_gets(temp, SIZE);
	while (p)
	{
		if (!strcmp(temp, p->name))
		{
			prev->next = p->next;
			free(p);
			p = NULL;
			show_line("             删除成功！");
			system("sleep 3s");
			return;
		}
		prev = p;
		p = p->next;
	}
	puts("未查找到符合条件的菜品！");
	press_to_continue();

	return;
}
//按价格升序排序
void sort_pup_dish(Node *head)
{
	Node *p = head, *pmin;
	while (p)
	{
		pmin = find_pmin_dish(p);
		if (strcmp(pmin->name, p->name))
		{
			swap_dish(pmin, p);
		}
		p = p->next;
	}

	return;
}
//按星级升序排序
void sort_sup_dish(Node *head)
{
	Node *p = head, *pmin;
	while (p)
	{
		pmin = find_smin_dish(p);
		if (strcmp(pmin->name, p->name))
		{
			swap_dish(pmin, p);
		}
		p = p->next;
	}

	return;
}
//按价格降序排序
void sort_pdown_dish(Node *head)
{
	Node *p = head, *pmax;
	while (p)
	{
		pmax = find_pmax_dish(p);
		if (strcmp(pmax->name, p->name))
		{
			swap_dish(pmax, p);
		}
		p = p->next;
	}

	return;
}
//按星级降序排序
void sort_sdown_dish(Node *head)
{
	Node *p = head, *pmax;
	while (p)
	{
		pmax = find_smax_dish(p);
		if (strcmp(pmax->name, p->name))
		{
			swap_dish(pmax, p);
		}
		p = p->next;
	}

	return;
}
//找到菜品中价格最小值节点
Node * find_pmin_dish(Node *head)
{
	Node *pmin = head, *p = head->next;		//p指向当前节点位置，pmin用于存储最小值节点
	while (p)
	{
		if (p->price < pmin->price)
			pmin = p;
		p = p->next;
	}

	return pmin;
}
//找到菜品中星级最小值节点
Node * find_smin_dish(Node *head)
{
	Node *pmin = head, *p = head->next;
	while (p)
	{
		if (p->star < pmin->star)
			pmin = p;
		p = p->next;
	}

	return pmin;
}
//找到菜品中星级最大值节点
Node * find_smax_dish(Node *head)
{
	Node *pmax = head, *p = head->next;
	while (p)
	{
		if (p->star > pmax->star)
			pmax = p;
		p = p->next;
	}

	return pmax;
}
//找到菜品中价格最大值节点
Node * find_pmax_dish(Node *head)
{
	Node *pmax = head, *p = head->next;
	while (p)
	{
		if (p->price > pmax->price)
			pmax = p;
		p = p->next;
	}

	return pmax;
}
//交换菜品信息
void swap_dish(Node *p1, Node *p2)
{
	char name[SIZE];
	int price;
	char restaurant[SIZE];
	int star;
	char comment[SIZE];

	price = p1->price;
	p1->price = p2->price;
	p2->price = price;

	star = p1->star;
	p1->star = p2->star;
	p2->star = star;

	strcpy(name, p1->name);
	strcpy(p1->name, p2->name);
	strcpy(p2->name, name);

	strcpy(restaurant, p1->restaurant);
	strcpy(p1->restaurant, p2->restaurant);
	strcpy(p2->restaurant, restaurant);

	strcpy(comment, p1->comment);
	strcpy(p1->comment, p2->comment);
	strcpy(p2->comment, comment);

	return;
}
//排序菜品菜单
void sort_dish_menu(Node *head)
{
	Node * p = head;
	char choice;

	show_line("              排序菜品");
	printf("\n  %-20s%-20s\n  %-20s%-20s\n\n", "a. 按价格升序排序", "b. 按价格降序排序", "c. 按星级升序排序", "q. 按星级降序排序");
	for (int i =0; i < 40; i++)
		putchar('*');
 	putchar('\n');

	choice = get_choice();
	switch (choice)
	{
		case 'a': sort_pup_dish(p);
				  break;
		case 'b': sort_pdown_dish(p);
				  break;
		case 'c': sort_sup_dish(p);
				  break;
		case 'q': sort_sdown_dish(p);
				  break;
	}
	show_line("              排序成功！");
	system("sleep 3s");
	return;
}
//修改菜品信息
void modify_dish(Node *head)
{
	Node *p = head;
	char temp[SIZE];

	show_line("              修改菜品");
	puts("请输入需要修改的菜品名称：");
	s_gets(temp, SIZE);

	while (p)
	{
		if (!strcmp(temp, p->name))
		{
			puts("请输入菜品价格：");
			p->price = get_int();
			puts("请输入XUPTer们的评价:");
			s_gets(p->comment, SIZE);
			puts("请输入菜馆名称：");
			s_gets(p->restaurant, SIZE);
			puts("请输入该菜品的星级");
			p->star = get_int();
			show_line("             修改成功！");
			system("sleep 3s");
			return;
		}
		p = p->next;
	}

	puts("不存在该菜品！");
	press_to_continue();
	return;
}
