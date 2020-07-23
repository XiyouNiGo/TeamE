#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include "thread_pool.h"
#include "my_config.h"

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
                pthread_exit(NULL);
            }
        }
        //线程池处于关闭状态时退出
        if (pool->state == CLOSE)
        {
            pthread_mutex_unlock(&pool->lock);
            printf("Thread 0x%x is exiting\n", (unsigned int)pthread_self());
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
        (*(task.function))(task.arg);
        //结束任务
        printf("Thread 0x%x stops working\n", (unsigned int)pthread_self());
        pthread_mutex_lock(&pool->thread_counter);
        pool->busy_thread_num--;
        pthread_mutex_unlock(&pool->thread_counter);
    }

    pthread_exit(NULL);
}
//向任务队列中添加任务
int thread_pool_add_task(thread_pool_t *pool, void *(*function)(void*), void *arg)
{
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
    pool->queue_rear = (pool->queue_rear + 1) % pool->queue_max_size;  /* 逻辑环  */
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
        pthread_join(pool->alive_thread_num, NULL);

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
