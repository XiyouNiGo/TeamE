/*************************************************************************
	> File Name: msg.c
	> Author: 
	> Mail: 
	> Created Time: 2020年07月08日 星期三 10时30分50秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<signal.h>

#define MSG_SIZE 1024 
#define MSG_FILE "./msgfile"
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
    if((msgid = msgget(key,0664 | IPC_CREAT)) == -1)
        print_err("msgget failed");
    msgctl(msgid,IPC_RMID,NULL);
    exit(-1);

}
int creat_or_get_msgque(void)
{
    int msgid;
    key_t key;
    int fd;
    if((fd = open(MSG_FILE,O_CREAT | O_RDWR,0664)) == -1)
        print_err("open failed");
    if((key = ftok(MSG_FILE,'a')) == -1)
        print_err("ftok failed");
    if((msgid = msgget(key,0664 | IPC_CREAT)) == -1)
        print_err("msgget failed");
    return msgid;
}
int main(int argc,char ** argv)
{
    signal(SIGINT,signal_fun);
    if(argc != 2)
    {
        printf("./a.out recv_msgtype\n");
        return 0;
    }
    int recv_msgtype = atol(argv[1]);


    int msgid = -1;
    msgid = creat_or_get_msgque();
    
    printf("msgid = %d\n",msgid);
    int ret = fork();
    if(ret > 0)
    {
        struct msgbuf msg_buf = { 0 };
        while(1)
        {
            bzero(&msg_buf,sizeof(msg_buf));
            //封装消息包0 从键盘输入，相当于scanf
            //read(0,&msg_buf.mtext,sizeof(msg_buf.mtext));
            scanf("%s",msg_buf.mtext);
            printf("input recv_msgtype\n");
            scanf("%ld",&msg_buf.mtype);
           // read(0,&msg_buf.mtype,sizeof(msg_buf.mtype));
            //发送消息包
            msgsnd(msgid,&msg_buf,MSG_SIZE,0);
        }
    }

    else if(ret == 0)
    {
       struct msgbuf msg_buf = { 0 } ;
        while(1)
        { 
            //清空结构体
            bzero(&msg_buf,sizeof(msg_buf));
            int ret = msgrcv(msgid,&msg_buf,MSG_SIZE,recv_msgtype,0);
            if(ret > 0)
            {
                printf("%s\n",msg_buf.mtext);
            }
        }
    }
    return 0;
}

