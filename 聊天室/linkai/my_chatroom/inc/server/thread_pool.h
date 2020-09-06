#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <pthread.h>
#include "server_config.h"

#define DEFAULT_TIME 10
#define MAX_WAIT_TASK_NUM 10
#define DEFAULT_THREAD_NUM 10
//规定返回正值为成功或接受
//负值为失败或拒绝
#define ACCEPT 1
#define REFUSE -1

//任务列表
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


//消息类型
#define MSG_FILE 1          //文件
#define MSG_PRI_CHAT 2      //私聊
#define MSG_GRP_CHAT 3      //群聊
#define MSG_ADD_FRI_REQ 4   //添加好友
#define MSG_ADD_FRI_RES 5   //添加好友(回复)
#define MSG_APP_GRP_REQ 6   //申请加群
#define MSG_APP_GRP_RES 7   //申请加群(回复)
#define MSG_DIS_GRP 8       //解散群聊
#define MSG_QUI_GRP 9       //退出群聊

//消息状态
#define MSG_READ 1          //已读
#define MSG_SHIELD 2        //屏蔽
#define MSG_ACCEPT 4        //通过
#define MSG_DONE 8          //已完成(文件)

//关系
#define FRIEND 1
#define UID1_MUTE 2
#define UID2_MUTE 4

//群身份
#define LORD 1
#define MEMBER 2
#define ADMIN 3

//线程池任务
typedef struct thread_pool_task_t
{
    void *(*function)(void*, int);  //回调函数
    void *arg;                      //函数参数
    int connect_fd;                 //套接字
}thread_pool_task_t;
//线程池类型
typedef struct thread_pool_t
{
    //线程池信息
    int state;                      //线程池状态

    pthread_t admin_tid;            //管理者线程tid
    pthread_t *threads;             //供调用的线程tid

    int min_thread_num;             //最小线程数
    int max_thread_num;             //最大线程数
    int alive_thread_num;           //存活线程数
    int busy_thread_num;            //正在工作的线程数
    int wait_exit_thread_num;       //待销毁线程数

    pthread_mutex_t lock;           //全局锁
    pthread_mutex_t thread_counter; //忙线程数锁
    pthread_cond_t queue_not_full;  //任务队列非满
    pthread_cond_t queue_not_empty; //任务队列非空

    //任务队列信息
    thread_pool_task_t *task_queue; //队列
    int queue_front;                //队头
    int queue_rear;                 //队尾
    int queue_size;                 //队列当前大小
    int queue_max_size;             //队列最大容量
    
}thread_pool_t;

thread_pool_t *create_thread_pool(int min_thread_num, int max_thread_num, int queue_max_size);

void *worker_thread(void *thread_pool);

int thread_pool_add_task(thread_pool_t *pool, void *arg, int connect_fd);

int thread_pool_free(thread_pool_t *pool);

int thread_pool_destory(thread_pool_t *pool);

void *admin_thread(void *thread_pool);

int is_thread_alive(pthread_t tid);

pfunc thread_pool_get_func(int flag);

void *test_func(void *arg, int connect_fd);

void *register_user(void *arg, int connect_fd);

void parse_string(char *str, char **output, int *num);

void format_string(char *str, char *output[], int num);

void *login_user(void *arg, int connect_fd);

void *reset_pwd(void *arg, int connect_fd);

void *private_chat(void *arg, int connect_fd);

void *add_friend_req(void *arg, int connect_fd);

void *add_friend_res(void *arg, int connect_fd);

void *del_friend(void *arg, int connect_fd);

void *inq_friend(void *arg, int connect_fd);

void *list_friend(void *arg, int connect_fd);

void *shield_friend(void *arg, int connect_fd);

void *unshield_friend(void *arg, int connect_fd);

void *private_record(void *arg, int connect_fd);

void *group_record(void *arg, int connect_fd);

void *group_chat(void *arg, int connect_fd);

void *create_group(void *arg, int connect_fd);

void *dismiss_group(void *arg, int connect_fd);

void *apply_group_req(void *arg, int connect_fd);

void *apply_group_res(void *arg, int connect_fd);

void *quit_group(void *arg, int connect_fd);

void *list_group_all(void *arg, int connect_fd);

void *list_group_mine(void *arg, int connect_fd);

void *set_admin(void *arg, int connect_fd);

void *inq_group(void *arg, int connect_fd);

void *send_file_req(void *arg, int connect_fd);

void *send_file_res(void *arg, int connect_fd);

int get_uid_account(const char *account);

int get_nickname_account(const char *account, char *nickname);

int get_nickname_uid(const char *uid, char *nickname);

int get_cfd_account(const char *account);

int get_cfd_uid(const char *uid);

int is_user_online_account(const char *account);

int is_user_online_uid(const char *uid);

int get_grpname_gid(const char *gid, char *group_name);

void *list_user(void *arg, int connect_fd);

void *quit_user(void *arg, int connect_fd);

void *update_user_info(void *arg, int connect_fd);

void *update_group_info(void *arg, int connect_fd);

#endif
