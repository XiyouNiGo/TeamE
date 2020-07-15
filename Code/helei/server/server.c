/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月12日 星期日 12时12分58秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<pthread.h>
#include<ctype.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define SERV_PORT 9527
void sys_err(const char *str)
{
    perror(str);
    exit(1);
}
int main(void)
{
    int lfd = 0,cfd = 0;
    int ret;
    char buf[BUFSIZ];
    struct sockaddr_in serv_addr,clit_addr;
    lfd = socket(AF_INET,SOCK_STREAM,0);
    if(lfd == -1)
        sys_err("socket error");
    socklen_t clit_addr_len;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(lfd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr_in));
    listen(lfd,128);
    clit_addr_len = sizeof(struct sockaddr_in);
    cfd = accept(lfd, (struct sockaddr *) &clit_addr,&clit_addr_len);
    if(cfd == -1)
        sys_err("accept error");
    

    while(1)
    {
        ret = recv(cfd,buf,sizeof(buf),0);
        printf("%s\n",buf);
        for(int i = 0;i<ret;i++)
        {
            buf[i] = toupper(buf[i]);
        }
        send(cfd,buf,strlen(buf),0);


    }
    close(lfd);
    close(cfd);
    return 0;
}
