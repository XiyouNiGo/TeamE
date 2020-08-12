#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include "../../inc/client/interface.h"
#include "../../inc/client/list.h"
#include "../../inc/client/md5.h"
#include "../../inc/client/client_config.h"

int main(int argc, char *argv[])
{
    int connect_fd;
    struct packet write_buf;

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr);

    if ( (connect_fd = socket(AF_INET, SOCK_STREAM, 0))  == -1)
    {
        my_err("socket error");
    }
    //将套接字和服务器建立连接
    if (connect(connect_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        my_err("connect error");
    }

    while (1)
    {
        bale_packet(&write_buf, 4, 1);
        strcpy(write_buf.buf,"hhh");
        my_write(connect_fd, write_buf);
        sleep(3);
    }

    close(connect_fd);

    return 0;
}
