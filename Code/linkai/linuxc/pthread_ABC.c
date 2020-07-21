/*************************************************************************
    > File Name: pthread_ABC.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年07月15日 星期三 16时01分22秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <pthread.h>
pthread_mutex_t a_time, b_time, c_time; 
pthread_cond_t a_over = PTHREAD_COND_INITIALIZER;
pthread_cond_t b_over = PTHREAD_COND_INITIALIZER;
pthread_cond_t c_over = PTHREAD_COND_INITIALIZER;
void my_err(const char *str)
{
     perror(str);
     exit(1);
}
void *func_a(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&a_time);
        pthread_cond_wait(&c_over, &a_time);
        putchar('A');
        pthread_mutex_unlock(&a_time);
        pthread_cond_signal(&a_over);
    }
}
void *func_b(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&b_time);
        pthread_cond_wait(&a_over, &b_time);
        putchar('B');
        pthread_mutex_unlock(&b_time);
        pthread_cond_signal(&b_over);
    }
}
void *func_c(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&c_time);
        pthread_cond_wait(&b_over, &c_time);
        putchar('C');
        pthread_mutex_unlock(&c_time);
        pthread_cond_signal(&c_over);
    }
}
int main(int argc, char *argv[])
{
    pthread_t tid_a, tid_b, tid_c;
    pthread_mutex_init(&a_time, NULL);
    pthread_mutex_init(&b_time, NULL);
    pthread_mutex_init(&c_time, NULL);
    pthread_create(&tid_a, NULL, func_a, NULL);
    pthread_create(&tid_b, NULL, func_b, NULL);
    pthread_create(&tid_c, NULL, func_c, NULL);
    pthread_cond_signal(&c_over);
    pthread_exit( (void*)0 );   //while (1);
}
