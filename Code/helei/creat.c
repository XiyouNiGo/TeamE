/*************************************************************************
	> File Name: creat.c
	> Author: 
	> Mail: 
	> Created Time: 2020年03月26日 星期四 09时24分27秒
 ************************************************************************/

#include<stdio.h>
#include<time.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>
#include<fcntl.h>
int main(void)
{
    //umask(0);
    //该语句会修改umask值，
    int fd;
    fd=open("creat.txt",O_CREAT|O_RDWR,0777);
    if(fd == -1)
    {
        perror("open fail");
        return 0;
    }
    perror("open ");
    write(fd,"hello world!",sizeof("hello world!"));

}
//本机umask为0022
//即使创建文件的权限是777，但是还是会和umask进行(mode & ~umask)进行运算
//所以创建的文件权限为755
//两者之间不能直接进行减法运算
