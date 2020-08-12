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
    if (mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "chatroom", 0, NULL, 0) == NULL)
    {
        my_err("mysql_real_connect error");
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
    if (flag == REGISTER_USER)
        return register_user; 
    if (flag == RETRIEVE_PWD)
        return retrieve_pwd;
    if (flag == PRIVATE_CHAT)
        return private_chat;
    if (flag == ADD_FRIEND_REQ)
        return add_friend_req;
    if (flag == ADD_FRIEND_RES)
        return add_friend_res;
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
    MYSQL_RES *result;
    int flag = ACCEPT;
    parse_string(pack_str, output, &num);
    sprintf(query, "SELECT * FROM user_info WHERE account = %s AND password = '%s'", \
            output[0], output[1]);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
    if (mysql_num_rows(result) == 0)
        flag = REFUSE;
    else
    {
        //更新用户状态
        sprintf(query, "UPDATE user_info SET state = %d WHERE account = '%s'", ONLINE, output[0]);
        if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
        sprintf(query, "UPDATE user_info SET connect_fd = %d WHERE account = %s", connect_fd, output[0]);
    }
    pthread_mutex_unlock(&mysql_lock);
    struct packet ret_pack;
    bale_packet(&ret_pack, 0, LOGIN_USER * flag);
    my_write(connect_fd, ret_pack);
    mysql_free_result(result);
    free(arg);
}
//找回密码
void *retrieve_pwd(void *arg, int connect_fd)
{
    int num;
    char *output[1];    //账号
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    MYSQL_RES *result;
    int flag = ACCEPT;
    struct packet ret_pack;
    *(ret_pack.buf) = '\0';
    //第一次发送账号
    parse_string(pack_str, output, &num);
    //指向堆区空间
    char *account = output[0];
    sprintf(query, "SELECT * FROM user_info WHERE account = '%s'", output[0]);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
    //账号不存在或未设置密保均返回REFUSE
    if (mysql_num_rows(result) == 0)
        flag = REFUSE;
    else
    {
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row[7] == NULL)
            flag = REFUSE;
        else
            strcpy(ret_pack.buf, row[7]);
    }
    pthread_mutex_unlock(&mysql_lock);
    bale_packet(&ret_pack, strlen(ret_pack.buf), RETRIEVE_PWD * flag);
    my_write(connect_fd, ret_pack);
    //ACCEPT时继续收数据(密保答案)
    if (flag == ACCEPT)
    {
        //借用一下ret_pack...
        my_read(connect_fd, &ret_pack);
        pack_str = ((struct packet*)&ret_pack)->buf;
        parse_string(pack_str, output, &num);
        sprintf(query, "SELECT * FROM user_info WHERE security_a = '%s' AND account = %s", output[0], account);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
        if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
        pthread_mutex_unlock(&mysql_lock);
        if (mysql_num_rows(result) == 0)
        {
            *(ret_pack.buf) = '\0';
            flag = REFUSE;
        }
        else
        {
            MYSQL_ROW row = mysql_fetch_row(result);
            strcpy(ret_pack.buf, row[8]);
        }
        bale_packet(&ret_pack, strlen(ret_pack.buf), RETRIEVE_PWD * flag);
        my_write(connect_fd, ret_pack);
    }
    mysql_free_result(result);
    free(arg);
}
//私聊
void *private_chat(void *arg, int connect_fd)
{
    int num;
    char *output[3];    //两个账号，聊天内容
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    MYSQL_RES *result;
    int flag = ACCEPT;
    int uid1, uid2;
    struct packet ret_pack;
    parse_string(pack_str, output, &num);
    uid1 = get_uid_account(output[0]);
    uid2 = get_uid_account(output[1]);
    //查看是否在黑名单 
    sprintf(query, "SELECT * FROM user_relation WHERE uid1 = %d AND uid2 = %d "
            "OR uid1 = %d AND uid2 = %d",uid1, uid2, uid2, uid1);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    
    MYSQL_ROW row = mysql_fetch_row(result);
    if (atoi(row[2]) & FRIEND)
    {
        int state = 0;
        flag = ACCEPT;
        //查询是否屏蔽，若屏蔽则无法发送，只存入记录
        if (atoi(row[5]) & UID2_MUTE)
        {
            state |= MSG_SHIELD;
        }
        else
        {
            //在线则发送消息并存入,离线直接存入
            if (is_user_online(output[1]))
            {
                state |= MSG_READ;
            }
            char nickname[64];
            get_nickname_account(output[0], nickname);
            sprintf(ret_pack.buf, "%s\036%s\036%s\036", output[0], nickname, output[2]);
            bale_packet(&ret_pack, strlen(ret_pack.buf), PRIVATE_CHAT);
            my_write(get_cfd_account(output[1]), ret_pack);
        }
        sprintf(query, "INSERT INTO message(type, send_uid, recv_uid, content, state) "
                "VALUES(%d, %d, %d, '%s', %d)", PRIVATE_CHAT, uid1, uid2, output[2], state);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
        pthread_mutex_unlock(&mysql_lock);
    }
    else
    {
        flag = REFUSE;
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
    char *output[3];    //两个账号和验证消息内容
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    MYSQL_RES *result;
    int flag = ACCEPT;
    struct packet ret_pack;
    int uid1, uid2;
    char nickname[64];
    int recv_cfd = get_cfd_account(output[1]);
    *(ret_pack.buf) = '\0';
    parse_string(pack_str, output, &num);
    //获取账号和昵称
    uid1 = get_uid_account(output[0]);
    uid2 = get_uid_account(output[1]);
    get_nickname_account(output[0], nickname);
    sprintf(query, "SELECT * FROM user_relation WHERE uid1 = %d AND uid2 = %d "
            "OR uid1 = %d AND uid2 = %d", uid1, uid2, uid2, uid1);
    //若在黑名单中或已成为好友，返回失败
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    if (mysql_num_rows(result) == 0)
    {
        flag = ACCEPT;
    }
    //说明找到两人关系(好友或是黑名单)
    //此时都设置为REFUSE
    else
    {
        flag = REFUSE;
    }
    //创建申请成功
    if (flag == ACCEPT)
    {
        mysql_free_result(result);
        sprintf(query, "SELECT * FROM user_info WHERE uid = %d\n", uid2);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
        if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
        //没有这个账号...
        if (mysql_num_rows(result) == 0)
        {
            flag = REFUSE;
        }
        else
        {
            char status[12];
            sprintf(status, "%d", ONLINE);
            MYSQL_ROW row = mysql_fetch_row(result);
            //好友在线，发送消息(账号、昵称、验证消息)
            if (strcmp(row[11], status) == 0)
            {
                sprintf(ret_pack.buf, "%s\036%s\036%s\036", output[0], nickname, output[2]);
                bale_packet(&ret_pack, strlen(ret_pack.buf), ADD_FRIEND_REQ * flag);
                my_write(recv_cfd, ret_pack);
            }
            //否则存入离线消息
            else
            {
                int state = 0;
                sprintf(query, "INSERT INTO message(type, send_uid, recv_uid, content, state) "
                        "VALUES(%d, %d, %d, '%s', %d)", MSG_ADD_FRI_REQ, uid1, uid2, output[2], state);
                if (mysql_query(&mysql, query) < 0)
                    my_err("mysql_query error");
            }
        }
        pthread_mutex_unlock(&mysql_lock);
    }
    bale_packet(&ret_pack, 0, ADD_FRIEND_REQ * flag);
    my_write(connect_fd, ret_pack);
    mysql_free_result(result);
    free(arg);
}
//添加好友(回复)
void *add_friend_res(void *arg, int connect_fd)
{
    int num;
    char *output[3];    //两个账号，一个状态(TRUE or FALSE)
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    int flag = REFUSE;
    struct packet ret_pack;
    int uid1, uid2;
    char nickname[64];
    parse_string(pack_str, output, &num);
    uid1 = get_uid_account(output[0]);
    uid2 = get_uid_account(output[1]);
    get_nickname_account(output[1], nickname);
    //若通过，则在user_relation中加入信息
    if (atoi(output[2]) == TRUE)
    {
        flag = ACCEPT;
        sprintf(query, "INSERT INTO user_relation(uid1, uid2, relation) "
                "VALUES(%d, %d, %d)", uid1, uid2, FRIEND);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
                my_err("mysql_query error");
        pthread_mutex_unlock(&mysql_lock);
        //并删除数据
        sprintf(query, "DELETE FROM message WHERE uid1 = %d AND uid2 = %d AND type =%d",
                uid1, uid2, MSG_ADD_FRI_REQ);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
                my_err("mysql_query error");
        pthread_mutex_unlock(&mysql_lock);
    }
    //如果在线，发送之
    if (is_user_online(output[0]))
    {
        int connect_fd = get_cfd_account(output[0]);
        strcpy(ret_pack.buf, output[1]);
        bale_packet(&ret_pack, strlen(ret_pack.buf), flag * ADD_FRIEND_RES);
        my_write(connect_fd, ret_pack);
    }
    //不在线则存入离线消息
    else
    {
        int state = 0;
        if (atoi(output[2]) == TRUE)
            state |= MSG_ACCEPT;
        sprintf(query, "INSERT INTO message(type, send_uid, recv_uid, state) "
                       "VALUES(%d, %d, %d, '%s', %d)", MSG_ADD_FRI_RES, uid2, uid1, output[2], state);
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
    char *output[2];    //两个账号
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    int uid1, uid2;
    
    parse_string(pack_str, output, &num);
    uid1 = get_uid_account(output[0]);
    uid2 = get_uid_account(output[1]);

    sprintf(query, "DELETE FROM user_relation WHERE uid1 = %d AND uid2 = %d "
            "OR uid1 = %d AND uid2 = %d", uid1, uid2, uid2, uid1);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
        my_err("mysql_query error");
    pthread_mutex_unlock(&mysql_lock);

    free(arg);
}
//查询好友
void *inq_friend(void *arg, int connect_fd)
{
    int num;
    char *output[1];    //被查询者的账号
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    int flag;
    MYSQL_RES *result;
    struct packet ret_pack;

    parse_string(pack_str, output, &num);
    sprintf(query, "SELECT uid, account, nickname, gender, birthday, location, signature, state "
            " FROM user_info WHERE account = %s", output[0]);
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
    char *output[1];    //账号
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    MYSQL_RES *result;
    struct packet ret_pack;
    parse_string(pack_str, output, &num);
    int uid = get_uid_account(output[0]);
    //找到好友
    sprintf(query, "SELECT uid1, uid2 FROM user_relation WHERE uid1 = %d OR uid2 = %d",
            uid, uid);

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
            if (atoi(row[j]) != uid)
            {
                MYSQL_RES *result_temp;
                sprintf(query, "SELECT uid, account, nickname WHERE uid = %s",
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
                    for (int l = 0; l < 3; l++)
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
    char *output[2];    //两个账号
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    struct packet ret_pack;
    int uid1, uid2;
    MYSQL_RES *result;

    parse_string(pack_str, output, &num);
    uid1 = get_uid_account(output[0]);
    uid2 = get_uid_account(output[1]);
    //设置屏蔽关系，并把message中聊天记录全部置为屏蔽
    sprintf(query, "UPDATE TABLE user_relation SET WHERE uid1 = %d AND uid2 = %d "
            "OR uid1 = %d AND uid2 = %d",uid1, uid2, uid2, uid1);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
        my_err("mysql_query error");
    pthread_mutex_unlock(&mysql_lock);
    if ( (result = mysql_store_result(&mysql)) == NULL)
        my_err("mysql_store_result error");
    if (mysql_num_rows(result) == 0)
    {
        mysql_free_result(result);
        sprintf(query, "SELECT * FROM user_relation WHERE uid1 = %d AND uid2 = %d",
                uid1, uid2);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
        
    }
    bale_packet(&ret_pack, 0, DEL_FRIEND);
    my_write(connect_fd, ret_pack);
    free(arg);
}
void *private_record(void *arg, int connect_fd)
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
//群聊
void *group_chat(void *arg, int connect_fd)
{
    int num;
    char *output[3];    //账号、gid和聊天内容
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    MYSQL_RES *result;
    struct packet ret_pack;
    
    parse_string(pack_str, output, &num);
    int uid = get_uid_account(output[0]);
    //查出所有群员
    sprintf(query, "SELECT uid FROM group_member WHERE gid = %s", output[1]);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
        my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
        my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    int count = mysql_num_rows(result);
    //依次处理
    for (int i = 0; i < count; i++)
    {
        int state = 0;
        char timestamp[24];
        get_timestamp(timestamp);
        char nickname[64];
        get_nickname_account(output[0], nickname);
        char group_name[64];
        get_grpname_gid(output[1], group_name);
        //依次判断是否在线，在线发送，离线存入消息
        MYSQL_ROW row = mysql_fetch_row(result);
        sprintf(ret_pack.buf, "%s\036%s\036%s\036%s\036", nickname, group_name, output[2], timestamp);
        if (is_user_online_uid(row[0]))
        {
            state |= MSG_READ;
            //发回昵称、群名、内容以及时间
            int cfd = get_cfd_account(row[0]);
            bale_packet(&ret_pack, strlen(ret_pack.buf), GROUP_CHAT);
            my_write(cfd, ret_pack);
        }
        sprintf(query, "INSERT INTO message(type, send_uid, recv_uid, gid, content, state) "
                "VALUES(%d, %d, %s, %s, '%s', %d)", GROUP_CHAT, uid, row[0], output[1], output[2], state);
    }
    mysql_free_result(result);
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
            flag = ACCEPT;
    }
    //若为群主,删除群信息
    if (atoi(row[0]) == LORD) 
    {
        sprintf(query, "DELETE FROM group_info WHERE gid = %s", output[1]);
        pthread_mutex_lock(&mysql_lock);
        if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
        pthread_mutex_unlock(&mysql_lock);
    }
    //发回成功/失败
    bale_packet(&ret_pack, 0, flag * DISMISS_GROUP);
    my_write(connect_fd, ret_pack);
    free(arg);
}

void *apply_group_req(void *arg, int connect_fd)
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
void *apply_group_res(void *arg, int connect_fd)
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
//退出群聊
void *quit_group(void *arg, int connect_fd)
{
    int num;
    char *output[2];    //账号和gid
    char *pack_str = ((struct packet*)arg)->buf;
    char query[256];
    
    parse_string(pack_str, output, &num);
    int uid = get_uid_account(output[0]);
    sprintf(query, "DELETE FROM group_member WHERE uid = %d AND gid = %s",
            uid, output[1]);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    pthread_mutex_unlock(&mysql_lock);
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
    MYSQL_RES *result;
    char temp[64];
    struct packet ret_pack;
    char *pack_str = ((struct packet*)arg)->buf;
    MYSQL_ROW row;

    memset(temp, 0, 64);
    memset(ret_pack.buf, 0, 1024);
    
    parse_string(pack_str, output, &num);
    int uid = get_uid_account(output[0]);
    sprintf(query, "SELECT gid, group_name FROM group_member WHERE uid = %d", uid);
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
    MYSQL_RES *result;
    struct packet ret_pack;

    parse_string(pack_str, output, &num);
    sprintf(query, "SELECT * FROM group_info WHERE gid = %s", output[0]);
    pthread_mutex_lock(&mysql_lock);
    if (mysql_query(&mysql, query) < 0)
            my_err("mysql_query error");
    if ( (result = mysql_store_result(&mysql)) == NULL)
            my_err("mysql_store_result error");
    pthread_mutex_unlock(&mysql_lock);
    //gid,group_name, introduction,成员数量，在线成员
    MYSQL_ROW row = mysql_fetch_row(result);
    memset(ret_pack.buf, 0, 1024);
    for (int i = 0; i < 6; i++)
    {
        strcat(ret_pack.buf, row[i]);
        strcat(ret_pack.buf, "\036");
    }
    bale_packet(&ret_pack, strlen(ret_pack.buf), INQ_GROUP);
    my_write(connect_fd, ret_pack);
    mysql_free_result(result);
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
    sprintf(query, "SELECT * FROM user_info WHERE account = %s", account);
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
        strcpy(nickname, row[2]);
        mysql_free_result(result);
        return 0;
    }
}
//通过账号获取套接字
int get_cfd_account(const char *account)
{
    char query[256];
    MYSQL_RES *result;
    sprintf(query, "SELECT * FROM user_info WHERE account = %s", account);
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
        return atoi(row[12]);
    }
}
//通过uid获取套接字
int get_cfd_uid(const char *uid)
{
    char query[256];
    MYSQL_RES *result;
    sprintf(query, "SELECT * FROM user_info WHERE uid = %s", uid);
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
        return atoi(row[12]);
    }
}
//判断用户是否在线(通过账号)
int is_user_online(const char *account)
{
    char query[256];
    MYSQL_RES *result;
    sprintf(query, "SELECT * FROM user_info WHERE account = %s", account);
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
        return atoi(row[11]) == ONLINE;
    }
}
//判断用户是否在线(通过uid)
int is_user_online_uid(const char *uid)
{
    char query[256];
    MYSQL_RES *result;
    sprintf(query, "SELECT * FROM user_info WHERE uid = %s", uid);
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
        return atoi(row[11]) == ONLINE;
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
