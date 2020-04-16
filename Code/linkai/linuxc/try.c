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
#include <signal.h>
#include <errno.h>

#define PARAM_NONE   0                 //无参数
#define PARAM_A      1                 //-a  显示所有文件
#define PARAM_L      2                 //-l  显示一个文件的详细信息
#define MAXROWLEN    80                //一行显示的最多字符数
#define KB           1024
#define MB           1024*1024
#define GB           1024*1024*1024
#define TB           1024*1024*1024*1024
void display_single(char*);
void display_attribute(struct stat, char*);
void display(int, char*);
void display_dir(int, char*);

int g_leave_len = MAXROWLEN;	//一行剩余长度，用于输出对齐
int g_maxlen;				//存放某目录下第一行的最长文件名的长度

static void mask_ctrl_c()
{
    sigset_t intmask;
    sigemptyset(&intmask);/* 将信号集合设置为空 */
    sigaddset(&intmask,SIGINT);/* 加入中断 Ctrl+C 信号*/
    /*阻塞信号*/
    sigprocmask(SIG_BLOCK,&intmask,NULL);
}

int main(int argc, char *argv[])
{
	struct stat buf;
	int ret = lstat("", &buf);
	if (ret == -1)
		printf("asas");
}


