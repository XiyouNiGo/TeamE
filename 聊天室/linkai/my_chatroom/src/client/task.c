#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../inc/client/task.h"
#include "../../inc/client/client_config.h"
#include "../../inc/client/interface.h"

char account[20];

char uid[10];

struct packet ret_pack_buf;

int pchat_ing;

int gchat_ing;

int chat_uid;

int chat_gid;

void register_user(int connect_fd)
{
    char temp[256];
    struct packet send_pack;
    int len;
    memset(send_pack.buf, 0, 1024);

    printf("请输入昵称:\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    printf("请输入性别:\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");
    
    printf("请输入生日,格式为xxxx-xx-xx:\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    printf("请输入所在地:\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    printf("请输入签名:\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    printf("请输入密保问题:\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    printf("请输入密保答案:\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    printf("请输入密码:\n");
    secret_pwd(temp);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    if ((len = strlen(send_pack.buf)) > 1024)
    {
        printf("输入总和超过最大长度，请重新注册\n");
        return;
    }

    bale_packet(&send_pack, len, REGISTER_USER);
    my_write(connect_fd, send_pack);

    pthread_mutex_lock(&ret_lock);
    pthread_cond_wait(&ret_come, &ret_lock);
    pthread_mutex_unlock(&ret_lock);
}
void login_user(int connect_fd)
{
    char temp[256];
    struct packet send_pack;
    int len;
    memset(send_pack.buf, 0, 1024);

    printf("请输入账号:\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    printf("请输入密码:\n");
    secret_pwd(temp);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    if ((len = strlen(send_pack.buf)) > 1024)
    {
        printf("输入总和超过最大长度，请重新登录\n");
        return;
    }

    bale_packet(&send_pack, len, LOGIN_USER);
    my_write(connect_fd, send_pack);

    pthread_mutex_lock(&ret_lock);
    pthread_cond_wait(&ret_come, &ret_lock);
    pthread_mutex_unlock(&ret_lock);

    if (ret_pack_buf.flag == LOGIN_USER)
        chat_menu(connect_fd);
}
//重置密码(由于采用md5加密,不支持找回密码)
void reset_pwd(int connect_fd)
{
    char temp[256];
    struct packet send_pack;
    int len;
    char *output[1];
    int num;
    char *pack_str = ret_pack_buf.buf;
    memset(send_pack.buf, 0, 1024);

    printf("请输入账号(不会有人不记得账号吧...):\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    if ((len = strlen(send_pack.buf)) > 1024)
    {
        printf("输入总和超过最大长度，请重新登录\n");
        return;
    }

    bale_packet(&send_pack, len, RESET_PWD);
    my_write(connect_fd, send_pack);

    pthread_mutex_lock(&ret_lock);
    pthread_cond_wait(&ret_come, &ret_lock);
    pthread_mutex_unlock(&ret_lock);

    if (ret_pack_buf.flag == RESET_PWD)
    {
        parse_string(pack_str, output, &num);
        printf("密保问题：%s\n", output[0]);
        printf("请输入答案:");
        memset(send_pack.buf, 0, 1024);
        s_gets(temp, 256);
        strcat(send_pack.buf, temp);
        strcat(send_pack.buf, "\036");

        if ((len = strlen(send_pack.buf)) > 1024)
        {
            printf("输入总和超过最大长度，请重新登录\n");
            return;
        }

        bale_packet(&send_pack, len, RESET_PWD);
        my_write(connect_fd, send_pack);

        pthread_mutex_lock(&ret_lock);
        pthread_cond_wait(&ret_come, &ret_lock);
        pthread_mutex_unlock(&ret_lock);
        //可以开始发送新密码了
        if (ret_pack_buf.flag == RESET_PWD)
        {
            printf("请输入新密码：\n");
            memset(send_pack.buf, 0, 1024);
            secret_pwd(temp);
            strcat(send_pack.buf, temp);
            strcat(send_pack.buf, "\036");

            if ((len = strlen(send_pack.buf)) > 1024)
            {
                printf("输入总和超过最大长度，请重新输入\n");
                return;
            }

            bale_packet(&send_pack, len, RESET_PWD);
            my_write(connect_fd, send_pack);

            printf("重置成功！\n");
        }
        else
        {
            printf("密保答案错误！\n");
        }
    }
    else
    {
        printf("账号不存在！\n");
    }
}

void private_chat(int connect_fd)
{
    char temp[256];
    char prefix[256];
    memset(prefix, 0, 256);

    strcat(prefix, uid);
    strcat(prefix, "\036");

    printf("请输入好友的uid：\n");
    s_gets(temp, 256);
    chat_uid = atoi(temp);
    strcat(prefix, temp);
    strcat(prefix, "\036");

    private_chat_loop(prefix, connect_fd);
}

void add_friend_req(int connect_fd)
{
    struct packet send_pack;
    int len;
    char temp[256];
    memset(send_pack.buf, 0, 1024);
    
    strcat(send_pack.buf, uid);
    strcat(send_pack.buf, "\036");

    printf("请输入所加用户的uid：\n");
    s_gets(temp, 256);
    if (strcmp(uid, temp) == 0)
    {
        printf("好家伙，你自己加自己\n");
        return;
    }
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    printf("请输入验证消息：\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    if ((len = strlen(send_pack.buf)) > 1024)
    {
        printf("输入总和超过最大长度，请重新输入\n");
        return;
    }

    bale_packet(&send_pack, len, ADD_FRIEND_REQ);
    my_write(connect_fd, send_pack);

    pthread_mutex_lock(&ret_lock);
    pthread_cond_wait(&ret_come, &ret_lock);
    pthread_mutex_unlock(&ret_lock);
}

void add_friend_res(int connect_fd)
{
    struct packet send_pack;
    struct packet queue_pack;
    int len;
    int num;
    char *output[3];
    char temp[256];
    char *pack_str;
    memset(send_pack.buf, 0, 1024);
    pthread_mutex_lock(&queue_lock);
    //uid、昵称和验证消息
    DeQueue(Q, &queue_pack);
    pthread_mutex_unlock(&queue_lock);
    pack_str = queue_pack.buf;

    parse_string(pack_str, output, &num);

    strcat(send_pack.buf, output[0]);
    strcat(send_pack.buf, "\036");

    strcat(send_pack.buf, uid);
    strcat(send_pack.buf, "\036");

    printf("%s向你发来了好友申请，并说:%s\n", output[1], output[2]);
    printf("是否同意(1:同意,0:拒绝)：\n");
    
    sprintf(temp, "%d", get_choice_int(0, 1));
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    if ((len = strlen(send_pack.buf)) > 1024)
    {
        printf("输入总和超过最大长度，请重新输入\n");
        return;
    }

    bale_packet(&send_pack, len, ADD_FRIEND_RES);
    my_write(connect_fd, send_pack);
}

void del_friend(int connect_fd)
{
    struct packet send_pack;
    int len;
    char temp[256];
    memset(send_pack.buf, 0, 1024);
    
    printf("请输入需要删除的好友uid\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    strcat(send_pack.buf, uid);
    strcat(send_pack.buf, "\036");

    if ((len = strlen(send_pack.buf)) > 1024)
    {
        printf("输入总和超过最大长度，请重新输入\n");
        return;
    }

    bale_packet(&send_pack, len, DEL_FRIEND);
    my_write(connect_fd, send_pack);

    pthread_mutex_lock(&ret_lock);
    pthread_cond_wait(&ret_come, &ret_lock);
    pthread_mutex_unlock(&ret_lock);
}

void inq_friend(int connect_fd)
{
    struct packet send_pack;
    int len;
    char temp[256];
    memset(send_pack.buf, 0, 1024);
    
    printf("请输入查看资料的好友uid\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    if ((len = strlen(send_pack.buf)) > 1024)
    {
        printf("输入总和超过最大长度，请重新输入\n");
        return;
    }

    bale_packet(&send_pack, len, INQ_FRIEND);
    my_write(connect_fd, send_pack);

    pthread_mutex_lock(&ret_lock);
    pthread_cond_wait(&ret_come, &ret_lock);
    pthread_mutex_unlock(&ret_lock);
}

void list_friend(int connect_fd)
{
    struct packet send_pack;
    int len;
    memset(send_pack.buf, 0, 1024);
    
    strcat(send_pack.buf, uid);
    strcat(send_pack.buf, "\036");

    len = strlen(send_pack.buf);

    bale_packet(&send_pack, len, LIST_FRIEND);
    my_write(connect_fd, send_pack);

    pthread_mutex_lock(&ret_lock);
    pthread_cond_wait(&ret_come, &ret_lock);
    pthread_mutex_unlock(&ret_lock);
}

void shield_friend(int connect_fd)
{
    struct packet send_pack;
    int len;
    char temp[256];
    memset(send_pack.buf, 0, 1024);

    strcat(send_pack.buf, uid);
    strcat(send_pack.buf, "\036");
    
    printf("请输入屏蔽好友的uid\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    if ((len = strlen(send_pack.buf)) > 1024)
    {
        printf("输入总和超过最大长度，请重新输入\n");
        return;
    }

    bale_packet(&send_pack, len, SHIELD_FRIEND);
    my_write(connect_fd, send_pack);

    pthread_mutex_lock(&ret_lock);
    pthread_cond_wait(&ret_come, &ret_lock);
    pthread_mutex_unlock(&ret_lock);
}

void unshield_friend(int connect_fd)
{
    struct packet send_pack;
    int len;
    char temp[256];
    memset(send_pack.buf, 0, 1024);

    strcat(send_pack.buf, uid);
    strcat(send_pack.buf, "\036");
    
    printf("请输入解除屏蔽好友的uid\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    if ((len = strlen(send_pack.buf)) > 1024)
    {
        printf("输入总和超过最大长度，请重新输入\n");
        return;
    }

    bale_packet(&send_pack, len, UNSHIELD_FRIEND);
    my_write(connect_fd, send_pack);

    pthread_mutex_lock(&ret_lock);
    pthread_cond_wait(&ret_come, &ret_lock);
    pthread_mutex_unlock(&ret_lock);
}

void private_record(int connect_fd)
{
    char temp[256];
    struct packet send_pack;
    int len;
    memset(send_pack.buf, 0, 1024);

    strcat(send_pack.buf, uid);
    strcat(send_pack.buf, "\036");

    printf("请输入好友uid:\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    if ((len = strlen(send_pack.buf)) > 1024)
    {
        printf("输入总和超过最大长度，请重新创建\n");
        return;
    }

    bale_packet(&send_pack, len, PRIVATE_RECORD);
    my_write(connect_fd, send_pack);

    pthread_mutex_lock(&ret_lock);
    pthread_cond_wait(&ret_come, &ret_lock);
    pthread_mutex_unlock(&ret_lock);
}

void group_record(int connect_fd)
{
    char temp[256];
    struct packet send_pack;
    int len;
    memset(send_pack.buf, 0, 1024);

    strcat(send_pack.buf, uid);
    strcat(send_pack.buf, "\036");

    printf("请输入群聊gid:\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    if ((len = strlen(send_pack.buf)) > 1024)
    {
        printf("输入总和超过最大长度，请重新创建\n");
        return;
    }

    bale_packet(&send_pack, len, GROUP_RECORD);
    my_write(connect_fd, send_pack);

    pthread_mutex_lock(&ret_lock);
    pthread_cond_wait(&ret_come, &ret_lock);
    pthread_mutex_unlock(&ret_lock);
}


void group_chat(int connect_fd)
{
    char temp[256];
    char prefix[256];
    memset(prefix, 0, 256);

    strcat(prefix, uid);
    strcat(prefix, "\036");

    printf("请输入群聊的gid：\n");
    s_gets(temp, 256);
    chat_gid = atoi(temp);
    strcat(prefix, temp);
    strcat(prefix, "\036");

    group_chat_loop(prefix, connect_fd);
}

void create_group(int connect_fd)
{
    char temp[256];
    struct packet send_pack;
    int len;
    memset(send_pack.buf, 0, 1024);

    strcat(send_pack.buf, account);
    strcat(send_pack.buf, "\036");

    printf("请输入群名称:\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    printf("请输入群介绍:\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    if ((len = strlen(send_pack.buf)) > 1024)
    {
        printf("输入总和超过最大长度，请重新创建\n");
        return;
    }

    bale_packet(&send_pack, len, CREATE_GROUP);
    my_write(connect_fd, send_pack);

    printf("创建成功!\n");
} 

void dismiss_group(int connect_fd)
{
    char temp[256];
    struct packet send_pack;
    int len;
    memset(send_pack.buf, 0, 1024);

    strcat(send_pack.buf, account);
    strcat(send_pack.buf, "\036");

    printf("请输入gid:\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    if ((len = strlen(send_pack.buf)) > 1024)
    {
        printf("输入总和超过最大长度，请重新输入\n");
        return;
    }

    bale_packet(&send_pack, len, DISMISS_GROUP);
    my_write(connect_fd, send_pack);

    pthread_mutex_lock(&ret_lock);
    pthread_cond_wait(&ret_come, &ret_lock);
    pthread_mutex_unlock(&ret_lock);
}

void apply_group_req(int connect_fd)
{
    struct packet send_pack;
    int len;
    char temp[256];
    memset(send_pack.buf, 0, 1024);
    
    strcat(send_pack.buf, uid);
    strcat(send_pack.buf, "\036");

    printf("请输入所加群聊的gid：\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    printf("请输入验证消息：\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    if ((len = strlen(send_pack.buf)) > 1024)
    {
        printf("输入总和超过最大长度，请重新输入\n");
        return;
    }

    bale_packet(&send_pack, len, APPLY_GROUP_REQ);
    my_write(connect_fd, send_pack);

    pthread_mutex_lock(&ret_lock);
    pthread_cond_wait(&ret_come, &ret_lock);
    pthread_mutex_unlock(&ret_lock);
}

void apply_group_res(int connect_fd)
{
    struct packet send_pack;
    struct packet queue_pack;
    int len;
    int num;
    char *output[5];    //uid、gid、同意不同意
    char temp[256];
    char *pack_str;
    memset(send_pack.buf, 0, 1024);
    pthread_mutex_lock(&queue_lock);
    //uid、昵称、gid、grpname和验证消息
    DeQueue(Q, &queue_pack);
    pthread_mutex_unlock(&queue_lock);
    pack_str = queue_pack.buf;

    parse_string(pack_str, output, &num);

    strcat(send_pack.buf, output[0]);
    strcat(send_pack.buf, "\036");

    strcat(send_pack.buf, output[2]);
    strcat(send_pack.buf, "\036");

    printf("%s申请加入群聊%s\n", output[1], output[3]);
    printf("是否同意%s的加群申请(1:同意,0:拒绝)：\n", output[1]);
    
    sprintf(temp, "%d", get_choice_int(0, 1));
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    if ((len = strlen(send_pack.buf)) > 1024)
    {
        printf("输入总和超过最大长度，请重新输入\n");
        return;
    }
    //发回uid、gid、状态
    bale_packet(&send_pack, len, APPLY_GROUP_RES);
    my_write(connect_fd, send_pack);
}

void quit_group(int connect_fd)
{
    struct packet send_pack;
    int len;
    char temp[256];
    memset(send_pack.buf, 0, 1024);
    
    strcat(send_pack.buf, uid);
    strcat(send_pack.buf, "\036");

    printf("请输入将退出的群聊gid\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    if ((len = strlen(send_pack.buf)) > 1024)
    {
        printf("输入总和超过最大长度，请重新输入\n");
        return;
    }

    bale_packet(&send_pack, len, QUIT_GROUP);
    my_write(connect_fd, send_pack);

    pthread_mutex_lock(&ret_lock);
    pthread_cond_wait(&ret_come, &ret_lock);
    pthread_mutex_unlock(&ret_lock);
}

void list_group_all(int connect_fd)
{
    struct packet send_pack;

    bale_packet(&send_pack, 0, LIST_GROUP_ALL);
    my_write(connect_fd, send_pack);

    pthread_mutex_lock(&ret_lock);
    pthread_cond_wait(&ret_come, &ret_lock);
    pthread_mutex_unlock(&ret_lock);
}

void list_group_mine(int connect_fd)
{
    struct packet send_pack;
    int len;
    memset(send_pack.buf, 0, 1024);
    
    strcat(send_pack.buf, uid);
    strcat(send_pack.buf, "\036");

    len = strlen(send_pack.buf);

    bale_packet(&send_pack, len, LIST_GROUP_MINE);
    my_write(connect_fd, send_pack);

    pthread_mutex_lock(&ret_lock);
    pthread_cond_wait(&ret_come, &ret_lock);
    pthread_mutex_unlock(&ret_lock);
}

void inq_group(int connect_fd)
{
    struct packet send_pack;
    int len;
    char temp[256];
    memset(send_pack.buf, 0, 1024);
    
    printf("请输入查询的群聊gid\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    if ((len = strlen(send_pack.buf)) > 1024)
    {
        printf("输入总和超过最大长度，请重新输入\n");
        return;
    }

    bale_packet(&send_pack, len, INQ_GROUP);
    my_write(connect_fd, send_pack);

    pthread_mutex_lock(&ret_lock);
    pthread_cond_wait(&ret_come, &ret_lock);
    pthread_mutex_unlock(&ret_lock);
}

void set_admin(int connect_fd)
{
    struct packet send_pack;
    int len;
    char temp[256];
    memset(send_pack.buf, 0, 1024);
    
    strcat(send_pack.buf, uid);
    strcat(send_pack.buf, "\036");

    printf("请输入设置管理的群聊gid\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    printf("请输入设置管理的用户uid\n");
    s_gets(temp, 256);
    if (strcmp(temp, uid) == 0)
    {
        printf("请不要设置自己...\n");
        return;
    }
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    if ((len = strlen(send_pack.buf)) > 1024)
    {
        printf("输入总和超过最大长度，请重新输入\n");
        return;
    }

    bale_packet(&send_pack, len, SET_ADMIN);
    my_write(connect_fd, send_pack);

    pthread_mutex_lock(&ret_lock);
    pthread_cond_wait(&ret_come, &ret_lock);
    pthread_mutex_unlock(&ret_lock);
}


void send_file_req(int connect_fd);

void send_file_res(int connect_fd);

void list_user(int connect_fd)
{
    struct packet send_pack;

    bale_packet(&send_pack, 0, LIST_USER);
    my_write(connect_fd, send_pack);

    pthread_mutex_lock(&ret_lock);
    pthread_cond_wait(&ret_come, &ret_lock);
    pthread_mutex_unlock(&ret_lock);
}

void quit_user(int connect_fd)
{
    struct packet send_pack;
    int len;
    memset(send_pack.buf, 0, 1024);

    strcat(send_pack.buf, account);
    strcat(send_pack.buf, "\036");
    
    len = strlen(send_pack.buf);

    bale_packet(&send_pack, len, QUIT_USER);
    my_write(connect_fd, send_pack);

    printf("注销成功!\n");
}
//将ret_pack置于缓冲区(其实也不算缓冲区
//不过因为单个线程来看是串行的，没接受
//到信息是阻塞的，没必要弄队列，也没必要
//加锁)
void put_ret_in_buf(struct packet ret_pack)
{
    ret_pack_buf.flag = ret_pack.flag;
    ret_pack_buf.len = ret_pack.len;
    strcpy(ret_pack_buf.buf, ret_pack.buf);
}
//登录成功后的菜单
void chat_menu(int connect_fd)
{
    int choice;

    printf("      %-20s%-20s\n", "1.  用户列表", "2.  好友列表");
    printf("      %-20s%-20s\n", "3.  添加好友", "4.  删除好友");
    printf("      %-20s%-20s\n", "5.  好友资料", "6.  发送文件");
    printf("      %-20s%-20s\n", "7.  私聊好友", "8.  私聊记录");
    printf("      %-20s%-20s\n", "9.  屏蔽好友", "10. 解除屏蔽");
    printf("      %-20s%-20s\n", "11. 修改资料", "12. 修改群资料");
    putchar('\n');
    printf("      %-20s%-20s\n", "13. 群聊列表", "14. 我的群聊");
    printf("      %-20s%-20s\n", "15. 申请加群", "16. 退出群聊");
    printf("      %-20s%-20s\n", "17. 创建群聊", "18. 群聊资料");
    printf("      %-20s%-20s\n", "19. 发送群聊", "20. 群聊记录");
    printf("      %-20s%-20s\n", "21. 解散群聊", "22. 设置管理");
    putchar('\n');
    printf("      %-20s%-20s\n", "50. 注销登录", "51. 处理消息");

    while ((choice = get_choice_int(1, 100)) != 100)
    {
        switch (choice)
        {
            case 1:
                list_user(connect_fd);
                break;
            case 2:
                list_friend(connect_fd);
                break;
            case 3:
                add_friend_req(connect_fd);
                break;
            case 4:
                del_friend(connect_fd);
                break;
            case 5:
                inq_friend(connect_fd);
                break;
            case 7:
                private_chat(connect_fd);
                break;
            case 8:
                private_record(connect_fd);
                break;
            case 9:
                shield_friend(connect_fd);
                break;
            case 10:
                unshield_friend(connect_fd);
                break;
            case 11:
                update_user_info(connect_fd);
                break;
            case 12:
                update_group_info(connect_fd);
                break;
            case 13:
                list_group_all(connect_fd);
                break;
            case 14:
                list_group_mine(connect_fd);
                break;
            case 15:
                apply_group_req(connect_fd);
                break;
            case 16:
                quit_group(connect_fd);
                break;
            case 17:
                create_group(connect_fd);
                break;
            case 18:
                inq_group(connect_fd);
                break;
            case 19:
                group_chat(connect_fd);
                break;
            case 20:
                group_record(connect_fd);
                break;
            case 21:
                dismiss_group(connect_fd);
                break;
            case 22:
                set_admin(connect_fd);
                break;
            case 50: 
                quit_user(connect_fd);
                break;
            case 51: 
                handle_message(connect_fd);
                break;
            default:
                printf("未完善或未开发\n");
                break;
        }
        if (choice == 50)
            break;
        printf("      %-20s%-20s\n", "1.  用户列表", "2.  好友列表");
        printf("      %-20s%-20s\n", "3.  添加好友", "4.  删除好友");
        printf("      %-20s%-20s\n", "5.  好友资料", "6.  发送文件");
        printf("      %-20s%-20s\n", "7.  私聊好友", "8.  私聊记录");
        printf("      %-20s%-20s\n", "9.  屏蔽好友", "10. 解除屏蔽");
        printf("      %-20s%-20s\n", "11. 修改资料", "12. 修改群资料");
        putchar('\n');
        printf("      %-20s%-20s\n", "13. 群聊列表", "14. 我的群聊");
        printf("      %-20s%-20s\n", "15. 申请加群", "16. 退出群聊");
        printf("      %-20s%-20s\n", "17. 创建群聊", "18. 群聊资料");
        printf("      %-20s%-20s\n", "19. 发送群聊", "20. 群聊记录");
        printf("      %-20s%-20s\n", "21. 解散群聊", "22. 设置管理");
        putchar('\n');
        printf("      %-20s%-20s\n", "50. 注销登录", "51. 处理消息");
    }
}
//主菜单
void main_menu(int connect_fd)
{
    int choice;

    printf("      %-20s%-20s\n      %-20s%-20s\n", "1. 注册账号", "2. 登录账号", "3. 重置密码", "4. 退出系统");

    while ((choice = get_choice_int(1, 4)) != 4)
    {
        switch (choice)
        {
            case 1:
                register_user(connect_fd);
                break;
            case 2:
                login_user(connect_fd);
                break;
            case 3:
                reset_pwd(connect_fd);
                break;
        }
        printf("      %-20s%-20s\n      %-20s%-20s\n", "1. 注册账号", "2. 登录账号", "3. 重置密码", "4. 退出系统");
    }
}
//处理消息
void handle_message(int connect_fd)
{
    int queue_len;
    int choice = 2;
    struct packet queue_pack;
    int flag;

    while ((queue_len = QueueLength(*Q)) != 0)
    {
        printf("消息队列有%d条信息", queue_len);
        printf("，输入-1退出处理, 0开始处理：\n");
        choice = get_choice_int(-1, 0);
        if (choice == 0) 
        {
            //消息处理
           pthread_mutex_lock(&queue_lock);
            flag = Q->data[Q->front].flag;
           pthread_mutex_unlock(&queue_lock);

            switch (flag)
            {
                case ADD_FRIEND_REQ:
                    add_friend_res(connect_fd);
                    break;
                case APPLY_GROUP_REQ:
                    apply_group_res(connect_fd);
                    break;
                case PRIVATE_CHAT:
                    private_chat_res(connect_fd);
                    break;
                case GROUP_CHAT:
                    group_chat_res(connect_fd);
                default:
                    printf("未知类型的数据包：%d！\n", flag);
                    break;
            }
        }
        else if (choice == -1)
            break;
    }
    if ((queue_len == 0) && (choice == 2))
        printf("队列为空！\n");
    if ((queue_len == 0) && (choice != 2))
        printf("消息已清空！\n");
}

void private_chat_loop(char *prefix, int connect_fd)
{
    char timestamp[24];
    char temp[64];
    int len;
    struct packet send_pack;

    printf("聊天界面(输入#退出)：\n");

    pchat_ing = TRUE;
    while (1)
    {
        get_timestamp(timestamp);
        sprintf(send_pack.buf, "%s", prefix);
        s_gets(temp, 256);
        strcat(send_pack.buf, temp);
        strcat(send_pack.buf, "\036");

        strcat(send_pack.buf, timestamp);
        strcat(send_pack.buf, "\036");

        if ((len = strlen(send_pack.buf)) > 1024)
        {
            printf("输入总和超过最大长度，请重新输入\n");
            pchat_ing = FALSE;
            return;
        }
        if (strcmp("#", temp) == 0)
        {
            pchat_ing = FALSE;
            return;
        }
        bale_packet(&send_pack, len, PRIVATE_CHAT);
        my_write(connect_fd, send_pack);
    }
}

void group_chat_loop(char *prefix, int connect_fd)
{
    char timestamp[24];
    char temp[64];
    int len;
    struct packet send_pack;

    printf("聊天界面(输入#退出)：\n");

    gchat_ing = TRUE;
    while (1)
    {
        get_timestamp(timestamp);
        sprintf(send_pack.buf, "%s", prefix);
        s_gets(temp, 256);
        strcat(send_pack.buf, temp);
        strcat(send_pack.buf, "\036");

        strcat(send_pack.buf, timestamp);
        strcat(send_pack.buf, "\036");

        if ((len = strlen(send_pack.buf)) > 1024)
        {
            printf("输入总和超过最大长度，请重新输入\n");
            gchat_ing = FALSE;
            return;
        }
        if (strcmp("#", temp) == 0)
        {
            gchat_ing = FALSE;
            return;
        }
        bale_packet(&send_pack, len, GROUP_CHAT);
        my_write(connect_fd, send_pack);
    }
}


void private_chat_res(int connect_fd)
{
    struct packet queue_pack;
    int num;
    char *output[4];
    char *pack_str;
    int choice;
    char prefix[64];
    pthread_mutex_lock(&queue_lock);
    //uid、昵称、聊天内容和时间
    DeQueue(Q, &queue_pack);
    pthread_mutex_unlock(&queue_lock);
    pack_str = queue_pack.buf;

    parse_string(pack_str, output, &num);

    printf("%s\n%s对你说:%s\n", output[3], output[1], output[2]);
    printf("是否回复(1:回复,0:忽略)：\n");
    
    choice = get_choice_int(0, 1);
    if (choice == 0)
        return;
    else
    {
        chat_uid = atoi(output[0]);
        sprintf(prefix, "%s\036%s\036", uid, output[0]);
        private_chat_loop(prefix, connect_fd);
    }
}

void group_chat_res(int connect_fd)
{
    struct packet queue_pack;
    int num;
    char *output[6];
    char *pack_str;
    int choice;
    char prefix[64];
    pthread_mutex_lock(&queue_lock);
    //uid、昵称、gid、grpname、聊天内容和时间
    DeQueue(Q, &queue_pack);
    pthread_mutex_unlock(&queue_lock);
    pack_str = queue_pack.buf;

    parse_string(pack_str, output, &num);

    printf("%s\n群聊%s中的%s对大家说:%s\n", output[5], output[3], output[1], output[4]);
    printf("是否回复(1:回复,0:忽略)：\n");

    choice = get_choice_int(0, 1);
    if (choice == 0)
        return;
    else
    {
        chat_gid = atoi(output[2]);
        sprintf(prefix, "%s\036%s\036", uid, output[2]);
        group_chat_loop(prefix, connect_fd);
    }
}

void update_user_info(int connect_fd)
{
    char temp[256];
    struct packet send_pack;
    int len;
    memset(send_pack.buf, 0, 1024);

    strcat(send_pack.buf, uid);
    strcat(send_pack.buf, "\036");

    printf("请输入昵称:\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    printf("请输入性别:\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");
    
    printf("请输入生日,格式为xxxx-xx-xx:\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    printf("请输入所在地:\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    if ((len = strlen(send_pack.buf)) > 1024)
    {
        printf("输入总和超过最大长度，请重新注册\n");
        return;
    }

    bale_packet(&send_pack, len, UPDATE_USER_INFO);
    my_write(connect_fd, send_pack);

    pthread_mutex_lock(&ret_lock);
    pthread_cond_wait(&ret_come, &ret_lock);
    pthread_mutex_unlock(&ret_lock);
}

void update_group_info(int connect_fd)
{
    char temp[256];
    struct packet send_pack;
    int len;
    memset(send_pack.buf, 0, 1024);

    strcat(send_pack.buf, uid);
    strcat(send_pack.buf, "\036");

    printf("请输入需修改的群聊gid:\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    printf("请输入群名称:\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");

    printf("请输入群介绍:\n");
    s_gets(temp, 256);
    strcat(send_pack.buf, temp);
    strcat(send_pack.buf, "\036");


    if ((len = strlen(send_pack.buf)) > 1024)
    {
        printf("输入总和超过最大长度，请重新注册\n");
        return;
    }

    bale_packet(&send_pack, len, UPDATE_GRP_INFO);
    my_write(connect_fd, send_pack);

    pthread_mutex_lock(&ret_lock);
    pthread_cond_wait(&ret_come, &ret_lock);
    pthread_mutex_unlock(&ret_lock);
}
