
/*************************************************************************
    > File Name: try.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年08月06日 星期四 09时44分15秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>
#include <mysql/mysql.h>
//#include "inc/server/server_config.h"
//#include "inc/server/thread_pool.h"

void format_string(char *str, char *output[], int num)
{
    int i;
    char temp[256];
    for (i = 0; i < num; i++)
    {
        if (i == 0)
        {
            sprintf(str, "'%s'", output[i]);
        }
        else
        {
            strcpy(temp, str);
            //若为DEFAULT，不加单引号
            if (strcmp("DEFAULT", output[i]))
                sprintf(str, "%s,'%s'", temp, output[i]);
            else
                sprintf(str, "%s,%s", temp, output[i]);
        }
    }
}

void parse_string(char *str, char **output, int *num)
{
    char *temp;
    *num = 0;
    temp = (char*)strtok(str, "\036");
    while (temp)
    {
        *output++ = temp;
        (*num)++;
        temp = (char*)strtok(NULL, "\036");
    }
}

MYSQL mysql;

void my_err(char *str)
{
    fprintf(stderr, "%s", str);
    perror(":");
}

int main(int argc, char *argv[])
{
    if (mysql_init(&mysql) == NULL)
    {
        my_err("mysql_init error");
    }
    if (mysql_library_init(0, NULL, NULL) < 0)
    {
        my_err("mysql_library_init error");
    }
    //连接服务器(3306为mysql默认端口)
    if (mysql_real_connect(&mysql, "localhost", "root", "root", "chatroom", 0, NULL, 0) == NULL)
    {
        my_err("mysql_real_connect error");
    }
    //设置字符集
    if (mysql_set_character_set(&mysql, "utf8") < 0)
    {
        my_err("mysql_set_character_set error");
    }
    mysql_query(&mysql, "SELECT * FROM user_info WHERE uid = 2");
    MYSQL_RES *result = mysql_store_result(&mysql); 
    printf("%d\n", mysql_num_rows(result));
    MYSQL_ROW row = mysql_fetch_row(result);
    if (row[8] == NULL)
        printf("是NULL\n");
    else
        printf("%s\n", row[8]);
    /*int num;
    char *output[7];
    char pack_str[256] = "测试\036m\036DEFAULT\036DEFAULT\036DEFAULT\036DEFAULT\036DEFAULT";
    int account;
    char query[256];
    char temp[256];
    //pack_str被strtok破坏
    parse_string(pack_str, output, &num);
    format_string(temp, output, num);
    account = 123456;
    sprintf(query, "INSERT INTO user_info(account, nickname, gender, birthday, "
                   "location, signature, security_q, security_a, connect_fd) "
                   "VALUES(%d, %s, %d)", account, temp, 10000);
    printf("%s\n", query);*/
}
