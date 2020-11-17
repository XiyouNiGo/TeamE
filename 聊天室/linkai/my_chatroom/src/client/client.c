#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include "../../inc/client/interface.h"
#include "../../inc/client/md5.h"
#include "../../inc/client/client_config.h"
#include "../../inc/client/queue.h"
#include "../../inc/client/task.h"


//读线程，负责接收数据
//并存放到全局队列
void *read_thread(void *arg)
{
    int connect_fd = *(int*)arg;
    struct packet ret_pack;
    int flag;
    char *output[100];
    char temp[256];
    int num;
    char *pack_str;
    while (1)
    {
        memset(ret_pack.buf, 0, 1024);
        memset(temp, 0, 256);
        if (my_read(connect_fd, &ret_pack) <= 0)
        {
            printf("服务器已关闭，期待下次服务\n");
            exit(0);
        }
        pack_str = (ret_pack).buf;
        flag = ret_pack.flag;
        switch (flag)
        {
            case REGISTER_USER:
            {
                //该函数会破坏字符串，具体处理应该放到具体函数中
                parse_string(pack_str, output, &num);
                printf("注册成功！您的账号为:%s\n", output[0]);
                pthread_cond_signal(&ret_come);
                break;
            }
            case LOGIN_USER:
            {
                put_ret_in_buf(ret_pack);
                parse_string(pack_str, output, &num);
                printf("登录成功!\n");
                strcpy(uid, output[0]);
                strcpy(account, output[1]);
                pthread_cond_signal(&ret_come);
                break;
            }
            case -LOGIN_USER:
            {
                put_ret_in_buf(ret_pack);
                parse_string(pack_str, output, &num);
                printf("账号不存在或密码错误!\n");
                pthread_cond_signal(&ret_come);
                break;
            }
            case LIST_USER:
            {
                parse_string(pack_str, output, &num);
                printf("用户列表:\n");
                printf("%-5s %-10s %-10s %-5s\n", "uid", "account", "nickname", "state");
                for (int i = 0; i < num / 4; i++)
                {
                    printf("%-5s ", output[i * 4]);
                    printf("%-10s ", output[i * 4 + 1]);
                    printf("%-10s ", output[i * 4 + 2]);
                    if (atoi(output[i * 4 + 3]) == ONLINE)
                        printf("%-5s\n", "在线");
                    else
                        printf("%-5s\n", "离线");
                }
                pthread_cond_signal(&ret_come);
                break;
            }
            case RESET_PWD:
            {
                put_ret_in_buf(ret_pack);

                pthread_cond_signal(&ret_come);
                break;
            }
            case -RESET_PWD:
            {
                put_ret_in_buf(ret_pack);
                
                pthread_cond_signal(&ret_come);
                break;
            }
            case DISMISS_GROUP:
            {
                printf("解散成功！\n");

                pthread_cond_signal(&ret_come);
                break;
            }
            case -DISMISS_GROUP:
            {
                printf("你无权解散！\n");

                pthread_cond_signal(&ret_come);
                break;
            }
            case INQ_FRIEND:
            {
                parse_string(pack_str, output, &num);
                printf("%s的个人资料：\n", output[2]);
                printf("uid：%s\n", output[0]);
                printf("账号：%s\n", output[1]);
                printf("性别：%s\n", output[3]);
                printf("破壳日：%s\n", output[4]);
                printf("位置：%s\n", output[5]);
                printf("签名：%s\n", output[6]);
                printf("状态:");
                if (atoi(output[7]) == ONLINE)
                    printf("在线\n");
                else
                    printf("离线\n");

                pthread_cond_signal(&ret_come);
                break;
            }
            case -INQ_FRIEND:
            {
                printf("请不要无中生友\n");

                pthread_cond_signal(&ret_come);
                break;
            }
            case LIST_FRIEND:
            {
                parse_string(pack_str, output, &num);
                if (num == 0)
                    printf("还没有好友，快去添加吧\n");
                else
                {
                    printf("好友列表:\n");
                    printf("%-5s %-10s %-10s %-5s\n", "uid", "account", "nickname", "state");
                    for (int i = 0; i < num / 4; i++)
                    {
                        printf("%-5s ", output[i * 4]);
                        printf("%-10s ", output[i * 4 + 1]);
                        printf("%-10s ", output[i * 4 + 2]);
                        if (atoi(output[i * 4 + 3]) == ONLINE)
                            printf("%-5s\n", "在线");
                        else
                            printf("%-5s\n", "离线");
                    }
                }
                pthread_cond_signal(&ret_come);
                break;
            }
            case QUIT_GROUP:
            {
                printf("退出成功！\n");

                pthread_cond_signal(&ret_come);
                break;
            }
            case -QUIT_GROUP:
            {
                printf("不在群聊中或为群主！\n");

                pthread_cond_signal(&ret_come);
                break;
            }
            case LIST_GROUP_ALL:
            {
                parse_string(pack_str, output, &num);
                if (num == 0)
                    printf("还没有群聊，试着自己创建一个吧\n");
                else
                {
                    printf("群聊列表：\n");
                    printf("%-5s %-10s\n", "gid", "群名称");
                    for (int i = 0; i < num / 2; i++)
                    {
                        printf("%-5s ", output[i * 2]);
                        printf("%-10s\n", output[i * 2 + 1]);
                    }
                }
                pthread_cond_signal(&ret_come);
                break;
            }
            case LIST_GROUP_MINE:
            {
                parse_string(pack_str, output, &num);
                if (num == 0)
                    printf("还没有群聊，试着加入一个吧\n");
                else
                {
                    printf("我的群聊列表：\n");
                    printf("%-5s %-10s\n", "gid", "群名称");
                    for (int i = 0; i < num / 2; i++)
                    {
                        printf("%-5s ", output[i * 2]);
                        printf("%-10s\n", output[i * 2 + 1]);
                    }
                }
                pthread_cond_signal(&ret_come);
                break;
            }
            case -INQ_GROUP:
            {
                printf("没有该群聊！\n");

                pthread_cond_signal(&ret_come);
                break;
            }
            case INQ_GROUP:
            {
                parse_string(pack_str, output, &num);
                printf("%s的群资料：\n", output[1]);
                printf("gid：%s\n", output[0]);
                printf("群介绍：%s\n", output[2]);
                printf("成员数量：%s\n", output[3]);

                pthread_cond_signal(&ret_come);
                break;
            }
            case -ADD_FRIEND_REQ:
            {
                printf("用户不存在或已添加！\n");

                pthread_cond_signal(&ret_come);
                break;
            }
            //申请成功或有人加你
            case ADD_FRIEND_REQ:
            {
                struct packet temp;
                temp = ret_pack;
                parse_string(pack_str, output, &num);
                if (num == 0)
                    printf("已向该用户发送申请！\n");
                else    //有人加
                {
                    pthread_mutex_lock(&queue_lock);
                    EnQueue(Q, temp);
                    pthread_mutex_unlock(&queue_lock);
                    printf("有人向你发送了加好友请求，快去处理消息吧！\n");
                }
                pthread_cond_signal(&ret_come);
                break;
            }
            case -ADD_FRIEND_RES:
            {
                parse_string(pack_str, output, &num);
                printf("%s拒绝了你的好友申请，我们来日方长！\n", output[0]);

                pthread_cond_signal(&ret_come);
                break;
            }
            case ADD_FRIEND_RES:
            {
                parse_string(pack_str, output, &num);

                printf("%s同意了你的好友申请，快去和Ta聊天吧！\n", output[0]);
                pthread_cond_signal(&ret_come);
                break;
            }
            case DEL_FRIEND:
            {
                printf("删除成功！\n");

                pthread_cond_signal(&ret_come);
                break;
            }
            case -DEL_FRIEND:
            {
                printf("没有这位好友！\n");

                pthread_cond_signal(&ret_come);
                break;
            }
            case -APPLY_GROUP_REQ:
            {
                printf("群聊不存在或已添加！\n");

                pthread_cond_signal(&ret_come);
                break;
            }
            case APPLY_GROUP_REQ:
            {
                struct packet pack_temp;
                pack_temp = ret_pack;
                parse_string(pack_str, output, &num);
                if (num == 0)
                    printf("已发送加群申请！\n");
                else    //有人加群
                {
                    pthread_mutex_lock(&queue_lock);
                    EnQueue(Q, pack_temp);
                    pthread_mutex_unlock(&queue_lock);
                    printf("有人向你发送了加群请求，快去处理消息吧！\n");
                }
                pthread_cond_signal(&ret_come);
                break;
            }
            case -APPLY_GROUP_RES:
            {
                parse_string(pack_str, output, &num);
                printf("群聊%s拒绝了你的加群申请，我们来日方长！\n", output[0]);

                pthread_cond_signal(&ret_come);
                break;
            }
            case APPLY_GROUP_RES:
            {
                parse_string(pack_str, output, &num);

                printf("群聊%s同意了你的加群申请，快去和大家聊天吧！\n", output[0]);
                pthread_cond_signal(&ret_come);
                break;
            }
            case SHIELD_FRIEND:
            {
                printf("屏蔽成功！\n");

                pthread_cond_signal(&ret_come);
                break;
            }
            case -SHIELD_FRIEND:
            {
                printf("没有这位好友！\n");

                pthread_cond_signal(&ret_come);
                break;
            }
            case UNSHIELD_FRIEND:
            {
                printf("解除屏蔽成功！\n");

                pthread_cond_signal(&ret_come);
                break;
            }
            case -UNSHIELD_FRIEND:
            {
                printf("没有这位好友！\n");

                pthread_cond_signal(&ret_come);
                break;
            }
            case SET_ADMIN:
            {
                printf("设置成功\n");

                pthread_cond_signal(&ret_come);
                break;
            }
            case -SET_ADMIN:
            {
                printf("没有权限或这个b不在群聊中！\n");

                pthread_cond_signal(&ret_come);
                break;
            }
            case -PRIVATE_RECORD:
            {
                printf("对方不是您的好友\n");

                pthread_cond_signal(&ret_come);
                break;
            }
            case PRIVATE_RECORD:
            {
                parse_string(pack_str, output, &num);
                if (num == 0)
                    printf("暂时没有聊天记录~\n");
                else
                {
                    printf("聊天记录：\n");
                    for (int i = 0; i < num / 4; i++)
                    {
                        sprintf(temp, "%s:%s", output[i * 4 + 3], output[i * 4 + 2]);
                        //发送者为对方，左侧展示
                        if (strcmp(uid, output[i * 4]) != 0)
                        {
                            printf("%-94s\n", output[i * 4 + 1]);
                            printf("%-94s\n", temp);
                        }
                        //反之，右侧
                        else
                        {
                            printf("%94s\n", output[i * 4 + 1]);
                            printf("%94s\n", temp);
                        }
                    }
                }
                pthread_cond_signal(&ret_come);
                break;
            }
            case -GROUP_RECORD:
            {
                printf("不在群聊中\n");

                pthread_cond_signal(&ret_come);
                break;
            }
            case GROUP_RECORD:
            {
                parse_string(pack_str, output, &num);
                if (num == 0)
                    printf("暂时没有聊天记录~\n");
                else
                {
                    printf("群聊记录：\n");
                    for (int i = 0; i < num / 4; i++)
                    {
                        sprintf(temp, "%s:%s", output[i * 4 + 3], output[i * 4 + 2]);
                        //发送者为其他人，左侧展示
                        if (strcmp(uid, output[i * 4]) != 0)
                        {
                            printf("%-94s\n", output[i * 4 + 1]);
                            printf("%-94s\n", temp);
                        }
                        //反之，右侧
                        else
                        {
                            printf("%94s\n", output[i * 4 + 1]);
                            printf("%94s\n", temp);
                        }
                    }
                }
                pthread_cond_signal(&ret_come);
                break;
            }
            case PRIVATE_CHAT:
            {
                struct packet pack_temp;
                pack_temp = ret_pack;
                parse_string(pack_str, output, &num);
                if (num == 0)
                    printf("发送成功~\n");
                else
                {
                    if (pchat_ing == TRUE && chat_uid == atoi(output[0]))
                    {
                        sprintf(temp, "%s:%s\n", output[1], output[2]);
                        printf("%-94s\n", output[3]);
                        printf("%-94s\n", temp);
                    }
                    else
                    {
                        pthread_mutex_lock(&queue_lock);
                        EnQueue(Q, pack_temp);
                        pthread_mutex_unlock(&queue_lock);
                        printf("%s向你发送了一条消息，快去处理吧！\n", output[1]);
                    }
                }
                break;
            }
            case -PRIVATE_CHAT:
            {
                printf("不是好友！请输入#退出！\n");
                break;
            }
            case GROUP_CHAT:
            {
                struct packet pack_temp;
                pack_temp = ret_pack;
                parse_string(pack_str, output, &num);
                if (num == 0)
                    printf("发送成功~\n");
                else
                {
                    if (gchat_ing == TRUE && chat_gid == atoi(output[2]))
                    {
                        sprintf(temp, "%s:%s\n", output[1], output[4]);
                        printf("%-94s\n", output[5]);
                        printf("%-94s\n", temp);
                    }
                    else
                    {
                        pthread_mutex_lock(&queue_lock);
                        EnQueue(Q, pack_temp);
                        pthread_mutex_unlock(&queue_lock);
                        printf("群聊%s中%s发送了一条消息，快去处理吧！\n", output[3], output[1]);
                    }
                }
                break;
            }
            case -GROUP_CHAT:
            {
                printf("不在群聊中!请输入#退出\n");
                break;
            }
            case UPDATE_USER_INFO:
            {
                printf("修改成功！\n");
                pthread_cond_signal(&ret_come);
                break;
            }
            case UPDATE_GRP_INFO:
            {
                printf("修改成功！\n");
                pthread_cond_signal(&ret_come);
                break;
            }
            case -UPDATE_GRP_INFO:
            {
                printf("无权限！\n");
                pthread_cond_signal(&ret_come);
                break;
            }
            default:
                printf("未知类型的数据包:%d\n", flag);
                break;
        }
    }
}
//写线程，负责发送数据
//同时负责显示
void *write_thread(void *arg)
{
    int connect_fd = *(int*)arg;
    main_menu(connect_fd);
    printf("Bye bye!\n");
    exit(0);
}

int main(int argc, char *argv[])
{
    initial_animation();
    pthread_t rid, wid;
    int connect_fd;
    pchat_ing = FALSE;
    gchat_ing = FALSE;

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    if (inet_pton(AF_INET, "47.94.14.45", &server_addr.sin_addr.s_addr) != 1)
        my_err("inet_pton error");
    //inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr);

    if ( (connect_fd = socket(AF_INET, SOCK_STREAM, 0))  == -1)
    {
        my_err("socket error");
    }
    CLOSE();
    printf("连接中......\n");
    //将套接字和服务器建立连接
    if (connect(connect_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        my_err("服务器还没开呢......\n");
    }
    printf("连接成功！\n");
    //初始化消息队列
    Q = (SqQueue*)malloc(sizeof(SqQueue));
    InitQueue(Q);
    //创建读线程和写线程
    pthread_create(&rid, NULL, read_thread, (void*)(&connect_fd));
    pthread_create(&wid, NULL, write_thread, (void*)(&connect_fd));
    pthread_join(rid, NULL);
    pthread_join(wid, NULL);

    return 0;
}
