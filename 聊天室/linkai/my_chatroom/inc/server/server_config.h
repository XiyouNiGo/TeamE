#ifndef _SERVER_CONFIG_H_
#define _SERVER_CONFIG_H_

#include <mysql/mysql.h>

#define TRUE 1
#define FALSE 0

#define OPEN 1
#define CLOSE 0

#define ONLINE 1
#define OFFLINE 0

#define BUFSIZE_LINE 256
#define BUFSIZE_WORD 64
#define BUFSIZE_READ 256
#define BUFSIZE_FLAG 4
#define BUFSIZE_LEN 4
#define BUFSIZE_PACK sizeof(struct packet)

#define SERV_PORT 8016
#define EPOLL_MAX 5000
#define LISTEN_MAX 20

#define MIN_TH_NUM 10
#define MAX_TH_NUM 50
#define QUEUESIZE_MAX 100

#define DEFAULT "DEFAULT"

#define ACCOUNT_MIN 1000000000

#define my_err(format)      { \
                                    fprintf(stderr, "(File:%s, Line:%d) ", __FILE__, __LINE__); \
                                    perror(format); \
                                    write_log(format); \
                                    exit(1); \
                            } \

#define report_err(format)  { \
                                    fprintf(stderr, "(File:%s, Line:%d) ", __FILE__, __LINE__); \
                                    perror(format); \
                                    write_log(format); \
                            } \
//mysql_real_connect()是不安全的
extern pthread_mutex_t mysql_connect_lock;
//共享句柄，query和store_result间需有互斥锁保证多线程安全
extern pthread_mutex_t mysql_lock;
//共享一个全局句柄
extern MYSQL mysql;

typedef void *(*pfunc)(void*, int);
    
struct packet
{
    int len;
    int flag;
    char buf[1024];
}packet;

void get_timestamp(char *timestamp);

int get_random_int();

void write_log(const char *log);

int read_s(int connect_fd, char *read_buf, int count);

int write_s(int connect_fd, char *write_buf, int count);

void bale_packet(struct packet *packet, int len, int flag);

int my_read(int connect_fd, struct packet *read_buf);

int my_write(int connect_fd, struct packet write_buf);

#endif
