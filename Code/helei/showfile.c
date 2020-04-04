/*************************************************************************
	> File Name: showfile.c
	> Author: 
	> Mail: 
	> Created Time: 2020年03月26日 星期四 11时19分57秒
 ************************************************************************/

#include<stdio.h>
#include<dirent.h>
#include<sys/types.h>
#include<unistd.h>
int my_readdir(char *path)
{
    DIR * dir;
    struct dirent *ptr;
    if((dir = opendir(path))==NULL)
    {
        perror("opendir fail:");
        return 0;
    }
    while((ptr=readdir(dir))!=NULL)
    {
        printf("file name : %s\n",ptr->d_name);
    }
    closedir(dir);
    return 0;
}
int main(int argc,char *argv[])
{
    if(argc < 2 )
    {
        printf("listfile <target path>\n");
        return 0;
    }
    if(my_readdir(argv[1])<0)
    {
        return 0;
    }
    return 0;

}
/*struct dirent
{
    long d_ino;
    //存放该目录的i节点
    off_t d_off;
    //目录文件开头至此目录进入点的位移
    unsigned short d_reclen;
    //d_name 的长度
    char d_name [NAME_MAX + 1];
   //以NULL结尾的文件名
}
1、readdir函数原型：struct dirent * readdir(DIR *dir)；
2、返回值是结构体指针，指向下一个文件信息
3、该函数成功执行返回该目录下一个文件的信息，知道目录文件尾
返回NULL
4、DIR *dir 就像是文件描述符
*/
