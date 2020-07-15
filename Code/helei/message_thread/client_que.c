/*************************************************************************
	> File Name: server_que.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月10日 星期五 10时18分13秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<signal.h>

#define CLIENT 1
#define SERVER 2
#define MSG_SIZE  1024
#define MSG_FILE "./msgfile"
//调整两者的发送接受顺序就可以保证数据正确读取
struct msgbuf
{
    long mtype;
    char mtext[MSG_SIZE];
    
};
void print_err(char *str)
{
    perror(str);
    exit(-1);
}
void signal_fun(int signo)
{
    int msgid;
    key_t key;
    if((key = ftok(MSG_FILE,'a')) == -1) 
        print_err("ftok failed");
    if((msgid =msgget(key,0664 | IPC_CREAT)) == -1)
        print_err("msgget failed");
    msgctl(msgid,IPC_RMID,NULL);
    exit(-1);
}

int creat_or_get_msgque(void)
{
    int msgid;
    key_t key;
    int fd;
    if((fd = open (MSG_FILE,O_CREAT | O_RDWR,0664)) == -1)
        print_err("open failed");
    if((key = ftok(MSG_FILE,'a')) == -1) 
        print_err("ftok failed");
    if((msgid =msgget(key,0664 | IPC_CREAT)) == -1)
        print_err("msgget failed");
    return msgid;

}

int main(void)
{
    signal(SIGINT,signal_fun);
    int msgid = -1;
    msgid = creat_or_get_msgque ();
    int ret = fork();
    if (ret > 0)
    {
        struct msgbuf msg_buf = { 0 };
        while(1)
        {
            bzero(&msg_buf,sizeof(msg_buf));
            msg_buf.mtype = CLIENT;
            printf("CLIENT : ");
            fgets(msg_buf.mtext,MSG_SIZE,stdin);
            //scanf("%s",msg_buf.mtext);
            msg_buf.mtext[strlen(msg_buf.mtext)-1] = '\0';
            msgsnd(msgid,&msg_buf,MSG_SIZE,0);
        }
    }

    else if ( ret == 0 )
    {
        struct msgbuf msg_buf = { 0 };
        while(1)
        {
            bzero(&msg_buf,sizeof(msg_buf));
            int ret = msgrcv(msgid,&msg_buf,MSG_SIZE,SERVER,0);
            if(ret > 0)
            {
                printf("SERVER : %s\n",msg_buf.mtext);
            }
        }
    }
}
