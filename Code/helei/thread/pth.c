/*************************************************************************
	> File Name: pth.c
	> Author: 
	> Mail: 
	> Created Time: 2020年05月07日 星期四 08时57分13秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>


void *pth_fun()
{
    while(1)
    {
        printf("1111\n");
        sleep(1);
    }
    return NULL;

}
int main(void)
{
    pthread_t TID = 0;
    pthread_create(&TID,NULL,pth_fun(),NULL);
    while(1)
    {
        printf("2222\n");
        sleep(3);
    }
}
