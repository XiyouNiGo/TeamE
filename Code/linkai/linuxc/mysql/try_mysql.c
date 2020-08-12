
/*************************************************************************
    > File Name: mysql_thread_safe.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年08月03日 星期一 23时14分37秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <mysql/mysql.h>

MYSQL mysql;

void my_err(const char *str)
{
     perror(str);
     exit(1);
}

int get_random_int()
{
    srand( (unsigned int)time(NULL) );
    int ret;
    char query[64];
    MYSQL_RES *result;
    while (1)
    {
        ret = rand() % INT_MAX;
        sprintf(query, "SELECT * FROM Human WHERE salary = %d", ret);

        if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
        if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
        //查询不到说明唯一
        if (mysql_num_rows(result) == 0)
            return ret;
    }
}

void* func(void* arg)
{
    mysql_init(&mysql);
	mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "test", 0, NULL, 0);
    mysql_query(&mysql, "SELECT * FROM Human WHERE salary = 2");
    MYSQL_RES *result = mysql_store_result(&mysql);
    if (result == NULL)
        my_err("mysql_store_result error");
    if (mysql_num_rows(result) == 0)
        printf("No result\n");
    for (int i = 0; i < mysql_num_rows(result); i++)
    {
        MYSQL_ROW row = mysql_fetch_row(result);
        for (int j = 0; j < mysql_num_fields(result); j++)
            printf("%s\n", row[j]);
    }
    mysql_free_result(result);
    mysql_thread_end();
	return (void *)0;
}
int main()
{
    pthread_t tid;
    mysql_init(&mysql);
	mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "test", 3306, NULL, 0);
    //pthread_create(&tid, NULL, func, (void*)&mysql);
    printf("%d\n", get_random_int());
}
