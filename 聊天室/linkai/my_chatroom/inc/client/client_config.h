#ifndef _CLIENT_CONFIG_H_
#define _CLIENT_CONFIG_H_

#include <pthread.h>
#include "queue.h"
#include "packet_type.h"

#define TRUE 1
#define FALSE 0

#define ONLINE 1
#define OFFLINE 0

#define BUFSIZE_FLAG 4
#define BUFSIZE_LEN 4
#define BUFSIZE_PACK sizeof(struct packet)

#define SERV_PORT 8306

#define FRIEND 1
#define UID1_MUTE 2
#define UID2_MUTE 4

#define TEST_FUNC 0              //用于测试

#define LOGIN_USER 1        //用户登录
#define REGISTER_USER 2     //用户注册
#define RESET_PWD 3         //重置密码

#define PRIVATE_CHAT 4      //私聊
#define ADD_FRIEND_REQ 5    //添加好友 
#define ADD_FRIEND_RES 6    //添加好友(回复)
#define DEL_FRIEND 7        //删除好友
#define INQ_FRIEND 8        //查询好友
#define LIST_FRIEND 9       //好友列表
#define SHIELD_FRIEND 10    //屏蔽好友
#define UNSHIELD_FRIEND 25  //解除屏蔽
#define PRIVATE_RECORD 11   //私聊记录
#define UPDATE_USER_INFO 29 //修改个人资料

#define GROUP_CHAT 12       //群聊
#define GROUP_RECORD 28     //群聊记录
#define CREATE_GROUP 13     //创建群聊
#define DISMISS_GROUP 14    //解散群聊
#define APPLY_GROUP_REQ 15  //申请加群
#define APPLY_GROUP_RES 16  //申请加群(回复)
#define QUIT_GROUP 17       //退出群聊
#define LIST_GROUP_ALL 18   //所有群聊列表
#define LIST_GROUP_MINE 19  //我的群聊列表
#define INQ_GROUP 20        //查询群聊
#define SET_ADMIN 26        //设置群管理
#define UPDATE_GRP_INFO 27  //修改群资料

#define SEND_FILE_REQ 21    //发送文件
#define SEND_FILE_RES 22    //发送文件(回复)

#define LIST_USER 23        //列出所有用户
#define QUIT_USER 24        //退出登录

#define DEFAULT "DEFAULT"

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

/*struct packet
{
    int len;
    int flag;
    char buf[1024];
}packet;*/

extern pthread_mutex_t ret_lock;
extern pthread_cond_t ret_come;

extern pthread_mutex_t queue_lock;

void get_timestamp(char *timestamp);

void write_log(const char *log);

int read_s(int connect_fd, char *read_buf, int count);

int write_s(int connect_fd, char *write_buf, int count);

void bale_packet(struct packet *packet, int len, int flag);

int my_read(int connect_fd, struct packet *read_buf);

int my_write(int connect_fd, struct packet write_buf);

void parse_string(char *str, char **output, int *num);

void server_shutdown(int signo);

#endif
