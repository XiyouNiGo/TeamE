
/*************************************************************************
    > File Name: try1.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年08月12日 星期三 08时23分17秒
 ************************************************************************/

 #include <mysql/mysql.h>
 #include <pthread.h>
#include <stdio.h>
 void* func(void* arg)
{
	MYSQL* mysql = (MYSQL *)arg;
	mysql_real_connect(mysql, "127.0.0.1″, "root" , "123456″, "chen", 1234, NULL, 0);
	mysql_close(mysql);
	return (void *)0;
}
int main()
{
	MYSQL mysql;
	if (NULL == mysql_init(&mysql))
	{
		return -1;
	}
	pthread_t thread;
	pthread_create(&thread, NULL, func, &mysql);
	pthread_join(thread, NULL);
	return 0;
}
