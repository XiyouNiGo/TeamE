/*************************************************************************
	> File Name: getcwd.c
	> Author: 
	> Mail: 
	> Created Time: 2020年03月26日 星期四 10时13分46秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<errno.h>
int main(void)
{
    char buf[100]={0};
    char *buf2;
    buf2=getcwd(buf,sizeof(buf));
    if(getcwd == NULL)
    {
        perror("getcwd fail : ");
        return 0;
    }
    printf("%s\n",buf2);
}
//buf的长度一定要足够大去存下路径
//getcwd的返回值是 char * 
//且返回的内容是当前目录，
//如果自己对返回值的大小无法把控，防止出错，可以使用指针形式
//即buf2 = getcwd(0,0);
//这样更加节省内存，且防止出错
