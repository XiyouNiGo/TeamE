
/*************************************************************************
  > File Name: pthread_consumer_producer.c
  > Author: NiGo
  > Mail: nigo@xiyoulinux.org
  > Created Time: 2020年05月16日 星期六 15时50分18秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>
#include <string.h>

struct message
{
    int num;
    struct message *next;
};

void pthread_err(int ret, const char *str)
{
    fprintf(stderr, "%s error : %s\n", str, strerror(ret));
    exit(1);
}

struct message *head = NULL;

pthread_cond_t has_product = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *consumer(void *arg)
{
    while (1)
    {
        struct message *temp;
        pthread_mutex_lock(&mutex);
        if (head == NULL)   //多个消费者时用while,因为broadcast之后如有多个消费者阻塞，却只有一个数据，会发生段错误
        {
            //阻塞等待条件变量，并解锁（返回时重新加锁）
            pthread_cond_wait(&has_product, &mutex);
        }
        //模拟消费数据
        temp = head;
        head = head->next;

        pthread_mutex_unlock(&mutex);
        printf("-----Consume %d\n", temp->num);
        free(temp);
        sleep(rand() % 3);
    }

    return NULL;
}

void *producer(void *arg)
{
    while (1)
    {
        //模拟生产数据
        struct message *product = (struct message*)malloc(sizeof(struct message));
        product->num = rand()%1000+1;
        printf("=====Produce %d\n", product->num);
        //加锁（锁的粒度越小越好，提高并发性）
        pthread_mutex_lock(&mutex);
        //头插法
        product->next = head;
        head = product;
        //解锁
        pthread_mutex_unlock(&mutex);
        //唤醒阻塞在has_product上的线程
        pthread_cond_signal(&has_product);//多个消费者:phread_cond_broadcast(&has_product);
        sleep(rand() % 3);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t pid, cid;
    srand( (unsigned int)time(NULL));

    pthread_create(&pid, NULL, producer, NULL);
    pthread_create(&cid, NULL, consumer, NULL);

    pthread_join(pid, NULL);
    pthread_join(cid, NULL);

    return 0;
}
