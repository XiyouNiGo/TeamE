#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include "my_recv.h"

void my_err(const char *str, int line)
{
    fprintf(stderr, "Line: %d ", line);
    perror(str);
}

void my_err_exit(const char *str, int line)
{
    fprintf(stderr, "Line: %d ", line);
    perror(str);
    exit(1);
}
//从套接字上读取一次数据(以'\n'为结尾)
int my_recv(int connect_fd, char *data_buf, int len)
{
    static char recv_buf[BUFSIZE];  //自定义缓冲区
    static char *p_read;            //指向下一次读取数据位置
    static int len_remain = 0;      //缓冲区剩余字节数
    int i;
    //如果缓冲区无数据，从套接字读取
    if (len_remain <= 0)
    {
        if ( (len_remain = recv(connect_fd, recv_buf, sizeof(recv_buf), 0)) < 0)
             my_err_exit("recv error", __LINE__);
        if (len_remain == 0)
            return 0;
        p_read = recv_buf;
    }
    //从自定义缓冲区读数据
    for (i = 0; *p_read != '\n' && len_remain > 0; i++)
    {
        //防止越界
        if (i > len)
            return -1;
        data_buf[i] = *p_read++;
        len_remain--;
    }
    //去除'\0'
    if (*p_read == '\0')
        len_remain--;
        p_read++;

    return i;
}

