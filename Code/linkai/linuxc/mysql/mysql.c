
/*************************************************************************
    > File Name: mysql.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年07月18日 星期六 08时50分21秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <pthread.h>
#include <mysql/mysql.h>

void my_err(const char *str)
{
     perror(str);
     exit(1);
}

int main(int argc, char *argv[])
{
    MYSQL mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;

    if (mysql_init(&mysql) == NULL)
    {
        my_err("mysql_init error");
    }
    if (mysql_library_init(0, NULL, NULL) < 0)
    {
        my_err("mysql_library_init error");
    }
    //连接服务器(3306为mysql默认端口)
    if (mysql_real_connect(&mysql, "localhost", "root", "root", "test", 3306, NULL, 0) == 0)
    {
        my_err("mysql_real_connect error");
    }
    //设置字符集
    if (mysql_set_character_set(&mysql, "utf8") < 0)
    {
        my_err("mysql_set_character_set error");
    }
    //查询数据表
    if (mysql_real_query(&mysql, "SELECT * FROM Human", strlen("SELECT * FROM Human")))
    {
        my_err("mysql_real_query error");
    }
    //存储结果集
    if ( (res = mysql_store_result(&mysql)) == NULL)
    {
        my_err("mysql_store_result error");
    }
    //读取结果集并输出
    while (row = mysql_fetch_row(res))
    {
        printf("%s", row[0]);
    }
    //释放结果集
    mysql_free_result(res);
    //关闭连接
    mysql_close(&mysql);
    
    mysql_library_end();

    return 0;
}
