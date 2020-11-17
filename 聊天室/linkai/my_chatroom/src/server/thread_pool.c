#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <mysql/mysql.h>
#include "../../inc/server/thread_pool.h"
#include "../../inc/server/server_config.h"
//创建线程池
thread_pool_t *create_thread_pool(int min_thread_num, int max_thread_num, int queue_max_size)
{
    int i;
    thread_pool_t *pool = NULL;
    //线程池初始化
    if ( (pool = (thread_pool_t*)malloc(sizeof(thread_pool_t))) == NULL)
        my_err("malloc error");
    pool->min_thread_num = min_thread_num;
    pool->max_thread_num = max_thread_num;
    pool->busy_thread_num = 0;
    pool->alive_thread_num = min_thread_num;
    pool->wait_exit_thread_num = 0;
    pool->queue_front = 0;
    pool->queue_rear = 0;
    pool->queue_size = 0;
    pool->queue_max_size = queue_max_size;
    pool->state = OPEN;
    if ( (pool->threads = (pthread_t*)malloc(sizeof(pthread_t) * max_thread_num)) == NULL)
        my_err("malloc error");
    memset(pool->threads, 0, sizeof(pthread_t) * max_thread_num);
    if ( (pool->task_queue = (thread_pool_task_t*)malloc(sizeof(thread_pool_task_t) * queue_max_size)) == NULL)
        my_err("malloc error");
    memset(pool->task_queue, 0, sizeof(thread_pool_task_t) * queue_max_size);
    if (pthread_mutex_init(&pool->lock, NULL) != 0)
        my_err("pthread_mutex_init error");
    if (pthread_mutex_init(&pool->thread_counter, NULL) != 0)
        my_err("pthread_mutex_init error");
    if (pthread_cond_init(&pool->queue_not_empty, NULL) != 0)
        my_err("pthread_cond_init error");
    if (pthread_cond_init(&pool->queue_not_full, NULL) != 0)
        my_err("pthread_cond_init error");
    //创建工作线程和管理者线程
    for (i = 0; i < min_thread_num; i++)
    {
        pthread_create(&pool->threads[i], NULL, worker_thread, (void*)pool);
        printf("Thread 0x%x has started\n", (unsigned int)pool->threads[i]);
    }
    pthread_create(&pool->admin_tid, NULL, admin_thread, (void*)pool);
    return pool;
}
//工作线程
void *worker_thread(void *thread_pool)
{
    thread_pool_t *pool = (thread_pool_t*)thread_pool;
    thread_pool_task_t task;
    pthread_mutex_lock(&mysql_connect_lock);
    mysql_init(&mysql);
    if (mysql_real_connect(&mysql, "47.94.14.45", "Linux_8306", "18150268306", "chat_room_8306", 0, NULL, 0) == NULL)
    //if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "chatroom", 0, NULL, 0) == NULL)
    {
        my_err("mysql_real_connect error");
    }
    if (mysql_set_character_set(&mysql, "utf8") < 0)
    {
        my_err("mysql_set_character_set error");
    }
    pthread_mutex_unlock(&mysql_connect_lock);
    while (1)
    {
        pthread_mutex_lock(&pool->lock);
        //无任务时阻塞(此时线程空闲)
        while ( (pool->queue_size == 0) && (pool->state == OPEN))
        {
            printf("Thread 0x%x is waiting\n", (unsigned int)pthread_self());
            pthread_cond_wait(&pool->queue_not_empty, &pool->lock);
            //判断是否需要退出线程
            if (pool->wait_exit_thread_num > 0 && pool->alive_thread_num > pool->min_thread_num)
            {
                pool->wait_exit_thread_num--;
                pool->alive_thread_num--;
                pthread_mutex_unlock(&pool->lock);
                printf("Thread 0x%x is exiting\n", (unsigned int)pthread_self());
                //调用pthread_exit()前先调用mysql_thread_end()释放线程相关变量
                mysql_thread_end();
                pthread_exit(NULL);
            }
        }
        //线程池处于关闭状态时退出
        if (pool->state == CLOSE)
        {
            pthread_mutex_unlock(&pool->lock);
            printf("Thread 0x%x is exiting\n", (unsigned int)pthread_self());
            mysql_thread_end();
            pthread_exit(NULL);
        }
        //否则线程取出任务(环形队列)
        task.function = pool->task_queue[pool->queue_front].function;
        task.arg = pool->task_queue[pool->queue_front].arg;
        task.connect_fd = pool->task_queue[pool->queue_front].connect_fd;
        pool->queue_front = (pool->queue_front + 1) % pool->queue_max_size;
        pool->queue_size--;
        //通知可以添加任务
        pthread_cond_broadcast(&pool->queue_not_full);
        pthread_mutex_unlock(&(pool->lock));
        //修改忙线程数
        pthread_mutex_lock(&pool->thread_counter);
        pool->busy_thread_num++;
        pthread_mutex_unlock(&pool->thread_counter);
        //执行任务
        printf("Thread 0x%x starts working\n", (unsigned int)pthread_self());
        (*(task.function))(task.arg, task.connect_fd);
        //结束任务
        printf("Thread 0x%x stops working\n", (unsigned int)pthread_self());
        pthread_mutex_lock(&pool->thread_counter);
        pool->busy_thread_num--;
        pthread_mutex_unlock(&pool->thread_counter);
    }
    mysql_thread_end();
    pthread_exit(NULL);
}
//向任务队列中添加任务
int thread_pool_add_task(thread_pool_t *pool, void *arg, int connect_fd)
{
    pfunc function = thread_pool_get_func(((struct packet*)arg)->flag);
    pthread_mutex_lock(&pool->lock);
    //队列已满时阻塞等待
    while (pool->queue_size == pool->queue_max_size && pool->state == OPEN)
    {
        pthread_cond_wait(&pool->queue_not_full, &pool->lock);
    }
    //线程池处于关闭状态时退出
    if (pool->state == CLOSE)
    {
        pthread_mutex_unlock(&pool->lock);
        return -1;
    }
    //清空任务参数
    if (pool->task_queue[pool->queue_rear].arg != NULL)
    {
        free(pool->task_queue[pool->queue_rear].arg);
        pool->task_queue[pool->queue_rear].arg = NULL;
    }
    //添加任务
    pool->task_queue[pool->queue_rear].function = function;
    pool->task_queue[pool->queue_rear].arg = arg;
    pool->task_queue[pool->queue_rear].connect_fd = connect_fd;
    pool->queue_rear = (pool->queue_rear + 1) % pool->queue_max_size;
    pool->queue_size++;
    //唤醒线程中一个线程
    pthread_cond_signal(&pool->queue_not_empty);
    pthread_mutex_unlock(&pool->lock);
    return 0;
}
int thread_pool_free(thread_pool_t *pool)
{
    if (pool == NULL)
        return -1;
    if (pool->task_queue)
        free(pool->task_queue);
    if (pool->threads)
        free(pool->threads);
    pthread_mutex_destroy(&pool->lock);
    pthread_mutex_destroy(&pool->thread_counter);
    pthread_cond_destroy(&pool->queue_not_empty);
    pthread_cond_destroy(&pool->queue_not_full);
    free(pool);
    return 0;
}
int thread_pool_destory(thread_pool_t *pool)
{
    int i;
    if (pool == NULL)
        return -1;
    pool->state = CLOSE;
    pthread_join(pool->admin_tid, NULL);
    for (i = 0; i < pool->alive_thread_num; i++)
        pthread_cond_broadcast(&pool->queue_not_empty);
    for (i = 0; i < pool->alive_thread_num; i++)
        pthread_join(pool->threads[i], NULL);
    thread_pool_free(pool);
    return 0;
}
//管理者线程
void *admin_thread(void *thread_pool)
{
    int i;
    thread_pool_t *pool = (thread_pool_t*)thread_pool;
    while (pool->state == OPEN)
    {
        //阻塞一段时间后管理(此时让出CPU)
        sleep(DEFAULT_TIME);
        //获取任务数和存活线程数
        pthread_mutex_lock(&pool->lock);
        int queue_size = pool->queue_size;
        int alive_thread_num = pool->alive_thread_num;
        pthread_mutex_unlock(&pool->lock);
        //获取忙线程数
        pthread_mutex_lock(&pool->thread_counter);
        int busy_thread_num = pool->busy_thread_num;
        pthread_mutex_unlock(&pool->thread_counter);
        printf("admin: busy %d, alive %d\n", busy_thread_num, alive_thread_num);
        //当任务队列里任务数量大于最大等待任务数时创建新线程
        if (queue_size >= MAX_WAIT_TASK_NUM && alive_thread_num < pool->max_thread_num)
        {
            int add_num = 0;    //已新增线程数
            pthread_mutex_lock(&pool->lock);
            //一次性增加DEFAULT_THREAD_NUM个线程
            for (i = 0; i < pool->max_thread_num && add_num < DEFAULT_THREAD_NUM && pool->alive_thread_num < pool->max_thread_num; i++)
            {
                //并不是每个线程号都能创建
                if (pool->threads[i] == 0 || is_thread_alive(pool->threads[i] == FALSE))
                {
                    pthread_create(&pool->threads[i], NULL, worker_thread, (void*)pool);
                    add_num++;
                    pool->alive_thread_num++;
                    printf("admin: add worker_thread\n");
                }
            }
            pthread_mutex_unlock(&pool->lock);
        }
        //销毁线程
        if ( (busy_thread_num * 2) < alive_thread_num && alive_thread_num > pool->min_thread_num)
        {
            //一次性销毁DEFAULT_THREAD_NUM个线程
            pthread_mutex_lock(&pool->lock);
            pool->wait_exit_thread_num = DEFAULT_THREAD_NUM;
            pthread_mutex_unlock(&pool->lock);
            //通知空闲进程
            for (i = 0; i < DEFAULT_THREAD_NUM; i++)
            {
                pthread_cond_signal(&pool->queue_not_empty);
            }
        }
    }
    return NULL;
}
//判断线程是否存活
int is_thread_alive(pthread_t tid)
{
    int ret = pthread_kill(tid, 0); //0号信号用于测试线程是否存活,类似ping
    if (ret == ESRCH)   //线程不存在
        return FALSE;
    return TRUE;
}
//根据flag获取任务
pfunc thread_pool_get_func(int flag)
{
    if (flag == TEST_FUNC)
        return test_func;
    if (flag == LOGIN_USER)
        return  login_user;
    if (flag == REGISTER_USER)
        return register_user; 
    if (flag == RESET_PWD)
        return reset_pwd;
    if (flag == PRIVATE_CHAT)
        return private_chat;
    if (flag == ADD_FRIEND_REQ)
        return add_friend_req;
    if (flag == ADD_FRIEND_RES)
        return add_friend_res;
    if (flag == DEL_FRIEND)
        return del_friend;
    if (flag == INQ_FRIEND)
        return inq_friend;
    if (flag == LIST_FRIEND)
        return list_friend;
    if (flag == SHIELD_FRIEND)
        return shield_friend;
    if (flag == UNSHIELD_FRIEND)
        return unshield_friend;
    if (flag == PRIVATE_RECORD)
        return private_record;
    if (flag == GROUP_RECORD)
        return group_record;
    if (flag == GROUP_CHAT)
        return group_chat;
    if (flag == CREATE_GROUP)
        return create_group;
    if (flag == DISMISS_GROUP)
        return dismiss_group;
    if (flag == APPLY_GROUP_REQ)
        return apply_group_req;
    if (flag == APPLY_GROUP_RES)
        return apply_group_res;
    if (flag == QUIT_GROUP)
        return quit_group;
    if (flag == LIST_GROUP_ALL)
        return list_group_all;
    if (flag == LIST_GROUP_MINE)
        return list_group_mine;
    if (flag == INQ_GROUP)
        return inq_group;
    if (flag == SEND_FILE_REQ)
        return send_file_req;
    if (flag == SEND_FILE_RES)
        return send_file_res;
    if (flag == LIST_USER)
        return list_user;
    if (flag == QUIT_USER)
        return quit_user;
    if (flag == SET_ADMIN)
        return set_admin;
    if (flag == UPDATE_USER_INFO)
        return update_user_info;
    if (flag == UPDATE_GRP_INFO)
        return update_group_info;
}
//用于测试
void *test_func(void *arg, int connect_fd)
{
    char *temp = (char*)arg;
    printf("%s\n", (char*)temp + BUFSIZE_FLAG + BUFSIZE_LEN);
    free(arg);
}
//注册函数
void *register_user(void *arg, int connect_fd)
{
    int num;
    char *output[8];
    char *pack_str = ((struct packet*)arg)->buf;
    int account;
    char query[256];
    char temp[256];
    //pack_str被strtok破坏
    parse_string(pack_str, output, &num);
    format_string(temp, output, num);
    account = get_random_int();
    
    sprintf(query, "INSERT INTO user_info(account, nickname, gender, birthday, "
                   "location, signature, security_q, security_a, password, connect_fd) "
                   "VALUES(%d, %s, %d)", account, temp, connect_fd);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
        my_err("mysql_query error");
    pthread_mutex_unlock(&mysql_lock);
    //返回账号
    struct packet ret_pack;
    sprintf(ret_pack.buf, "%d\036", account);
    bale_packet(&ret_pack, strlen(ret_pack.buf), REGISTER_USER);
    my_write(connect_fd, ret_pack);
    free(arg);
}
//解析字符串
void parse_string(char *str, char **output, int *num)
{
    char *temp;
    *num = 0;
    temp = (char*)strtok(str, "\036");
    while (temp)
    {
        *output++ = temp;
        (*num)++;   //括号必须加(结合律从右到左)
        temp = (char*)strtok(NULL, "\036");
    }
}
//格式化字符串(为query语句服务)
void format_string(char *str, char *output[], int num)
{
    int i;
    char temp[256];
    for (i = 0; i < num; i++)
    {
        if (i == 0)
        {
            sprintf(str, "'%s'", output[i]);
        }
        else
        {
            //若不用temp，结果是未定义的
            strcpy(temp, str);
            //若为DEFAULT，不加单引号
            if (strcmp(DEFAULT, output[i]))
                sprintf(str, "%s,'%s'", temp, output[i]);
            else
                sprintf(str, "%s,%s", temp, output[i]);
        }
    }
}
//登录函数
void *login_user(void *arg, int connect_fd)
{
    int num;
    char *output[2];    //0为账号,1为密码
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    char nickname[64];
    char grpname[64];
    MYSQL_RES *result;
    struct packet ret_pack;
    int count;
    MYSQL_ROW row;
    int flag = ACCEPT;
    int uid;
    parse_string(pack_str, output, &num);
    uid = get_uid_account(output[0]);
    sprintf(query, "SELECT * FROM user_info WHERE account = %s AND password = '%s'", \
            output[0], output[1]);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
        my_err("mysql_store_result error");
    if (mysql_num_rows(result) == 0)
    {
        flag = REFUSE;
        *(ret_pack.buf) = '\0';
    }
    else
    {
        MYSQL_ROW row = mysql_fetch_row(result);
        //更新用户状态
        sprintf(query, "UPDATE user_info SET state = %d WHERE account = '%s'", ONLINE, output[0]);
        if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
        sprintf(query, "UPDATE user_info SET connect_fd = %d WHERE account = %s", connect_fd, output[0]);
        if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
        //发回uid、account
        sprintf(ret_pack.buf, "%s\036%s\036", row[0], row[1]);
    }
    pthread_mutex_unlock(&mysql_lock);
    bale_packet(&ret_pack, strlen(ret_pack.buf), LOGIN_USER * flag);
    my_write(connect_fd, ret_pack);
    mysql_free_result(result);
    free(arg);

    //好友申请
    sprintf(query, "SELECT mid, send_uid, content FROM message WHERE type = %d "
            "AND recv_uid = %d", MSG_ADD_FRI_REQ, uid);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
        my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
        my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    //挨个发送并删除
    count = mysql_num_rows(result);
    for (int i = 0; i < count; i++)
    {
        row = mysql_fetch_row(result);
        get_nickname_uid(row[1], nickname);
        sprintf(ret_pack.buf, "%s\036%s\036%s\036", row[1], nickname, row[2]);
        bale_packet(&ret_pack, strlen(ret_pack.buf), ADD_FRIEND_REQ);
        my_write(connect_fd, ret_pack);

        sprintf(query, "DELETE FROM message WHERE mid = %s", row[0]);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
        pthread_mutex_unlock(&mysql_lock);
    }
    mysql_free_result(result);

    //加好友回复
    sprintf(query, "SELECT mid, send_uid, state FROM message WHERE type = %d AND recv_uid = %d",
            MSG_ADD_FRI_RES, uid);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
        my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
        my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    //挨个发送并删除
    count = mysql_num_rows(result);
    for (int i = 0; i < count; i++)
    {
        row = mysql_fetch_row(result);
        get_nickname_uid(row[1], nickname);

        if ((atoi(row[2]) & MSG_ACCEPT) != 0)
            flag = ACCEPT;
        else
            flag = REFUSE;

        sprintf(ret_pack.buf, "%s\036", nickname);
        bale_packet(&ret_pack, strlen(ret_pack.buf), flag * ADD_FRIEND_RES);
        my_write(connect_fd, ret_pack);

        sprintf(query, "DELETE FROM message WHERE mid = %s", row[0]);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
        pthread_mutex_unlock(&mysql_lock);
    }
    mysql_free_result(result);

    //加群申请
    sprintf(query, "SELECT mid, send_uid, content, gid FROM message WHERE type = %d "
            "AND recv_uid = %d", MSG_APP_GRP_REQ, uid);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
        my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
        my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    //挨个发送并删除
    count = mysql_num_rows(result);
    for (int i = 0; i < count; i++)
    {
        row = mysql_fetch_row(result);
        get_nickname_uid(row[1], nickname);
        get_grpname_gid(row[3], grpname);
        sprintf(ret_pack.buf, "%s\036%s\036%s\036%s\036%s\036", row[1], nickname, row[3], grpname, row[2]);
        bale_packet(&ret_pack, strlen(ret_pack.buf), APPLY_GROUP_REQ);
        my_write(connect_fd, ret_pack);

        sprintf(query, "DELETE FROM message WHERE mid = %s", row[0]);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
        pthread_mutex_unlock(&mysql_lock);
    }
    mysql_free_result(result);

    //加群回复
    sprintf(query, "SELECT mid, gid, state FROM message WHERE type = %d AND recv_uid = %d",
            MSG_APP_GRP_RES, uid);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
        my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
        my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    //挨个发送并删除
    count = mysql_num_rows(result);
    for (int i = 0; i < count; i++)
    {
        row = mysql_fetch_row(result);
        get_grpname_gid(row[1], grpname);

        if ((atoi(row[2]) & MSG_ACCEPT) != 0)
            flag = ACCEPT;
        else
            flag = REFUSE;

        sprintf(ret_pack.buf, "%s\036", grpname);
        bale_packet(&ret_pack, strlen(ret_pack.buf), flag * APPLY_GROUP_RES);
        my_write(connect_fd, ret_pack);

        sprintf(query, "DELETE FROM message WHERE mid = %s", row[0]);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
        pthread_mutex_unlock(&mysql_lock);
    }
    mysql_free_result(result);

    //私聊
    sprintf(query, "SELECT mid, send_uid, content, create_time FROM message WHERE type = %d AND recv_uid = %d "
            "AND (state & %d) = 0 AND (state & %d) != 0", MSG_PRI_CHAT, uid, MSG_READ, MSG_SHIELD);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
        my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
        my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    //挨个发送并设置已读
    count = mysql_num_rows(result);
    for (int i = 0; i < count; i++)
    {
        row = mysql_fetch_row(result);
        get_nickname_uid(row[1], nickname);

        sprintf(ret_pack.buf, "%s\036%s\036%s\036%s\036", row[1], nickname, row[2], row[3]);
        bale_packet(&ret_pack, strlen(ret_pack.buf), PRIVATE_CHAT);
        my_write(connect_fd, ret_pack);

        sprintf(query, "UPDATE message SET state = (state | %d) WHERE mid = %s", MSG_READ, row[0]);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
        pthread_mutex_unlock(&mysql_lock);
    }
    mysql_free_result(result);

    //群聊
    sprintf(query, "SELECT mid, send_uid, gid, content, create_time FROM message WHERE type = %d AND recv_uid = %d "
            "AND (state & %d) = 0", MSG_GRP_CHAT, uid, MSG_READ);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
        my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
        my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    //挨个发送并设置已读
    count = mysql_num_rows(result);
    for (int i = 0; i < count; i++)
    {
        row = mysql_fetch_row(result);
        get_nickname_uid(row[1], nickname);
        get_grpname_gid(row[2], grpname);

        sprintf(ret_pack.buf, "%s\036%s\036%s\036%s\036%s\036%s\036", row[1], nickname, row[2], grpname, row[3], row[4]);
        bale_packet(&ret_pack, strlen(ret_pack.buf), GROUP_CHAT);
        my_write(connect_fd, ret_pack);

        sprintf(query, "UPDATE message SET state = (state | %d) WHERE mid = %s", MSG_READ, row[0]);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
        pthread_mutex_unlock(&mysql_lock);
    }
}
//重置密码
void *reset_pwd(void *arg, int connect_fd)
{
    int num;
    char *output[1];    //账号、密保答案、新密码(分批发)
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    MYSQL_RES *result;
    struct packet ret_pack;
    char account[15];
    char security_q[64];
    char security_a[64];
    //第一次接受账号
    parse_string(pack_str, output, &num);
    strcpy(account, output[0]);

    sprintf(query, "SELECT security_q, security_a FROM user_info WHERE account = %s", account);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
        my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
        my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    //账号不存在返回失败
    if (mysql_num_rows(result) == 0)
    {
        bale_packet(&ret_pack, 0, -RESET_PWD);
        my_write(connect_fd, ret_pack);
    }
    else    //说明账号存在
    {
        MYSQL_ROW row = mysql_fetch_row(result);
        strcpy(security_q, row[0]);
        strcpy(security_a, row[1]);
        strcpy(ret_pack.buf, security_q);
        bale_packet(&ret_pack, strlen(ret_pack.buf), RESET_PWD);
        my_write(connect_fd, ret_pack);

        //接收答案
        my_read(connect_fd, &ret_pack);
        pack_str = ((struct packet*)&ret_pack)->buf;
        parse_string(pack_str, output, &num);
        //答案正确
        if (strcmp(output[0], security_a) == 0)
        {
            bale_packet(&ret_pack, 0, RESET_PWD);
            my_write(connect_fd, ret_pack);
            //接受新密码
            my_read(connect_fd, &ret_pack);
            pack_str = ((struct packet*)&ret_pack)->buf;
            parse_string(pack_str, output, &num);
            //更新密码
            sprintf(query, "UPDATE user_info SET password = '%s' WHERE account = %s", 
                    output[0], account);
            pthread_mutex_lock(&mysql_lock);
            if (mysql_query(&mysql, query) < 0)
                my_err("mysql_query error"); 
            pthread_mutex_unlock(&mysql_lock);
        }
        //答案错误
        else
        {
            bale_packet(&ret_pack, 0, -RESET_PWD);
            my_write(connect_fd, ret_pack);
        }
    }
    
    mysql_free_result(result);
    free(arg);
}
//私聊
void *private_chat(void *arg, int connect_fd)
{
    int num;
    char *output[4];    //两个uid，聊天内容,时间
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    MYSQL_RES *result;
    int flag = ACCEPT;
    int is_uid1 = 0;    //接收人是否是uid1
    struct packet ret_pack;
    memset(ret_pack.buf, 0, 1024);
    parse_string(pack_str, output, &num);
    sprintf(query, "SELECT uid1, relation FROM user_relation WHERE uid1 = %s AND uid2 = %s "
            "OR uid1 = %s AND uid2 = %s", output[0], output[1], output[1], output[0]);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
        my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    //不为好友
    if (mysql_num_rows(result) == 0)
    {
        flag = REFUSE;
        mysql_free_result(result);
    }
    //为好友
    else
    {
        flag = ACCEPT;
        MYSQL_ROW row = mysql_fetch_row(result);
        if (strcmp(row[0], output[1]) == 0)
            is_uid1 = TRUE;
        else
            is_uid1 = FALSE;
        if (atoi(row[1]) & FRIEND)
        {
            int state = 0;
            flag = ACCEPT;
            //查询是否屏蔽，若屏蔽则无法发送，只存入记录
            if ( (atoi(row[1]) & UID1_MUTE && is_uid1) || (atoi(row[1]) & UID2_MUTE && (!is_uid1)) )
            {
                state |= MSG_SHIELD;
            }
            else
            {
                //在线则发送消息并存入,离线直接存入
                if (is_user_online_uid(output[1]))
                {
                    state |= MSG_READ;
                    char nickname[64];
                    get_nickname_uid(output[0], nickname);
                    //uid、昵称和内容
                    printf("!!!:\n%s %s %s %s\n", output[0], nickname, output[2], output[3]);
                    sprintf(ret_pack.buf, "%s\036%s\036%s\036%s\036", output[0], nickname, output[2], output[3]);
                    bale_packet(&ret_pack, strlen(ret_pack.buf), PRIVATE_CHAT);
                    my_write(get_cfd_uid(output[1]), ret_pack);
                }
            }
            sprintf(query, "INSERT INTO message(type, send_uid, recv_uid, content, state) "
                    "VALUES(%d, %s, %s, '%s', %d)", MSG_PRI_CHAT, output[0], output[1], output[2], state);
            pthread_mutex_lock(&mysql_lock);
            if (mysql_query(&mysql, query) < 0)
                my_err("mysql_query error");
            pthread_mutex_unlock(&mysql_lock);
        }
        else
        {
            flag = REFUSE;
        }
        //mysql_free_result(result);
    }
    
    bale_packet(&ret_pack, 0, flag * PRIVATE_CHAT);
    my_write(connect_fd, ret_pack);
    mysql_free_result(result);
    free(arg);
}
//添加好友
void *add_friend_req(void *arg, int connect_fd)
{
    int num;
    char *output[3];    //两个uid和验证消息内容
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    MYSQL_RES *result;
    int flag = ACCEPT;
    struct packet ret_pack;
    char nickname[64];

    memset(ret_pack.buf, 0, 1024);

    parse_string(pack_str, output, &num);
    int recv_cfd = get_cfd_uid(output[1]);
    //获取账号和昵称
    get_nickname_uid(output[0], nickname);
    //查无此号
    if (recv_cfd == -1)
        flag = REFUSE;
    else
    {
        sprintf(query, "SELECT * FROM user_relation WHERE uid1 = %s AND uid2 = %s "
               "OR uid1 = %s AND uid2 = %s", output[0], output[1], output[1], output[0]);
        //若在黑名单中或已成为好友，返回失败
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
        if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
        pthread_mutex_unlock(&mysql_lock);
        if (mysql_num_rows(result) != 0)
        {
            flag = REFUSE;
            mysql_free_result(result);
        }
        //用户存在且没有建立关系
        else
        {
            flag = ACCEPT;
            mysql_free_result(result);
            sprintf(query, "SELECT state FROM user_info WHERE uid = %s", output[1]);
            pthread_mutex_lock(&mysql_lock);
            if (mysql_query(&mysql, query) < 0)
                my_err("mysql_query error");
            if ( (result = mysql_store_result(&mysql)) == NULL)
                my_err("mysql_store_result error");
            pthread_mutex_unlock(&mysql_lock);
            MYSQL_ROW row = mysql_fetch_row(result);
            //用户在线，发送消息(uid、昵称、验证消息)
            if (atoi(row[0]) == ONLINE)
            {
                sprintf(ret_pack.buf, "%s\036%s\036%s\036", output[0], nickname, output[2]);
                bale_packet(&ret_pack, strlen(ret_pack.buf), ADD_FRIEND_REQ);
                my_write(recv_cfd, ret_pack);
            }
            //否则存入离线消息
            else
            {
                int state = 0;
                sprintf(query, "INSERT INTO message(type, send_uid, recv_uid, content, state) "
                        "VALUES(%d, %s, %s, '%s', %d)", MSG_ADD_FRI_REQ, output[0], output[1], output[2], state);
                pthread_mutex_lock(&mysql_lock);
                if (mysql_query(&mysql, query) < 0)
                    my_err("mysql_query error");
                pthread_mutex_unlock(&mysql_lock);
            }
            mysql_free_result(result);
        }
    }
    
    bale_packet(&ret_pack, 0, ADD_FRIEND_REQ * flag);
    my_write(connect_fd, ret_pack);
    free(arg);
}
//添加好友(回复)
void *add_friend_res(void *arg, int connect_fd)
{
    int num;
    char *output[3];    //两个uid，一个状态(TRUE or FALSE)
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    int flag = REFUSE;
    struct packet ret_pack;
    char nickname[64];

    memset(ret_pack.buf, 0, 1024);
    parse_string(pack_str, output, &num);
    get_nickname_uid(output[1], nickname);
    //若通过，则在user_relation中加入信息
    if (atoi(output[2]) == TRUE)
    {
        flag = ACCEPT;
        sprintf(query, "INSERT INTO user_relation(uid1, uid2, relation) "
                "VALUES(%s, %s, %d)", output[0], output[1], FRIEND);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
                my_err("mysql_query error");
        pthread_mutex_unlock(&mysql_lock);
        //并删除数据
        sprintf(query, "DELETE FROM message WHERE send_uid = %s AND recv_uid = %s AND type = %d",
                output[0], output[1], MSG_ADD_FRI_REQ);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
                my_err("mysql_query error");
        pthread_mutex_unlock(&mysql_lock);
    }
    else
        flag = REFUSE;
    //如果在线，发送之(只发送昵称给申请者)
    if (is_user_online_uid(output[0]))
    {
        int recv_fd = get_cfd_uid(output[0]);
        strcpy(ret_pack.buf, nickname);
        bale_packet(&ret_pack, strlen(ret_pack.buf), flag * ADD_FRIEND_RES);
        my_write(recv_fd, ret_pack);
    }
    //不在线则存入离线消息
    else
    {
        int state = 0;
        if (atoi(output[2]) == TRUE)
            state |= MSG_ACCEPT;
        sprintf(query, "INSERT INTO message(type, send_uid, recv_uid, state) "
                       "VALUES(%d, %s, %s, %d)", MSG_ADD_FRI_RES, output[1], output[0], state);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
                my_err("mysql_query error");
        pthread_mutex_unlock(&mysql_lock);
    }
    free(arg);
}
//删除好友
void *del_friend(void *arg, int connect_fd)
{
    int num;
    char *output[2];    //两个uid
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    int flag;
    struct packet ret_pack;
    MYSQL_RES *result;

    memset(ret_pack.buf, 0, 1024);
    parse_string(pack_str, output, &num);
    //查询是否存在好友关系    
    sprintf(query, "SELECT * FROM user_relation WHERE uid1 = %s AND uid2 = %s "
            "OR uid1 = %s AND uid2 = %s", output[0], output[1], output[1], output[0]);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
        my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    if (mysql_num_rows(result) == 0)
    {
        flag = REFUSE;
    }
    else
    {
        flag = ACCEPT;
        sprintf(query, "DELETE FROM user_relation WHERE uid1 = %s AND uid2 = %s "
                "OR uid1 = %s AND uid2 = %s", output[0], output[1], output[1], output[0]);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
        pthread_mutex_unlock(&mysql_lock);
    }
    mysql_free_result(result);
    bale_packet(&ret_pack, 0, DEL_FRIEND * flag);
    my_write(connect_fd, ret_pack);

    free(arg);
}
//查询好友
void *inq_friend(void *arg, int connect_fd)
{
    int num;
    char *output[1];    //被查询者的uid
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    int flag;
    MYSQL_RES *result;
    struct packet ret_pack;

    parse_string(pack_str, output, &num);
    sprintf(query, "SELECT uid, account, nickname, gender, birthday, location, signature, state "
            " FROM user_info WHERE uid = %s", output[0]);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    //失败直接返回失败
    if (mysql_num_rows(result) == 0)
    {
        flag = REFUSE;
        *(ret_pack.buf) = '\0';
    }
    //成功则返回uid,账号，昵称，性别，生日，位置，签名，状态
    else
    {
        flag = ACCEPT;
        MYSQL_ROW row = mysql_fetch_row(result);
        memset(ret_pack.buf, 0, 1024);
        for (int i = 0; i < 8; i++)
        {
            strcat(ret_pack.buf, row[i]);
            strcat(ret_pack.buf, "\036");
        }
    }
    bale_packet(&ret_pack, strlen(ret_pack.buf), INQ_FRIEND * flag);
    my_write(connect_fd, ret_pack);
    mysql_free_result(result);
    free(arg);
}
//列出好友
void *list_friend(void *arg, int connect_fd)
{
    int num;
    char *output[1];    //uid
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    MYSQL_RES *result;
    struct packet ret_pack;
    parse_string(pack_str, output, &num);
    //找到好友
    sprintf(query, "SELECT uid1, uid2 FROM user_relation WHERE uid1 = %s OR uid2 = %s",
            output[0], output[0]);

    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    //返回uid, 账号, 昵称
    memset(ret_pack.buf, 0, 1024);
    int count = mysql_num_rows(result);
    for (int i = 0; i < count; i++)
    {
        MYSQL_ROW row = mysql_fetch_row(result);
        for (int j =0; j < 2; j++)
        {
            if (strcmp(row[j], output[0]) != 0)
            {
                MYSQL_RES *result_temp;
                sprintf(query, "SELECT uid, account, nickname, state FROM user_info WHERE uid = %s",
                        row[j]);
                pthread_mutex_lock(&mysql_lock);
                if (mysql_query(&mysql, query) < 0)
                    my_err("mysql_query error");
                if ( (result_temp = mysql_store_result(&mysql)) == NULL)
                    my_err("mysql_store_result error");
                pthread_mutex_unlock(&mysql_lock);
                for (int k = 0; k < (int)mysql_num_rows(result_temp); k++)
                {
                    MYSQL_ROW row_temp = mysql_fetch_row(result_temp);
                    for (int l = 0; l < 4; l++)
                    {
                        strcat(ret_pack.buf, row_temp[l]);
                        strcat(ret_pack.buf, "\036");
                    }
                }
            }
        }
    }
    bale_packet(&ret_pack, strlen(ret_pack.buf), LIST_FRIEND);
    my_write(connect_fd, ret_pack);
    mysql_free_result(result);
    free(arg);
}
//屏蔽好友
void *shield_friend(void *arg, int connect_fd)
{
    int num;
    char *output[2];    //两个uid
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    struct packet ret_pack;
    int uid1, uid2;
    MYSQL_ROW row;
    int flag;
    int mute;
    int rid;
    MYSQL_RES *result;

    parse_string(pack_str, output, &num);
    //好友是否存在
    sprintf(query, "SELECT rid, uid1 FROM user_relation  WHERE uid1 = %s AND uid2 = %s "
            "OR uid1 = %s AND uid2 = %s", output[0], output[1], output[1], output[0]);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
        my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
        my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    if (mysql_num_rows(result) == 0)
    {
        mysql_free_result(result);
        flag = REFUSE;
    }
    else
    {
        row = mysql_fetch_row(result);
        rid = atoi(row[0]);
        if (strcmp(output[0], row[1]) == 0)
            mute = UID1_MUTE;
        else
            mute = UID2_MUTE;
        mysql_free_result(result);
        flag = ACCEPT;
        //设置屏蔽关系
        sprintf(query, "UPDATE user_relation SET relation = (relation | %d) WHERE rid = %d",
                mute, rid);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
        pthread_mutex_unlock(&mysql_lock);
        
        //将消息设置为屏蔽
        sprintf(query, "UPDATE message SET state = (state | %d) WHERE recv_uid = %s",
                MSG_SHIELD, output[0]);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
        pthread_mutex_unlock(&mysql_lock);
    }
    bale_packet(&ret_pack, 0, flag * SHIELD_FRIEND);
    my_write(connect_fd, ret_pack);
    free(arg);
}
//解除屏蔽好友
void *unshield_friend(void *arg, int connect_fd)
{
    int num;
    char *output[2];    //两个uid
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    struct packet ret_pack;
    int uid1, uid2;
    MYSQL_ROW row;
    int flag;
    int mute;
    int rid;
    MYSQL_RES *result;

    parse_string(pack_str, output, &num);
    //好友是否存在
    sprintf(query, "SELECT rid, uid1 FROM user_relation  WHERE uid1 = %s AND uid2 = %s "
            "OR uid1 = %s AND uid2 = %s", output[0], output[1], output[1], output[0]);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
        my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
        my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    if (mysql_num_rows(result) == 0)
    {
        mysql_free_result(result);
        flag = REFUSE;
    }
    else
    {
        row = mysql_fetch_row(result);
        rid = atoi(row[0]);
        if (strcmp(output[0], row[1]) == 0)
            mute = UID1_MUTE;
        else
            mute = UID2_MUTE;
        mysql_free_result(result);
        flag = ACCEPT;
        //解除屏蔽
        sprintf(query, "UPDATE user_relation SET relation = (relation & %d) WHERE rid = %d",
                ~mute, rid);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
        pthread_mutex_unlock(&mysql_lock);

        //将消息解除屏蔽
        sprintf(query, "UPDATE message SET state = (state & %d) WHERE recv_uid = %s",
                ~MSG_SHIELD, output[0]);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
        pthread_mutex_unlock(&mysql_lock);
    }
    bale_packet(&ret_pack, 0, flag * UNSHIELD_FRIEND);
    my_write(connect_fd, ret_pack);
    free(arg);
}
//私聊记录
void *private_record(void *arg, int connect_fd)
{
    int num;
    char *output[2];    //两个uid
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    MYSQL_RES *result;
    int flag;
    char nickname[64];
    MYSQL_ROW row;
    struct packet ret_pack;
    memset(ret_pack.buf, 0, 1024);

    parse_string(pack_str, output, &num);
    sprintf(query, "SELECT * FROM user_relation WHERE uid1 = %s AND uid2 = %s "
            "OR uid1 = %s AND uid2 = %s", output[0], output[1], output[1], output[0]);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
        my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
        my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    //没有该好友
    if (mysql_num_rows(result) == 0)
    {
        flag = REFUSE;
        mysql_free_result(result);
    }
    else
    {
        flag = ACCEPT;
        mysql_free_result(result);
        sprintf(query, "SELECT send_uid, create_time, content FROM message "
                "WHERE (type = %d AND send_uid = %s AND recv_uid = %s) OR "
                "(type = %d AND send_uid = %s AND recv_uid = %s)", MSG_PRI_CHAT, \
                output[0], output[1], MSG_PRI_CHAT, output[1], output[0]);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
        if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
        pthread_mutex_unlock(&mysql_lock);
        int count = mysql_num_rows(result);
        for (int i = 0; i < count; i++)
        {
            //uid、时间、内容、昵称
            row = mysql_fetch_row(result);

            for (int j = 0; j < 3; j++)
            {
                strcat(ret_pack.buf, row[j]);
                strcat(ret_pack.buf, "\036");
            }
            get_nickname_uid(row[0], nickname);
            strcat(ret_pack.buf, nickname);
            strcat(ret_pack.buf, "\036");
        }
        mysql_free_result(result);
    }
    bale_packet(&ret_pack, strlen(ret_pack.buf), flag * PRIVATE_RECORD);
    my_write(connect_fd, ret_pack);
    free(arg);
}
//群聊记录
void *group_record(void *arg, int connect_fd)
{
    int num;
    char *output[2];    //uid、gid
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    MYSQL_RES *result;
    int flag;
    char nickname[64];
    MYSQL_ROW row;
    struct packet ret_pack;
    memset(ret_pack.buf, 0, 1024);

    parse_string(pack_str, output, &num);
    sprintf(query, "SELECT * FROM group_member WHERE uid = %s AND gid = %s ",
            output[0], output[1]);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
        my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
        my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    //不在群聊中
    if (mysql_num_rows(result) == 0)
    {
        flag = REFUSE;
        mysql_free_result(result);
    }
    else
    {
        flag = ACCEPT;
        mysql_free_result(result);
        sprintf(query, "SELECT send_uid, create_time, content FROM message "
                "WHERE (gid = %s AND type = %d AND recv_uid = %s) OR "
                "(gid = %s AND type = %d AND send_uid = %s)", output[1], MSG_GRP_CHAT, \
                output[0], output[1], MSG_GRP_CHAT, output[0]);
        printf("q:%s\n", query);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
        if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
        pthread_mutex_unlock(&mysql_lock);
        int count = mysql_num_rows(result);
        for (int i = 0; i < count; i++)
        {
            //uid、时间、内容、昵称
            row = mysql_fetch_row(result);

            for (int j = 0; j < 3; j++)
            {
                strcat(ret_pack.buf, row[j]);
                strcat(ret_pack.buf, "\036");
            }
            get_nickname_uid(row[0], nickname);
            strcat(ret_pack.buf, nickname);
            strcat(ret_pack.buf, "\036");
        }
        mysql_free_result(result);
    }
    bale_packet(&ret_pack, strlen(ret_pack.buf), flag * GROUP_RECORD);
    my_write(connect_fd, ret_pack);
    free(arg);
}
//群聊
void *group_chat(void *arg, int connect_fd)
{
    int num;
    char *output[4];    //uid、gid、内容、时间
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    MYSQL_RES *result;
    int flag;
    MYSQL_ROW row;
    struct packet ret_pack;
    memset(ret_pack.buf, 0, 1024);

    parse_string(pack_str, output, &num);
    sprintf(query, "SELECT * FROM group_member WHERE uid = %s AND gid = %s",
            output[0], output[1]);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
        my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
        my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    //不在群中
    if (mysql_num_rows(result) == 0)
    {
        flag = REFUSE;
        mysql_free_result(result);
    }
    else
    {
        flag = ACCEPT;
        mysql_free_result(result);
        //查找群成员，挨个发送并存入
        sprintf(query, "SELECT uid FROM group_member WHERE uid != %s AND gid = %s", output[0], output[1]);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
        if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
        pthread_mutex_unlock(&mysql_lock);
        int count = mysql_num_rows(result);
        for (int i = 0; i < count; i++)
        {
            int state = 0;
            row = mysql_fetch_row(result);
            //在线则发送消息并存入,离线直接存入
            if (is_user_online_uid(row[0]))
            {
                state |= MSG_READ;
                char nickname[64];
                char grpname[64];
                get_nickname_uid(output[0], nickname);
                get_grpname_gid(output[1], grpname);
                //uid、昵称、gid、群名称、内容、时间
                sprintf(ret_pack.buf, "%s\036%s\036%s\036%s\036%s\036%s\036", output[0], nickname, output[1], grpname, output[2], output[3]);                    bale_packet(&ret_pack, strlen(ret_pack.buf), PRIVATE_CHAT);
                bale_packet(&ret_pack, strlen(ret_pack.buf), GROUP_CHAT);
                my_write(get_cfd_uid(row[0]), ret_pack);
            }
            sprintf(query, "INSERT INTO message(type, send_uid, recv_uid, content, state, gid) "
                    "VALUES(%d, %s, %s, '%s', %d, %s)", MSG_GRP_CHAT, output[0], row[0], output[2], state, output[1]);
            pthread_mutex_lock(&mysql_lock);
            if (mysql_query(&mysql, query) < 0)
                my_err("mysql_query error");
            pthread_mutex_unlock(&mysql_lock);
        }
        mysql_free_result(result);
    }
    bale_packet(&ret_pack, 0, flag * GROUP_CHAT);
    my_write(connect_fd, ret_pack);
    free(arg);
}
//创建群聊
void *create_group(void *arg, int connect_fd)
{
    int num;
    char *output[3];    //账号、群名称、群介绍
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    char nickname[64];
    int gid;
    parse_string(pack_str, output, &num);
    get_nickname_account(output[0], nickname);
    //增加群信息
    sprintf(query, "INSERT INTO group_info(group_name, introduction) "
            "VALUES('%s', '%s')", output[1], output[2]);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    gid = mysql_insert_id(&mysql);
    pthread_mutex_unlock(&mysql_lock);
    //增加群成员信息(群主)
    int uid = get_uid_account(output[0]);
    sprintf(query, "INSERT INTO group_member(uid, gid, nickname, group_name, identy) "
            "VALUES(%d, %d, '%s', '%s', %d)", uid, gid, nickname, output[1], LORD);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    pthread_mutex_unlock(&mysql_lock);
    free(arg);
}
//解散群聊
void *dismiss_group(void *arg, int connect_fd)
{
    int num;
    char *output[2];    //账号、gid
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    struct packet ret_pack;
    int flag = ACCEPT;
    MYSQL_RES *result;
    MYSQL_ROW row;
    
    parse_string(pack_str, output, &num);
    int uid = get_uid_account(output[0]);
    //是否为群主
    sprintf(query, "SELECT identy FROM group_member WHERE gid = %s AND uid = %d", output[1], uid);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
        my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
        my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    if (mysql_num_rows(result) == 0)
        flag = REFUSE;
    else
    {
        row = mysql_fetch_row(result);
        mysql_free_result(result);
        if (atoi(row[0]) != LORD) 
            flag = REFUSE;
        else
        {
            flag = ACCEPT;
            //若为群主,删除群信息
            sprintf(query, "DELETE FROM group_info WHERE gid = %s", output[1]);
            pthread_mutex_lock(&mysql_lock);
            if (mysql_query(&mysql, query) < 0)
                my_err("mysql_query error");
            pthread_mutex_unlock(&mysql_lock);
        }
    }
    
    //发回成功/失败
    bale_packet(&ret_pack, 0, flag * DISMISS_GROUP);
    my_write(connect_fd, ret_pack);
    free(arg);
}
//申请加群
void *apply_group_req(void *arg, int connect_fd)
{
    int num;
    char *output[3];    //uid、gid和验证消息内容
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    MYSQL_RES *result;
    int flag = ACCEPT;
    struct packet ret_pack;
    int recv_cfd;
    char nickname[64];
    char grpname[64];

    memset(ret_pack.buf, 0, 1024);

    parse_string(pack_str, output, &num);
    //获取账号和昵称
    get_nickname_uid(output[0], nickname);
    //查找该群
    sprintf(query, "SELECT * FROM group_info WHERE gid = %s", output[1]);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
        my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
        my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    //查无此群
    if (mysql_num_rows(result) == 0)
    {
        flag = REFUSE;
        mysql_free_result(result);
    }
    //群聊存在
    else
    {
        get_grpname_gid(output[1], grpname);
        mysql_free_result(result);
        //查找是否已入群
        sprintf(query, "SELECT * FROM group_member WHERE uid = %s AND gid = %s ",
                output[0], output[1]);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
        if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
        pthread_mutex_unlock(&mysql_lock);
        //已入群
        if (mysql_num_rows(result) != 0)
        {
            flag = REFUSE;
            mysql_free_result(result);
        }
        //未入群
        else
        {
            flag = ACCEPT;
            mysql_free_result(result);
            //查找群主和管理员
            sprintf(query, "SELECT uid FROM group_member WHERE gid = %s AND identy = %d "
                    "OR gid = %s AND identy = %d", output[1], LORD, output[1], ADMIN);
            pthread_mutex_lock(&mysql_lock);
            if (mysql_query(&mysql, query) < 0)
                my_err("mysql_query error");
            if ( (result = mysql_store_result(&mysql)) == NULL)
                my_err("mysql_store_result error");
            pthread_mutex_unlock(&mysql_lock);
            int count = mysql_num_rows(result);
            //挨个发送，或存入离线消息(先判断是否在线)
            for (int i = 0; i < count; i++)
            {
                MYSQL_ROW row = mysql_fetch_row(result);
                //用户在线，发送消息(uid、nickname、gid、grpname、验证消息)
                if (is_user_online_uid(row[0]))
                {
                    recv_cfd = get_cfd_uid(row[0]);
                    sprintf(ret_pack.buf, "%s\036%s\036%s\036%s\036%s\036", output[0], nickname, output[1], grpname, output[2]);
                    bale_packet(&ret_pack, strlen(ret_pack.buf), APPLY_GROUP_REQ);
                    my_write(recv_cfd, ret_pack);
                }
                //否则存入离线消息
                else
                {
                    int state = 0;
                    sprintf(query, "INSERT INTO message(type, send_uid, recv_uid, content, state, gid) "
                            "VALUES(%d, %s, %s, '%s', %d, '%s')", MSG_APP_GRP_REQ, output[0], row[0], output[2], state, output[1]);
                    pthread_mutex_lock(&mysql_lock);
                    if (mysql_query(&mysql, query) < 0)
                        my_err("mysql_query error");
                    pthread_mutex_unlock(&mysql_lock);
                }
            }
            mysql_free_result(result);
        }
    }
    
    bale_packet(&ret_pack, 0, APPLY_GROUP_REQ * flag);
    my_write(connect_fd, ret_pack);
    free(arg);
}
//加群结果
void *apply_group_res(void *arg, int connect_fd)
{
    int num;
    char *output[3];    //uid、gid、一个状态(TRUE or FALSE)
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    int flag = REFUSE;
    char grpname[64];
    char nickname[64];
    struct packet ret_pack;

    memset(ret_pack.buf, 0, 1024);
    parse_string(pack_str, output, &num);
    get_nickname_uid(output[0], nickname);
    //若通过，则在group_member中加入信息
    if (atoi(output[2]) == TRUE)
    {
        flag = ACCEPT;
        sprintf(query, "INSERT INTO group_member (uid, gid, identy, nickname) "
                "VALUES(%s, %s, %d, '%s')", output[0], output[1], MEMBER, nickname);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
                my_err("mysql_query error");
        pthread_mutex_unlock(&mysql_lock);
        //添加群人数
        sprintf(query, "UPDATE group_info SET member_number = (member_number + 1) WHERE gid = %s",
                output[1]);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
        pthread_mutex_unlock(&mysql_lock);
    }
    else
        flag = REFUSE;
    //删除申请消息(连着管理的一起删掉)
    sprintf(query, "DELETE FROM message WHERE send_uid = %s AND gid = %s AND type = %d",
            output[0], output[1], MSG_APP_GRP_REQ);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
        my_err("mysql_query error");
    pthread_mutex_unlock(&mysql_lock);
    //如果在线，发送之(gid、grpname)
    if (is_user_online_uid(output[0]))
    {
        get_grpname_gid(output[1], grpname);
        int recv_fd = get_cfd_uid(output[0]);
        sprintf(ret_pack.buf, "%s\036", grpname);
        bale_packet(&ret_pack, strlen(ret_pack.buf), flag * APPLY_GROUP_RES);
        my_write(recv_fd, ret_pack);
    }
    //不在线则存入离线消息
    else
    {
        int state = 0;
        if (atoi(output[2]) == TRUE)
            state |= MSG_ACCEPT;
        sprintf(query, "INSERT INTO message(type, gid, recv_uid, state) "
                       "VALUES(%d, %s, %s, %d)", MSG_APP_GRP_RES, output[1], output[0], state);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
                my_err("mysql_query error");
        pthread_mutex_unlock(&mysql_lock);
    }
    free(arg);
}
//退出群聊
void *quit_group(void *arg, int connect_fd)
{
    int num;
    char *output[2];    //uid和gid
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    int flag;
    struct packet ret_pack;
    MYSQL_RES *result;
    MYSQL_ROW row;
    
    parse_string(pack_str, output, &num);
    //查看是否在群聊中
    sprintf(query, "SELECT identy FROM group_member WHERE uid = %s AND gid = %s",
            output[0], output[1]);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    //不在群聊
    if (mysql_num_rows(result) == 0)
        flag = REFUSE;
    else
    {
        mysql_free_result(result);
        row = mysql_fetch_row(result);
        //如果为群主，同样返回失败
        if (atoi(row[0]) == LORD)
            flag = REFUSE;
        else
        {
            flag = ACCEPT;
            //删除信息
            sprintf(query, "DELETE FROM group_member WHERE uid = %s AND gid = %s",
                    output[0], output[1]);
            pthread_mutex_lock(&mysql_lock);
            if (mysql_query(&mysql, query) < 0)
                my_err("mysql_query error");
            pthread_mutex_unlock(&mysql_lock);
            sprintf(query, "UPDATE group_info SET member_number = (member_number - 1) "
                    "WHERE gid = %s", output[1]);
            pthread_mutex_lock(&mysql_lock);
            if (mysql_query(&mysql, query) < 0)
                my_err("mysql_query error");
            pthread_mutex_unlock(&mysql_lock);
        }
    }
    bale_packet(&ret_pack, 0, QUIT_GROUP * flag);
    my_write(connect_fd, ret_pack);
    free(arg);
}
//列出所有群聊
void *list_group_all(void *arg, int connect_fd)
{
    int num;
    //不需要数据了
    char query[256];
    MYSQL_RES *result;
    char temp[64];
    struct packet ret_pack;
    MYSQL_ROW row;

    memset(temp, 0, 64);
    memset(ret_pack.buf, 0, 1024);

    sprintf(query, "SELECT gid, group_name FROM group_info");
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    int count = mysql_num_rows(result);
    for (int i = 0; i < count; i++)
    {
        row = mysql_fetch_row(result);
        sprintf(temp, "%s\036%s\036", row[0], row[1]);
        strcat(ret_pack.buf, temp);
    }
    bale_packet(&ret_pack, strlen(ret_pack.buf), LIST_GROUP_ALL);
    my_write(connect_fd, ret_pack);
    mysql_free_result(result);
    free(arg);
}
//列出我的群聊
void *list_group_mine(void *arg, int connect_fd)
{
    int num;
    char *output[1];
    char query[256];
    char grpname[256];
    MYSQL_RES *result;
    char temp[64];
    struct packet ret_pack;
    char *pack_str = ((struct packet*)arg)->buf;
    MYSQL_ROW row;

    memset(temp, 0, 64);
    memset(ret_pack.buf, 0, 1024);
    
    parse_string(pack_str, output, &num);
    sprintf(query, "SELECT gid FROM group_member WHERE uid = %s", output[0]);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    int count = mysql_num_rows(result);
    for (int i = 0; i < count; i++)
    {
        row = mysql_fetch_row(result);
        get_grpname_gid(row[0], grpname);
        sprintf(temp, "%s\036%s\036", row[0], grpname);
        strcat(ret_pack.buf, temp);
    }
    bale_packet(&ret_pack, strlen(ret_pack.buf), LIST_GROUP_MINE);
    my_write(connect_fd, ret_pack);
    mysql_free_result(result);
    free(arg);
}
//查询群聊
void *inq_group(void *arg, int connect_fd)
{
    int num;
    char *output[1];    //gid
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    int flag;
    MYSQL_RES *result;
    struct packet ret_pack;
    memset(ret_pack.buf, 0, 1024);

    parse_string(pack_str, output, &num);
    sprintf(query, "SELECT gid, group_name, introduction, member_number "
            "FROM group_info WHERE gid = %s", output[0]);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    if (mysql_num_rows(result) == 0)
        flag = REFUSE;
    //存在则发回gid,group_name, introduction,成员数量
    else
    {
        flag = ACCEPT;
        MYSQL_ROW row = mysql_fetch_row(result);
        for (int i = 0; i < 4; i++)
        {
            strcat(ret_pack.buf, row[i]);
            strcat(ret_pack.buf, "\036");
        }
    }
    bale_packet(&ret_pack, strlen(ret_pack.buf), flag * INQ_GROUP);
    my_write(connect_fd, ret_pack);
    mysql_free_result(result);
    free(arg);
}
//设置管理
void *set_admin(void *arg, int connect_fd)
{
    int num;
    char *output[3];    //uid、gid、被设置的uid
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    int flag;
    MYSQL_RES *result;
    struct packet ret_pack;
    memset(ret_pack.buf, 0, 1024);

    parse_string(pack_str, output, &num);
    //查询是否为群主
    sprintf(query, "SELECT identy FROM group_member WHERE gid = %s AND uid = %s",
            output[1], output[0]);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    if (mysql_num_rows(result) == 0)
        flag = REFUSE;
    else
    {
        MYSQL_ROW row = mysql_fetch_row(result);
        mysql_free_result(result);
        //不为群主
        if (atoi(row[0]) != LORD)
        {
            flag = REFUSE;
        }
        else
        {
            sprintf(query, "SELECT * FROM group_member WHERE gid = %s AND uid = %s",
                    output[1], output[2]);
            pthread_mutex_lock(&mysql_lock);
            if (mysql_query(&mysql, query) < 0)
                my_err("mysql_query error");
            if ( (result = mysql_store_result(&mysql)) == NULL)
                my_err("mysql_store_result error");
            pthread_mutex_unlock(&mysql_lock);
            //不在群聊中
            if (mysql_num_rows(result) == 0)
            {
                flag = REFUSE;
                mysql_free_result(result);
            }
            else    //在群聊中则更新
            {
                flag = ACCEPT;
                sprintf(query, "UPDATE group_member SET identy = %d WHERE gid = %s AND uid = %s",
                    ADMIN, output[1], output[2]);
                pthread_mutex_lock(&mysql_lock);
                if (mysql_query(&mysql, query) < 0)
                    my_err("mysql_query error");
                pthread_mutex_unlock(&mysql_lock);
            }
        }
    }
    bale_packet(&ret_pack, strlen(ret_pack.buf), flag * SET_ADMIN);
    my_write(connect_fd, ret_pack);
    free(arg);
}
void *send_file_req(void *arg, int connect_fd)
{
    int num;
    char *output[8];
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    MYSQL_RES *result;
    int flag;
    struct packet ret_pack;
    parse_string(pack_str, output, &num);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    mysql_free_result(result);
    free(arg);
}
void *send_file_res(void *arg, int connect_fd)
{
    int num;
    char *output[8];
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    MYSQL_RES *result;
    int flag;
    struct packet ret_pack;
    parse_string(pack_str, output, &num);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    mysql_free_result(result);
    free(arg);
}
//列出用户
void *list_user(void *arg, int connect_fd)
{
    MYSQL_RES *result;
    MYSQL_ROW row;
    struct packet ret_pack;
    char query[256];
    memset(ret_pack.buf, 0, 1024);
    //返回uid、账号、昵称、状态
    sprintf(query, "SELECT uid, account, nickname, state FROM user_info");
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    int count = mysql_num_rows(result);
    for (int i = 0; i < count; i++)
    {
        row = mysql_fetch_row(result);
        for (int j = 0; j < 4; j++)
        {
            strcat(ret_pack.buf, row[j]);
            strcat(ret_pack.buf, "\036");
        }
    }
    
    bale_packet(&ret_pack, strlen(ret_pack.buf), LIST_USER);
    my_write(connect_fd, ret_pack);
    mysql_free_result(result);
    free(arg);
}
//通过账号获取uid
int get_uid_account(const char *account)
{
    char query[256];
    MYSQL_RES *result;
    int ret;
    sprintf(query, "SELECT * FROM user_info WHERE account = %s", account);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    if (mysql_num_rows(result) == 0)
        ret = -1;
    else
    {
        MYSQL_ROW row = mysql_fetch_row(result);
        ret = atoi(row[0]);
    }
    mysql_free_result(result);
    return ret;
}
//通过账号获取昵称
int get_nickname_account(const char *account, char *nickname)
{
    char query[256];
    MYSQL_RES *result;
    sprintf(query, "SELECT nickname FROM user_info WHERE account = %s", account);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    if (mysql_num_rows(result) == 0)
    {
        mysql_free_result(result);
        return -1;
    }
    else
    {
        MYSQL_ROW row = mysql_fetch_row(result);
        strcpy(nickname, row[0]);
        mysql_free_result(result);
        return 0;
    }
}
//通过uid获取昵称
int get_nickname_uid(const char *uid, char *nickname)
{
    char query[256];
    MYSQL_RES *result;
    sprintf(query, "SELECT nickname FROM user_info WHERE uid = %s", uid);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    if (mysql_num_rows(result) == 0)
    {
        mysql_free_result(result);
        return -1;
    }
    else
    {
        MYSQL_ROW row = mysql_fetch_row(result);
        strcpy(nickname, row[0]);
        mysql_free_result(result);
        return 0;
    }
}
//通过账号获取套接字
int get_cfd_account(const char *account)
{
    char query[256];
    MYSQL_RES *result;
    sprintf(query, "SELECT connect_fd FROM user_info WHERE account = %s", account);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    if (mysql_num_rows(result) == 0)
    {
        mysql_free_result(result);
        return -1;
    }
    else
    {
        MYSQL_ROW row = mysql_fetch_row(result);
        mysql_free_result(result);
        return atoi(row[0]);
    }
}
//通过uid获取套接字
int get_cfd_uid(const char *uid)
{
    char query[256];
    MYSQL_RES *result;
    sprintf(query, "SELECT connect_fd FROM user_info WHERE uid = %s", uid);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    if (mysql_num_rows(result) == 0)
    {
        mysql_free_result(result);
        return -1;
    }
    else
    {
        MYSQL_ROW row = mysql_fetch_row(result);
        mysql_free_result(result);
        return atoi(row[0]);
    }
}
//判断用户是否在线(通过账号)
int is_user_online_account(const char *account)
{
    char query[256];
    MYSQL_RES *result;
    sprintf(query, "SELECT state FROM user_info WHERE account = %s", account);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    if (mysql_num_rows(result) == 0)
    {
        mysql_free_result(result);
        return -1;
    }
    else
    {
        MYSQL_ROW row = mysql_fetch_row(result);
        mysql_free_result(result);
        return atoi(row[0]) == ONLINE;
    }
}
//判断用户是否在线(通过uid)
int is_user_online_uid(const char *uid)
{
    char query[256];
    MYSQL_RES *result;
    sprintf(query, "SELECT state FROM user_info WHERE uid = %s", uid);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    if (mysql_num_rows(result) == 0)
    {
        mysql_free_result(result);
        return -1;
    }
    else
    {
        MYSQL_ROW row = mysql_fetch_row(result);
        mysql_free_result(result);
        return atoi(row[0]) == ONLINE;
    }
}
//通过gid获取群名
int get_grpname_gid(const char *gid, char *group_name)
{
    char query[256];
    MYSQL_RES *result;
    sprintf(query, "SELECT group_name FROM group_info WHERE gid = %s", gid);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    if (mysql_num_rows(result) == 0)
    {
        mysql_free_result(result);
        return -1;
    }
    else
    {
        MYSQL_ROW row = mysql_fetch_row(result);
        strcpy(group_name, row[0]);
        mysql_free_result(result);
        return 0;
    }
}
//退出登录
void *quit_user(void *arg, int connect_fd)
{
    int num;
    char *output[1];    //账号
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    parse_string(pack_str, output, &num);
    sprintf(query, "UPDATE user_info SET state = %d WHERE account = %s", OFFLINE, output[0]);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    pthread_mutex_unlock(&mysql_lock);
}
//修改个人资料
void *update_user_info(void *arg, int connect_fd)
{
    int num;
    char *output[5];    //uid、nickname、gender、生日、所在地
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    struct packet ret_pack;
    memset(ret_pack.buf, 0, 1024);

    parse_string(pack_str, output, &num);
    
    sprintf(query, "UPDATE user_info SET nickname = '%s', gender = '%s', birthday = '%s', location = '%s' WHERE uid = %s",
            output[1], output[2], output[3], output[4], output[0]);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
        my_err("mysql_query error");
    pthread_mutex_unlock(&mysql_lock);
    
    bale_packet(&ret_pack, 0, UPDATE_USER_INFO);
    my_write(connect_fd, ret_pack);
    free(arg);
}

void *update_group_info(void *arg, int connect_fd)
{
    int num;
    char *output[4];    //uid、gid、群名称、群介绍
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    int flag;
    MYSQL_RES *result;
    struct packet ret_pack;
    memset(ret_pack.buf, 0, 1024);

    parse_string(pack_str, output, &num);
    sprintf(query, "SELECT * FROM group_member WHERE uid = %s AND gid = %s AND identy != %d",
            output[0], output[1], MEMBER);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
        my_err("mysql_query error");
    if ((result = mysql_store_result(&mysql)) == NULL)
        my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    //不为群主或管理
    if (mysql_num_rows(result) == 0)
        flag = REFUSE;
    else
    {
        flag = ACCEPT;
        sprintf(query, "UPDATE group_info SET group_name = '%s', introduction = '%s' WHERE gid = %s",
            output[2], output[3], output[1]);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
        pthread_mutex_unlock(&mysql_lock);
    }

    mysql_free_result(result);
    bale_packet(&ret_pack, 0, flag * UPDATE_GRP_INFO);
    my_write(connect_fd, ret_pack);
    free(arg);
}

