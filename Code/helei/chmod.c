/****************mZ********************************************************
	> File Name: chmod.c
	> Author: 
	> Mail: 
	> Created Time: 2020年03月26日 星期四 08时49分41秒
 ************************************************************************/

#include<stdio.h>
#include<time.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>
int main(int argc,char *argv[])
{
    struct stat buf;
    if(argc!=2)
    {
        printf("Exit!\n");
        return 0;
    }
    else if(stat(argv[1],&buf)==-1)        //在buf中存储argv[1]对应的文件名的属性信息
    {
        perror("stat : ");
        return 0;
    }

    printf("device is : %d \n",buf.st_dev);
    //文件的设备编号
    printf("inode is : %d\n",buf.st_ino);
    //文件的i节点编号
    printf("mode is : %o\n",buf.st_mode);
    //文件的类型和存取权限
    printf("number of hard links is : %d\n",buf.st_nlink);
    //连到该文件的硬链接数目
    printf("user ID of owner is : %d\n",buf.st_uid);
    //文件所有者的用户id
    printf("group ID of owner is : %d\n",buf.st_gid);
    //文件所有组的组id
    printf("device type (if inode device) is : %d\n",buf.st_rdev);
    //若为设备文件，则为其设备编号
    printf("total size, in bytes is : %d\n",buf.st_size);
    //文件大小，以字节计算
    printf("blocksize for filesystem I/O is : %d\n",buf.st_blksize);
    //文件系统的io缓冲区大小
    printf("number of blocks allocated is %d\n",buf.st_blocks);
    //占用文件区块的个数，每一区块大小通常为512字节
    printf("time of last sccess is : %s\n",ctime (&buf.st_atime));
    //文件最近一次被访问的时间
    printf("time of last modification is : %s\n",ctime (&buf.st_mtime));
    //文件最后一次被修改的时间
    printf("time of last change is :%s\n",ctime (&buf.st_ctime));
    //文件最近一次被修改的时间，文件权限，所有者，所属组更新时改变
    return 0;

}
