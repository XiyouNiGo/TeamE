
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
#include <pthread.h>

#define SERV_PORT 9527

void my_err(const char *str)
{
     perror(str);
     exit(1);
}

int main(int argc, char *argv[])
{
    int connect_fd;
    char buf[BUFSIZ];

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, "172.17.0.1", &server_addr.sin_addr.s_addr);

    if ( (connect_fd = socket(AF_INET, SOCK_STREAM, 0))  == -1)
    {
        my_err("socket error");
    }
    //将套接字和客户端建立连接
    if (connect(connect_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        my_err("connect error");
    }

    while (1)
    {
        write(connect_fd, "hello\n", 6);
        sleep(2);
        int ret = read(connect_fd, buf, sizeof(buf));
        //printf("ret = %d\n", ret);
        write(STDOUT_FILENO, buf, ret);
    }

    close(connect_fd);

    return 0;
}
