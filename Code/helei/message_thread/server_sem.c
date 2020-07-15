/*************************************************************************
	> File Name: server_sem.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月10日 星期五 18时19分15秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<linux/sem.h>


#define MAX_RESOURCE 5

int main(void)
{
    key_t key;
    int semid;
    struct sembuf sbuf = {0,-1,IPC_NOWAIT};
    union semun semopts;
    if((key = ftok(".",'a')) == -1)
    {
        perror("ftok error\n");
        exit(-1);
    }
    if((semid = semget(key,1,IPC_CREAT | 0664)) == -1)
    {
        perror("semget error\n");
        exit(1);
    }
    semopts.val = MAX_RESOURCE;
    if(semctl(semid,0,SETVAL,semopts) == -1)
    {
        perror("semctl error\n");
        exit(-1);
    }
    while(1)
    {
        if(semop(semid,&buf,1) == -1)
        {
            perror("semop error\n");
            exit(1);
        }
        sleep(3);
    }
    return 0;
}

