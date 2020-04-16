#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include "debug.h"

int lock_set(int fd, struct flock *lock);	//锁的设置或释放锁
int lock_test(int fd, struct flock *lock);	//测试锁，lock指定锁能被设置时返回0

int main()
{
	int fd;
	int ret;
	struct flock lock;
	char read_buf[32];

	//打开或创建文件
	if ((fd = open("example_65", O_CREAT|O_TRUNC|O_RDWR, S_IRWXU)) == -1)
	{
		my_err("open", __LINE__);
	}
	//写数据
	if (write(fd, "test lock", 10) != 10)
	{
		my_err("write", __LINE__);
	}
	//初始化lock结构
	memset(&lock, 0, sizeof(struct flock));
	lock.l_start = SEEK_SET;
	lock.l_whence = 0;
	lock.l_len = 0;
	//设置读锁
	lock.l_type = F_RDLCK;
	if (lock_test(fd, &lock) == 0)	//能被设置
	{
		lock.l_type = F_RDLCK;
		lock_set(fd, &lock);
	}
	//读数据
	lseek(fd, 0, SEEK_SET);
	if ((ret = read(fd, read_buf, 10)) < 0)
	{
		my_err("read", __LINE__);
	}
	read_buf[ret] = '\0';
	printf("%s\n", read_buf);
	
	getchar();

	//设置写锁
	lock.l_type = F_WRLCK;
	if (lock_test(fd, &lock) == 0)	//能被设置
	{
		lock.l_type = F_WRLCK;
		lock_set(fd, &lock);
	}
	//释放锁
	lock.l_type = F_UNLCK;
	lock_set(fd, &lock) == 0;	//能被设置
	close(fd);

	return 0;
}
int lock_set(int fd, struct flock *lock)
{
	if (fcntl(fd, F_SETLK, lock) == 0)
	{
		if (lock->l_type == F_RDLCK)
			printf("set read lock, pid:%d\n", getpid());
		else if (lock->l_type == F_WRLCK)
			printf("set write lock, pid:%d\n", getpid());
		else if (lock->l_type == F_UNLCK)
			printf("release lock, pid:%d\n", getpid());
		return 0;
	}
	else
	{
		perror("lock operation failed:");
		return -1;
	}
}
int lock_test(int fd, struct flock *lock)
{
	if (fcntl(fd, F_GETLK, lock) == 0)
	{
		if (lock->l_type == F_UNLCK)	//能按参数lock要求设置锁(能的时候l_type设置为F_UNLCK)
		{
			printf("lock can be set in fd\n");
			return 0;
		}
		else	//存在不兼容的锁
		{
			if (lock->l_type == F_RDLCK)
			{
				printf("can't set lock, read lock has been set by:%d\n", lock->l_pid);
			}
			else if (lock->l_type == F_WRLCK)
			{
				printf("can't set lock, write lock has been set by:%d\n", lock->l_pid);
			}
			return -2;
		}
	}
	else
	{
		perror("get incompatible locks fail");
		return -1;
	}
}
