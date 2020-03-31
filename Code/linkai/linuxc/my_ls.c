#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>	//我的ubuntu好像没有Linux/limits.h这个头文件
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <errno.h>
#include "debug.h"

#define PARAM_NONE   0                 //无参数
#define PARAM_A      1                 //-a  显示所有文件
#define PARAM_L      2                 //-l  显示一个文件的详细信息
#define PARAM_R		 4				   //-R  递归
#define MAXROWLEN    80     //一行显示的最多字符数
//颜色
#define WHITE		 0
#define BLUE		 1
#define GREEN        2
#define RED          3
#define LBLUE        4
#define YELLOW       5

void display_single(const char*, int);
void display_attribute(struct stat, const char*);
void display(int, const char*);
void display_dir(int, const char*);
void print_name(const char*, int);
int get_color(struct stat);

int g_leave_len = MAXROWLEN;	//一行剩余长度，用于输出对齐
int g_maxlen;				//存放某目录下第一行的最长文件名的长度

int main(int argc, char *argv[])
{
	char path[PATH_MAX+1];
	int num;	//-的数量，用于辅助判断有没有输入文件名或目录名
	char param[32] = {0};	//保存命令行参数
	int flag_param = PARAM_NONE;
	struct stat statbuf;	
	//解析a、l、R等参数，把-后的参数存到param里面
	for (int i = 1, j = 0; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			for (int k= 1; k < strlen(argv[i]); k++, j++)
			{
				param[j] = argv[i][k];
			}
			num++;
		}
	}
	int i = 0;
	while (param[i] != 0)
	{
		if (param[i] == 'a')
		{
			flag_param |= PARAM_A;	//很妙的按位或操作
			i++;
			continue;
		}
		else if (param[i] == 'l')
		{
			flag_param |= PARAM_L;
			i++;
			continue;
		}
		else if (param[i] == 'R')
		{
			flag_param |= PARAM_R;
			i++;
			continue;
		}
		else
		{
			fprintf(stderr, "my_ls: 不适用的选项 -- %c\n", param[i]);	//模拟ubuntu中ls的报错
			exit(1);
		}
	}
	//	printf("%d\n", flag_param);
	//没有文件或目录则显示当前目录
	if (num+1 == argc)
	{
		display_dir(flag_param, "./");
		return 0;
	}
	//解析文件/目录参数
	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			continue;
		}
		strcpy(path, argv[i]);
		//判断是否存在
		if (stat(path, &statbuf) == -1)
		{
			my_err("stat", __LINE__);
		}
		//如果是目录
		if (S_ISDIR(statbuf.st_mode))
		{
			//如果目录最后一个字符不是/则加上
			if (path[strlen(argv[i])-1] != '/')
			{
				path[strlen(argv[i])] = '/';
				path[strlen(argv[i])+1] = '\0';
			}
			display_dir(flag_param, path);
		}
		//如果是文件
		else
		{
			display(flag_param, path);
		}
	}
	return 0;
}
//没有-l选项时仅打印文件名
void display_single(const char *name, int color)
{
	int len;
	//如果本行不足以打印一个文件名则换行
	if (g_leave_len < g_maxlen)
	{
		putchar('\n');
		g_leave_len = MAXROWLEN;
	}
	len = g_maxlen-strlen(name);
	print_name(name, color);
	//打印空格
	for (int i = 0; i < len+2; i++)
	{
		putchar(' ');
	}
	g_leave_len -= g_maxlen+2;
}
//根据flag和path显示目标文件
void display(int flag, const char *path)
{
	int i, j, path_len = strlen(path), color;
	char basename[NAME_MAX+1];	//文件名
	struct stat statbuf;	//储存lstat调用的产生的文件信息
	//解析文件名
	//printf("%s\n", path);
	for (i = 0, j = 0; i < path_len; i++)
	{
		if (path[i] == '/')
		{
			j = 0;
			continue;
		}
		basename[j++] = path[i];
	}
	basename[j] = '\0';
	//	printf("%s\n", basename);
	if (lstat(path, &statbuf) == -1)
	{
		my_err("lstat", __LINE__);
	}
	color = get_color(statbuf);
	switch (flag)
	{
		case PARAM_NONE:
			if (basename[0] != '.')
			{
				display_single(basename, color);
			}
			break;
		case PARAM_R:
			if (basename[0] != '.')
			{
				display_single(basename, color);
			}
			break;
		case PARAM_A:
			display_single(basename, color);
			break;
		case PARAM_A + PARAM_R:
			display_single(basename, color);
			break;
		case PARAM_L:
			if (basename[0] != '.')
			{
				display_attribute(statbuf, basename);
			}
			break;
		case PARAM_L + PARAM_R:
			if (basename[0] != '.')
			{
				display_attribute(statbuf, basename);
			}
			break;
		case PARAM_A + PARAM_L:
			display_attribute(statbuf, basename);
			break;
		case PARAM_A + PARAM_L + PARAM_R:
			display_attribute(statbuf, basename);
			break;
	}
}

