
/*************************************************************************
    > File Name: socket_client.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年06月09日 星期二 10时01分49秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <cjson/cJSON.h>

#define SERV_PORT 8000

#define BUFSIZE_FLAG 4

#define BUFSIZE_LEN 4

struct packet
{
    int len;
    int flag;
    char buf[1024];
};

void my_err(const char *str)
{
     perror(str);
     exit(1);
}

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
                my_err("read_s error");
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
                my_err("write_s error");
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
    if ( (n = read_s(connect_fd, (char*)read_buf, read_buf->len + BUFSIZE_LEN + BUFSIZE_FLAG)) != read_buf->len)
        return n;
    return n + BUFSIZE_FLAG + BUFSIZE_LEN;
}
//写入struct packet类型的自定义写函数
int my_write(int connect_fd, struct packet write_buf)
{
    return write_s(connect_fd, (char*)&write_buf, BUFSIZE_FLAG + BUFSIZE_LEN + write_buf.len);
}

int main(int argc, char *argv[])
{
    int connect_fd;
    char buf[BUFSIZ] = "hhhh";
    struct packet send_pack;

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr);

    if ( (connect_fd = socket(AF_INET, SOCK_STREAM, 0))  == -1)
    {
        my_err("socket error");
    }
    //将套接字和客户端建立连接
    if (connect(connect_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        my_err("connect error");
    }

    strcpy(send_pack.buf, "jjjj");

    bale_packet(&send_pack, 4, 1);
    printf("len:%d\n", send_pack.len);
    printf("flag:%d\n", send_pack.flag);
    printf("buf:%s\n", send_pack.buf);
    my_write(connect_fd, send_pack);
    
    //write_s(connect_fd, buf, 4);

    close(connect_fd);

    return 0;
}
