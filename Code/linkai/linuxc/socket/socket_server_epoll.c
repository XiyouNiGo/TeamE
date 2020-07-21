#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
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
        for (i = 0; i < n; i++)
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
                if ( (n = read(sock_fd, buf, MAXLINE)) == 0)
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
                    for (j = 0; j < n; j++)
                        buf[j] = toupper(buf[i]);
                    write(STDOUT_FILENO, buf, n);
                    write(sock_fd, buf, n);
                }
            }
         }
    }
    if ( (ret = close(listen_fd)) < 0)
        my_err("close error");
    return 0;
}
