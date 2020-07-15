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
    int cfd;
    char buf[BUFSIZ];
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    //inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr.s_addr);
    inet_aton("127.0.0.1",&serv_addr.sin_addr);

    cfd = socket(AF_INET,SOCK_STREAM,0);
    if(cfd == -1)
        sys_err("socket error");
    int ret = connect(cfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    if(ret != 0)
        sys_err("connet error");
    
    while(1)
    {
        fgets(buf,sizeof(buf),stdin);
        buf[strlen(buf)-1] = '\0';
        send(cfd,buf,strlen(buf),0);
        recv(cfd,buf,sizeof(buf),0);
        printf("%s\n",buf);
        
       /* ret = read(cfd,&buf,sizeof(buf));
        write(STDOUT_FILENO,buf,ret);*/
    }
    close(cfd);
    return 0;

}
