/*************************************************************************
	> File Name: epoll.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月14日 星期二 15时33分07秒
 ************************************************************************/

#include<stdio.h>



sockfd = socket (AF_INET,SOCK_STREAM,0);
memset(&addr,0,sizeof(addr))
addr.sin_family = AF_INET;
addr.sin_port = htons(2000);
addr.sin_addr.s_addr = INADDR_ANY;
bind(socked,(struct sockaddr *)&addr,sizeof(addr));
listen(sockfd,5);
for(int i = 0; i < 5; i++)
{
    memset(&client,0,sizeof(client));
    addrlen = sizeof(client);
    fds[i] = accept(sockfd,(struct sockaddr *)&client,&addrlen);
    if(fds[i] > max)
        max = fds[i];
}

//select 函数
while(1)
{
    //fd_set *rset;     rset是一个文件描述符集
    //里面存放1024个二进制数据
    FD_ZERO(&rset);
    for(i = 0;i < 5;i++)
    {
        FD_SET(fds[i],&rset);
        //如果fds[0] = 0,fds[1] =1,fds[2] =3,fds[3] = 4,fd[4] = 7
        //则存放在rset中为1 1 0 1 1 0 0 1 ............000000000...0000000     

    }
    puts("round again");
    select(max+1,&rset,NULL,NULL,NULL);
    //监视文件描述符的变化情况（读写或异常）
    //成功返回时会将未准备好的文件描述符清零
    for(i = 0;i<5;i++)
    {
        if(FD_ISSET(fds[i],&rset))
        //FD_ISSET 是宏，判断fds是否在文件描述符集rset中，如果在返回非零，否则返回0
        {
            memset(buffer,0,MAXBUF);
            read(fds[i],buffer,MAXBUF);
            puts(buffer);
        }
    }
}



//pollfd


struct pollfd{
    int fd;
    short events;   //需要在意的事件是什么 
    short revents;  //对events的回馈
};

for(i = 0 ; i< 5;i++)
{
    memset(&client,0,sizeof(client));
    addrlen = sizeof(client);
    pollfds[i].fd = accept(sockfd,(struct sockaddr *)&client,&addrlen);
    pollfds[i].events = POLLIN; //针对读入数据
}
sleep(1);
while(1)
{
    puts("round again");
    poll(pollfds,5,50000);
    for(i = 0; i < 5;i++)
    {
        if(pollfds[i].revents & POLLIN)
        //有数据，置位revents
        {
            pollfds[i].revents = 0;
            memset(buffer,0,MAXBUF);
            read(pollfds[i].fd,buffer,MAXBUF);
            puts(buffer);
        }
    }
}

//epoll
//

struct epoll_event events[5];
int rpfd = rpoll_create(10);    //10没有什么意义，随便放值
...........
............
for(int i = 0 ; i<5;i++)
{
    static struct epoll_event ev;
    memset(&client,0,sizeof(client));
    addrlen = sizeof(client);
    ev.data.fd = accept(sockfd,(struct sockaddr * ) & client,&addrlen);
    ev.events = EPOLLIN;
    epoll_ctl (epfd,EPOLL_CTL_ADD,ev.data.fd,&ev);
    //将ev.data.fd 中保存的数据循环写入epfd
    //epfd
    //-> fd-events 
    //-> fd-events
    //-> fd-events
    //-> fd-events
    //-> fd-events
}


while(1)
{
    puts("round again");
    nfds = epoll_wait(rpfd,events,5,10000);
    //当有数据时，epoll_wait函数会将文件描述符进行重排，返回文件描述符有变化的个数
    for(i = 0 ; i < nfds; i++)
    {
        memset(buffer,0,MAXBUF);
        read(events[i].data.fd,buffer,MAXBUF);
        puts(buffer);
    }
}
