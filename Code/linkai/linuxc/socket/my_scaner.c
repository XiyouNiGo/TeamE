//端口扫描器
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <errno.h>
//端口信息
typedef struct port_segment
{
    struct in_addr dest_ip;         //目标ip
    unsigned short int min_port;    //起始端口
    unsigned short int max_port;    //最大端口
}port_segment;

void my_err(const char *str)
{
     perror(str);
     exit(1);
}
//扫描某ip地址上某端口
int do_scan(struct sockaddr_in server_addr)
{
    int connect_fd;
    int ret;

    if ( (connect_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        my_err("socket error");
    if ( (ret = connect(connect_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0) )
    {
        //目标端口未打开
        if (errno == ECONNREFUSED)
        {
            close(connect_fd);
            return 0;
        }
        else
        {
            close(connect_fd);
            return -1;
        }
    }
    else if (ret == 0)
    {
        printf("Port %d found in %s\n", ntohs(server_addr.sin_port),
               inet_ntoa(server_addr.sin_addr));
        close(connect_fd);
        return 1;
    }
}
//执行扫描的线程
void *scaner(void *arg)
{
    int i;
    struct sockaddr_in server_addr;
    port_segment port_info;

    memcpy(&port_info, arg, sizeof(struct port_segment));
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = port_info.dest_ip.s_addr;

    for (i = port_info.min_port; i <= port_info.max_port; i++)
    {
        server_addr.sin_port = htons(i);
        if (do_scan(server_addr) < 0)
            continue;
    }
    return NULL;
}
//-m:最大端口 -a:目标主机ip地址　-n:最大线程数
int main(int argc, char *argv[])
{
    pthread_t *thread;
    int max_port;   //最大端口号
    int thread_num; //最大线程数
    int segmen_len; //端口区间长度
    struct in_addr dest_ip;
    int i;

    if (argc != 7)
    {
        printf("Usage: [-m] [max_port] [-a] [server_addr] [-n] [thread_num]\n");
        exit(1);
    }

    for (i = 1; i < argc; i++)
    {
        if (strcmp("-m", argv[i]) == 0)
        {
            max_port = atoi(argv[i + 1]);
            if (max_port < 0 || max_port > 65535)
            {
                printf("Usage: invalid max dest port\n");
                exit(1);
            }
            continue;
        }
        if (strcmp("-a", argv[i]) == 0)
        {
            if (inet_aton(argv[i + 1], &dest_ip) == 0)
            {
                printf("Usage: invalid dest ip address\n");
                exit(1);
            }
            continue;
        }
        if (strcmp("-n", argv[i]) == 0)
        {
            thread_num = atoi(argv[i + 1]);
            if (thread_num <= 0)
            {
                printf("Usage: invalid thread_number\n");
                exit(1);
            }
            break;
        }
    }
    if (max_port < thread_num)
        thread_num = max_port;
    segmen_len = max_port / thread_num;
    if ( (max_port % thread_num) != 0)
        thread_num++;

    thread = (pthread_t*)malloc(thread_num * sizeof(pthread_t));
    for (i = 0; i < thread_num; i++)
    {
        port_segment port_info;
        port_info.dest_ip = dest_ip;
        port_info.min_port = i * segmen_len + 1;
        if (i == thread_num - 1)
            port_info.max_port = max_port;
        else
            port_info.max_port = port_info.min_port + segmen_len - 1;

        if (pthread_create(&thread[i], NULL, scaner, (void*)&port_info) != 0)
            my_err("pthread_create error");

        pthread_join(thread[i], NULL);
    }
    
    return 0;
}
