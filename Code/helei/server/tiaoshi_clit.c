/*************************************************************************
	> File Name: tiaoshi.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月13日 星期一 15时55分11秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include <stdlib.h>

#define SERV_PORT 4507
#define LISTENQ 12

void my_err(char *string,int line)
{
    fprintf(stderr,"line : %d",line);
    perror(string);
    exit(-1);
}
void send_data(int conn_fd,const char *string)
{
    if(send(conn_fd,string,strlen(string),0) < 0)
        my_err("send",__LINE__);
}
int main()
{
    int i;
    int ret;
    int conn_fd;
//    int serv_port = atoi(SERV_PORT);
    int serv_port = SERV_PORT;
    struct sockaddr_in serv_addr;
    char recv_buf[BUFSIZ];
    memset(&serv_addr,0,sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(serv_port);
    if(inet_aton("127.0.0.1",&serv_addr.sin_addr) == 0)
    {
        printf("invalid server ip address\n");
        exit(1);
    }
    conn_fd = socket(AF_INET,SOCK_STREAM,0);
    if(conn_fd < 0)
    {
        my_err("socket",__LINE__);
    }
    if(connect(conn_fd,(struct sockaddr *) &serv_addr,sizeof(struct sockaddr)) < 0)
        my_err("connect ",__LINE__);
    while(1) 
    {
        printf("input the data: ");
        scanf("%s",recv_buf);
        send(conn_fd,recv_buf,strlen(recv_buf),0);
        ret = recv(conn_fd,recv_buf,sizeof(recv_buf),0);
        if(ret > 0)
        {
            printf("%s\n",recv_buf);
        }
    }
}

