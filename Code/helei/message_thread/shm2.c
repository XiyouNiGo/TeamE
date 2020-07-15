/*************************************************************************
	> File Name: shm1.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月10日 星期五 13时38分23秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<strings.h>
#include<signal.h>

#define SHM_FILE "./shmfile"
#define SHM_SIZE 4096

int shmid = -1;

void print_err(char *str)
{
    perror(str);
    exit(-1);
}
void create_or_get_shm(void)
{
    int fd = 0;
    key_t key = -1;
    if((fd = open(SHM_FILE,O_RDWR | O_CREAT,0664)) == -1)
        print_err("open failed");
    if((key = ftok(SHM_FILE,'b')) == -1)
        print_err("ftok failed");
    if((shmid = shmget(key,SHM_SIZE,0664 | IPC_CREAT)) == -1)
        print_err("shmget failed");

}
char buf[300] = "aaaaaaaaaaaa\nbbbbbbbbb\nccccccccccccc\ndddddddddddddddddddddddddddddd:wqd";
void *shmaddr = NULL;

void signal_fun(int signo)
{
    shmdt(shmaddr);
    shmctl(shmid,IPC_RMID,NULL);
    exit(-1);
}
int main(void)
{
    signal(SIGINT,signal_fun);
    create_or_get_shm();
    //建立映射关系
    shmaddr = shmat(shmid,NULL,0);
    if(shmaddr == (void *)-1)
        print_err("shmat failed");
    while(1)
    {
        if(strlen((char *)shmaddr) != 0)
        {
            printf("%s\n",(char *)shmaddr); 
            bzero(shmaddr,SHM_SIZE);
            
        }
    }
    return 0;
}
