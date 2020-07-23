#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <pthread.h>

#define DEFAULT_TIME 10
#define MAX_WAIT_TASK_NUM 10
#define DEFAULT_THREAD_NUM 10

//线程池任务
typedef struct thread_pool_task_t
{
    void *(*function)(void*);   //回调函数
    void *arg;                  //函数参数
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

int thread_pool_add_task(thread_pool_t *pool, void *(*function)(void*), void *arg);

int thread_pool_free(thread_pool_t *pool);

int thread_pool_destory(thread_pool_t *pool);

void *admin_thread(void *thread_pool);

int is_thread_alive(pthread_t tid);

#endif
