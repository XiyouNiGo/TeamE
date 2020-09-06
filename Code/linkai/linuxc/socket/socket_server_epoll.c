#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <sys/epoll.h>
#define MAXLINE 8192
#define SERV_PORT 8000
#define OPEN_MAX 5000

void my_err(const char *str)
{
     perror(str);
     exit(1);
}

#define BUFSIZE_FLAG 4

#define BUFSIZE_LEN 4

struct packet
{
    int len;
    int flag;
    char buf[1024];
};

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

    if ( (n = read_s(connect_fd, (char*)(&read_buf->len), 4)) != 4)
        return n;

    if ( (n = read_s(connect_fd, (char*)(&read_buf->flag), 4)) != 4)
        return n;
    if ( (n = read_s(connect_fd, (char*)read_buf->buf, read_buf->len)) != read_buf->len)
        return n;
    /*if ( (n = read_s(connect_fd, (char*)read_buf, read_buf->len + BUFSIZE_LEN + BUFSIZE_FLAG)) != read_buf->len)
        return n;*/
    return n + BUFSIZE_FLAG + BUFSIZE_LEN;
}
//写入struct packet类型的自定义写函数
int my_write(int connect_fd, struct packet write_buf)
{
    return write_s(connect_fd, (char*)&write_buf, BUFSIZE_FLAG + BUFSIZE_LEN + write_buf.len);
}

int main(int argc, char *argv[])
{
    int i, j, listen_fd, connect_fd, sock_fd;
    int n, num = 0;
    int opt = 1;
    int efd, ret, num_ready;
    socklen_t len_client = sizeof(struct sockaddr_in);
    struct sockaddr_in client_addr, server_addr;
    struct epoll_event temp;        //epoll_ctl()
    struct epoll_event ep[OPEN_MAX];//epoll_wait()
    char buf[MAXLINE];
    struct packet send_pack;

    if ( (listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        my_err("socket error");
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);    
    if (bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in))== -1)
        my_err("bind error");
    if (listen(listen_fd, 20) == -1)
        my_err("listen error");
    if ( (efd = epoll_create(OPEN_MAX)) == -1)
        my_err("epoll_create error");
    temp.events = EPOLLIN;  //默认LT模式
    temp.data.fd = listen_fd;
    if ( (ret = epoll_ctl(efd, EPOLL_CTL_ADD, listen_fd, &temp)) == -1)
        my_err("epoll_ctl error");
    while (1)
    {
        if ( (num_ready = epoll_wait(efd, ep, OPEN_MAX, -1)) == -1)  //-1:永久阻塞
            my_err("epoll_wait error");
        for (i = 0; i < num_ready; i++)
        {
            if ( !(ep[i].events & EPOLLIN) )    //若不是读事件(可省略)
                continue;
            if (ep[i].data.fd == listen_fd)
            {
                if ( (connect_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &len_client)) == -1)
                    my_err("accept error");
                //长时间不用则关闭socket
                setsockopt(connect_fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt));
                printf("Receive from %s at Port %d\n",
                       inet_ntoa(client_addr.sin_addr),
                       ntohs(client_addr.sin_port));
                printf("Client %d: connect_fd %d\n", ++num, connect_fd);
                temp.events = EPOLLIN;  //包括对端异常关闭
                temp.data.fd = connect_fd;
                if ( (ret = epoll_ctl(efd, EPOLL_CTL_ADD, connect_fd, &temp)) == -1)
                    my_err("epoll error");
            }
            else    //connect_fd
            {
                sock_fd = ep[i].data.fd;
                //读到0说明客户端关闭
                if ( (n = my_read(sock_fd, &send_pack)) == 0)
                {
                    if ( (ret = epoll_ctl(efd, EPOLL_CTL_DEL, sock_fd, NULL)) < 0)
                        my_err("epoll_ctl error");
                    if ( (ret = close(sock_fd)) < 0)
                        my_err("close error");
                    printf("Client %d: close connection\n", connect_fd);
                }
                else if (n < 0)
                {
                    if ( (ret = epoll_ctl(efd, EPOLL_CTL_DEL, sock_fd, NULL)) < 0)
                        my_err("epoll_ctl error");
                    if ( (ret = close(sock_fd)) < 0)
                        my_err("close error");
                    my_err("read error");
                }
                else
                {
                    printf("len:%d\n", send_pack.len);
                    printf("flag:%d\n", send_pack.flag);
                    printf("buf:%s\n", send_pack.buf);
                    //printf("%s\n", buf);
                }
            }
         }
    }
    if ( (ret = close(listen_fd)) < 0)
        my_err("close error");
    return 0;
}