void display_dir(int flag_param, const char *path)
{


	DIR *dir;
	struct dirent *ptr;	//pointer recorder
	int count = 0;
	char filenames[256][PATH_MAX+1], temp[PATH_MAX+1];
	//获取总和最长的文件名
	if ((dir = opendir(path)) == NULL)
	{
		my_err("opendir", __LINE__);
	}
	while ((ptr = readdir(dir)) != NULL)
	{
		if (g_maxlen < strlen(ptr->d_name))
		{
			g_maxlen = strlen(ptr->d_name);
		}
		count++;
	}
	if (count > 256)
	{
		my_err("too many files under this dir", __LINE__);
	}
	closedir(dir);
	if ((dir = opendir(path)) == NULL)
	{
		my_err("opendir", __LINE__);
	}
	//获取目录下的所有文件名(绝对路径)
	int len = strlen(path);
	for (int i = 0; i < count; i++)
	{
		if ((ptr = readdir(dir)) == NULL)
		{
			my_err("readdir", __LINE__);
		}
		strncpy(filenames[i], path, len);
		filenames[i][len] = '\0';
		strcat(filenames[i], ptr->d_name);
		filenames[i][len+strlen(ptr->d_name)] = '\0';
	}
	//冒泡排序
	for (int i = 0; i < count-1; i++)
	{
		for (int j = 0; j < count-1-i; j++)
		{
			if (strcmp(filenames[j], filenames[j+1]) > 0)
			{
				strcpy(temp, filenames[j+1]);
				strcpy(filenames[j+1], filenames[j]);
				strcpy(filenames[j], temp);
			}
		}
	}
	for (int i = 0; i < count; i++)
	{
		//printf("%s\n", filenames[i]);
		display(flag_param, filenames[i]);
	}
	closedir(dir);
	//如果没有-l选项就打印一个换行符
	if ((flag_param & PARAM_L) == 0)	//位运算符优先级没有==高(找了半天...)
		putchar('\n');
}
//-l参数打印文件信息
void display_attribute(struct stat buf, const char *name)
{
	char buf_time[32];
	struct passwd *pwd;	//获取用户名
	struct group *grp;	//获取组名
	int color = WHITE;
	//文件类型
	if(S_ISLNK(buf.st_mode))
	{
		printf("l");
		color = LBLUE;
	}
	else if(S_ISREG(buf.st_mode))
	{
		printf("-");
	}
	else if(S_ISDIR(buf.st_mode))
	{
		printf("d");
		color = BLUE;
	}
	else if(S_ISCHR(buf.st_mode))
	{
		printf("c");
		color = YELLOW;
	}
	else if(S_ISBLK(buf.st_mode))
	{
		printf("b");
		color = YELLOW;
	}
	else if(S_ISFIFO(buf.st_mode))
	{
		printf("f");
	}
	else if(S_ISSOCK(buf.st_mode))
	{
		printf("s");
	}
	//user的权限
	if(buf.st_mode&S_IRUSR)
	{
		printf("r");
	}
	else
	{
		printf("-");
	}
	if(buf.st_mode&S_IWUSR)
	{
		printf("w");
	}
	else
	{
		printf("-");
	}
	if(buf.st_mode&S_IXUSR)
	{
		printf("x");
		color = GREEN;
	}
	else
	{
		printf("-");
	}
	//group的权限
	if(buf.st_mode&S_IRGRP)
	{
		printf("r");
	}
	else
	{
		printf("-");
	}
	if(buf.st_mode&S_IWGRP)
	{
		printf("w");
	}
	else
	{
		printf("-");
	}
	if(buf.st_mode&S_IXGRP)
	{
		printf("x");
	}
	else
	{
		printf("-");
	}
	//others的权限
	if(buf.st_mode&S_IROTH)
	{
		printf("r");
	}
	else
	{
		printf("-");
	}
	if(buf.st_mode&S_IWOTH)
	{
		printf("w");
	}
	else
	{
		printf("-");
	}
	if(buf.st_mode&S_IXOTH)
	{
		printf("x");
	}
	else
	{
		printf("-");
	}
	putchar(' ');
	//目录也有可能是可执行的，所以再来一次
	if (S_ISDIR(buf.st_mode))
	{
		color = BLUE;
	}
	/*通过uid与gid获取文件所有者的用户名和组名*/
	pwd=getpwuid(buf.st_uid);
	grp=getgrgid(buf.st_gid);
	printf("%d ",(int)buf.st_nlink);   //inode数量
	printf("%s ",pwd->pw_name);    //用户名
	printf("%s ",grp->gr_name);    //组名
	printf("%5ld ",buf.st_size);     //容量大小
	strcpy(buf_time,ctime(&buf.st_mtime));
	buf_time[strlen(buf_time)-1]='\0';//去掉换行符
	printf("%s ",buf_time);//打印文件时间
	print_name(name, color);
	putchar('\n');
}
//搞颜色(这里末尾不能加换行符，因为single跟attri输出结果是不一样的)
void print_name(const char *name, int color)
{
	if (color == GREEN)
	{
        printf("\033[1m\033[32m%-s\033[0m",name);
    }
	else if (color == BLUE)
	{
        printf("\033[1m\033[34m%-s\033[0m",name);
    }
	else if (color == WHITE)
	{   
		printf("%-s",name);
	}
	else if (color == LBLUE)
	{
		printf("\033[1m\033[36m%-s\033[0m",name);
	}
	else if (color == YELLOW)
	{
		printf("\033[1m\033[33m%-s\033[0m",name);
	}
}
//获得颜色
int get_color(struct stat buf)
{
	int color = WHITE;
	if (S_ISLNK(buf.st_mode))
	{
		color = LBLUE;
	}
	else if(S_ISDIR(buf.st_mode))
	{
		color = BLUE;
	}
	else if (S_ISCHR(buf.st_mode) || S_ISBLK(buf.st_mode))
	{
		color = YELLOW;
	}
	else if (buf.st_mode&S_IXUSR)
	{
		color = GREEN;
	}
	return color;
}
