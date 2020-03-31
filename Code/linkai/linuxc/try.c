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
#define MAXROWLEN    80                //一行显示的最多字符数

void display_single(char*);
void display_attribute(struct stat, char*);
void display(int, char*);
void display_dir(int, char*);

int g_leave_len = MAXROWLEN;	//一行剩余长度，用于输出对齐
int g_maxlen;				//存放某目录下第一行的最长文件名的长度


int main(int argc, char *argv[])
{
	if ((0 & 2) == 0)
		printf("d");
}


