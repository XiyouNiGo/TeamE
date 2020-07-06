
/*************************************************************************
    > File Name: try.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年07月06日 星期一 19时57分27秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
typedef char Elemtype;

typedef struct StackNode
{
    Elemtype data;
    StackNode *next;
}StackNode;

typedef struct LinkStack
{
    StackNode *top;
    int count;
}LinkStack;
void my_err(const char *str)
{
     perror(str);
     exit(1);
}
int InitStack(LinkStack *S)
{
    S->count = 0;
    //S->top->next = NULL;
    return 1;
}
int main(int argc, char *argv[])
{ 
 LinkStack *S;
}
