/*************************************************************************
	> File Name: gte_ids.c
	> Author: 
	> Mail: 
	> Created Time: 2020年04月08日 星期三 08时52分16秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
int main(void)
{
    pid_t pid;           // _t 无符号整形
    pid_t ppid;
    uid_t uid;
    gid_t gid;
    pid = getpid();
    pid = getppid();
    printf("pid = %u\n",pid);
    printf("ppid = %u\n",ppid);
    printf("uid = %u\n",getuid());
    printf("gid = %u\n",getgid());
    while(1);       //利用 ps-aux  查看该进程
}

