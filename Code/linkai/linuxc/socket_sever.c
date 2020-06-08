
/*************************************************************************
  > File Name: socket_sever.c
  > Author: NiGo
  > Mail: nigo@xiyoulinux.org
  > Created Time: 2020年06月08日 星期一 22时25分16秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <ctype.h>

#define SERV_PORT 9527

void my_err(const char *str)
{
    perror(str);
    exit(1);
}

int main(int argc, char *argv[])
{
    int socket_fd, new_fd;
    char buf[BUFSIZ];
    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    socklen_t len_client = sizeof(client_addr);
    //创建套接字
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        my_err("socket error");
    }
    //绑定地址结构(IP & port)
    if (bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        my_err("bind error");
    }
    //设置最大监听客户端数量
    if (listen(socket_fd, 128) == -1)
    {
        my_err("listen error");
    }
    //阻塞等待客户端响应
    if ((new_fd = accept(socket_fd, (struct sockaddr*)&client_addr, &len_client)) == -1)
    {
        my_err("accept error");
    }

    while (1)
    {
        int ret = read(new_fd, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, ret);
        for (int i = 0; i < ret; i++)
        {
            buf[i] = toupper(buf[i]);
        }

        write(new_fd, buf, ret);
    }

    close(socket_fd);
    close(new_fd);

    return 0;
}
