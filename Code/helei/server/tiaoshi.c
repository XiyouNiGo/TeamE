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
#include<ctype.h>
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
    int sock_fd ,conn_fd;
    int optval;
    int ret;
    socklen_t cli_len;
    struct sockaddr_in cli_addr,serv_addr;
    char recv_buf[128];
    sock_fd = socket(AF_INET,SOCK_STREAM,0);
    if(sock_fd < 0)
        my_err("socket",__LINE__);
    
    memset(&serv_addr,0,sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sock_fd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr_in)) < 0)
        my_err("bind",__LINE__);
    if(listen(sock_fd,LISTENQ) < 0)
        my_err("listen ",__LINE__);
    cli_len = sizeof(struct sockaddr_in);
    conn_fd = accept(sock_fd,(struct sockaddr *) &cli_addr,&cli_len);
    if(conn_fd < 0)
        my_err("accept",__LINE__);
    printf("accept a new client , ip : %s \n",inet_ntoa (cli_addr.sin_addr));
    while(1)
    {
        if((ret = recv(conn_fd,recv_buf,sizeof(recv_buf),0)) < 0 )
        {
            perror("recv");
            exit(1);
        }
        
        for(int i = 0; i< ret ;i++)
        {
            recv_buf[i] = toupper(recv_buf[i]);
        }
        send(conn_fd,recv_buf,strlen(recv_buf),0) ;

    }
}
