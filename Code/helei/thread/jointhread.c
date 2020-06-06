/*************************************************************************
	> File Name: jointhread.c
	> Author: 
	> Mail: 
	> Created Time: 2020年05月17日 星期日 08时57分29秒
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

void assisthread(void *arg)
{
    printf("I am helping to do some jobs\n");
    sleep(3);
    pthread_exit();
}
int main(void)
{
    pthread_t assistthid;
    int status ;
    pthread_create(&assistthid,NULL,(void *) assisthread,NULL);
    pthread_join(assistthid,(void *)&status);
    printf("assistthread's exit is caused %d\n",status);
    return 0;
}
