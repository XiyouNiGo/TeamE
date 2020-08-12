
/*************************************************************************
    > File Name: try.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年08月12日 星期三 01时45分24秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <mysql/mysql.h>
#include <pthread.h>

void my_err(const char *str)
{
     perror(str);
     exit(1);
}

MYSQL mysql;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *func(void *arg)
{
    //pthread_mutex_lock(&mutex);
    mysql_init(&mysql);
    if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "chatroom", 0, NULL, 0) == NULL)
    {
        my_err("mysql_real_connect error");
    }
    //pthread_mutex_unlock(&mutex);
    
}

int main(int argc, char *argv[])
{
    pthread_t tid;
    if (mysql_init(&mysql) == NULL)
    {
        my_err("mysql_init error");
    }
    if (mysql_library_init(0, NULL, NULL) < 0)
    {
        my_err("mysql_library_init error");
    }
    //连接服务器(3306为mysql默认端口)
    if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "chatroom", 0, NULL, 0) == NULL)
    {
        my_err("mysql_real_connect error");
    }
    pthread_create(&tid, NULL, func, NULL);
    pthread_create(&tid, NULL, func, NULL);
    pthread_create(&tid, NULL, func, NULL);
    pthread_create(&tid, NULL, func, NULL);

    while (1);
}
