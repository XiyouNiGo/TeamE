#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <sys/epoll.h>
#include <mysql/mysql.h>
#include <cjson/cJSON.h>
#include "../../inc/server/server_config.h"
#include "../../inc/server/thread_pool.h"

int main(int argc, char **argv)
{
    thread_pool_t *pool = create_thread_pool(MIN_TH_NUM, MAX_TH_NUM, QUEUESIZE_MAX);
    int i, listen_fd, connect_fd, sock_fd;
    int n, num = 0;
    int opt = 1;
    int efd, ret, num_ready;
    socklen_t len_client = sizeof(struct sockaddr_in);
    struct sockaddr_in client_addr, server_addr;
    struct epoll_event temp;        //epoll_ctl()
    struct epoll_event ep[EPOLL_MAX];//epoll_wait()
    //初始化锁
    pthread_mutex_init(&mysql_lock, NULL);
    //屏蔽信号
    sigset_t set;
	sigemptyset(&set);
	sigaddset(&set,SIGINT);	
	sigprocmask(SIG_BLOCK,&set,NULL);
    //设置无缓冲
    setbuf(stdout, NULL);
    //初始化mysql
    if (mysql_init(&mysql) == NULL)
    {
        my_err("mysql_init error");
    }
    if (mysql_library_init(0, NULL, NULL) < 0)
    {
        my_err("mysql_library_init error");
    }
    //连接服务器
    if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "chatroom", 0, NULL, 0) == NULL)
    {
        my_err("mysql_real_connect error");
    }
    //设置字符集
    if (mysql_set_character_set(&mysql, "utf8") < 0)
    {
        my_err("mysql_set_character_set error");
    }
    //设置所有用户为离线
    mysql_query(&mysql, "UPDATE user_info SET state = 0");
    //申请主动套接字
    if ( (listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        my_err("socket error");
    //设置端口复用属性
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    //设置服务器地址
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);    
    //绑定服务器地址
    if (bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in))== -1)
        my_err("bind error");
    //设置最大监听数
    if (listen(listen_fd, LISTEN_MAX) == -1)
        my_err("listen error");
    if ( (efd = epoll_create(EPOLL_MAX)) == -1)
        my_err("epoll_create error");
    temp.events = EPOLLIN | EPOLLET;
    temp.data.fd = listen_fd;
    if ( (ret = epoll_ctl(efd, EPOLL_CTL_ADD, listen_fd, &temp)) == -1)
        my_err("epoll_ctl error");
    while (1)
    {
        if ( (num_ready = epoll_wait(efd, ep, EPOLL_MAX, -1)) == -1)  //-1:永久阻塞
            my_err("epoll_wait error");
        for (i = 0; i < num_ready; i++)
        {
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
                temp.events = EPOLLIN | EPOLLET;  //包括对端正常关闭，改成边缘触发(具体接受数据大小已经应用层协议)
                temp.data.fd = connect_fd;
                if ( (ret = epoll_ctl(efd, EPOLL_CTL_ADD, connect_fd, &temp)) == -1)
                    my_err("epoll error");
            }
            else    //connect_fd
            {
                //使用堆区空间，解决同步问题
                struct packet *read_buf = (struct packet*)malloc(sizeof(struct packet));
                //接受客户端发来的信息并解析，并加入任务队列
                sock_fd = ep[i].data.fd;
                memset(read_buf, 0, BUFSIZE_PACK);
                //客户端正常关闭(read_s返回小于count的自然数)或异常关闭(触发EPOLLRDHUP)或read函数出错(非对方)
                if ( ep[i].events == EPOLLRDHUP || (n = my_read(sock_fd, read_buf)) < 0 || n != read_buf->len + BUFSIZE_FLAG + BUFSIZE_LEN)
                {
                    //出问题直接在此处free，否则在工作线程中free
                    free(read_buf);
                    if ( (ret = epoll_ctl(efd, EPOLL_CTL_DEL, sock_fd, NULL)) < 0)
                        my_err("epoll_ctl error");
                    if ( (ret = close(sock_fd)) < 0)
                        my_err("close error");
                    printf("Client %d: close connection\n", connect_fd);
                }
                else    //收包成功
                {
                    thread_pool_add_task(pool, read_buf, sock_fd);
                }
                //此处不需要再加入epoll事件
            }
         }
    }
    //关闭服务端
    if ( (ret = close(listen_fd)) < 0)
        my_err("close error");
    thread_pool_destory(pool);
    mysql_library_end();
    mysql_thread_end();
    return 0;
}
