#include "../../inc/server/server_config.h"
#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
//获取具体时间
void get_timestamp(char *timestamp)
{
    time_t seconds;
    struct tm *p;
    //返回1900-01-01 00:00:00至现在的秒数
    seconds = time(NULL);
    //将该秒数转化为本地时间
    p = localtime(&seconds);
    sprintf(timestamp, "%04d-%02d-%02d %02d:%02d:%02d", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday,
            p->tm_hour, p->tm_min, p->tm_sec);
}
//写入日志(默认日志为server.log)
void write_log(const char *log)
{
    static int fd_log;
    if ( (fd_log = open("log/server.log", O_RDWR | O_CREAT | O_APPEND, 0644)) < 0)
        my_err("open error");
    static char log_buf[BUFSIZE_LINE];
    get_timestamp(log_buf);
    sprintf(log_buf, "%s [%d] (%lu): %s\n", log_buf, (int)getpid(), (unsigned long)pthread_self(), log);
    write(fd_log, log_buf, BUFSIZE_LINE);
}
//安全的读取信息
int read_s(int connect_fd, char *read_buf, int count)
{
    int left_count = count;
    int ret;
    while (left_count > 0)
    {
        if ( (ret = read(connect_fd, read_buf, left_count)) < 0)
        {
            //若信号中断，继续读操作
            if (errno == EINTR)
                continue;
            else
            {
                report_err("read_s error");
                return -1;
            }
        }
        //对端关闭
        else if (ret == 0)
            return count - left_count;  //返回实际读取的字节数
        //成功读取
        else
        {
            read_buf += ret;
            left_count -= ret;
        }
    }
    return count;
}
//安全的写信息
int write_s(int connect_fd, char *write_buf, int count)
{
    int left_count = count;
    int ret;
    while (left_count > 0)
    {
        if ( (ret = write(connect_fd, write_buf, left_count)) < 0)
        {
            if (errno == EINTR)
                continue;
            else
            {
                report_err("write_s error");
                return -1;
            }
        }
        //返回0，且无errno代表没有错误发生
        else if (ret == 0)
            continue;
        //成功写入
        else
        {
            write_buf += ret;
            left_count -= ret;
        }
    }
    return count;
}
//打包packet数据包
void bale_packet(struct packet *packet, int len, int flag)
{
    packet->len = len;
    packet->flag = flag;
}
//读取struct packet类型的自定义读函数
int my_read(int connect_fd, struct packet *read_buf)
{
    int n;

    if ( (n = read_s(connect_fd, (char*)&read_buf->len, BUFSIZE_LEN)) != BUFSIZE_LEN)
        return n;
    if ( (n = read_s(connect_fd, (char*)&read_buf->flag, BUFSIZE_FLAG)) != BUFSIZE_FLAG)
        return n;
    if ( (n = read_s(connect_fd, read_buf->buf, read_buf->len)) != read_buf->len)
        return n;
    return n + BUFSIZE_FLAG + BUFSIZE_LEN;
}
//写入struct packet类型的自定义写函数
int my_write(int connect_fd, struct packet write_buf)
{
    return write_s(connect_fd, (char*)&write_buf, BUFSIZE_FLAG + BUFSIZE_LEN + write_buf.len);
}
//获取随机int值(用于生成账号)
int get_random_int()
{
    srand( (unsigned int)time(NULL) );
    int ret;
    char query[64];
    MYSQL_RES *result;
    while (1)
    {
        //至少10位数
        ret = ACCOUNT_MIN + rand() % (INT_MAX - ACCOUNT_MIN);
        sprintf(query, "SELECT * FROM USER_INFO WHERE account = %d", ret);

        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
        if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
        pthread_mutex_unlock(&mysql_lock);
        //查询不到说明唯一
        if (mysql_num_rows(result) == 0)
            return ret;
    }
}
