
/*************************************************************************
    > File Name: SqQueue.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年07月06日 星期一 15时33分26秒
 ************************************************************************/
#define MAXSIZE 100

typedef int Elemtype;

typedef struct SqQueue
{
    Elemtype data[MAXSIZE];
    int front;
    int rear;
}SqQueue;

int InitQueue(SqQueue *Q)
{
    Q->front = 0;
    Q->rear = 0;
    return 0;
}

int QueueLength(SqQueue Q)
{
    return (Q.rear - Q.front + MAXSIZE) % MAXSIZE;
}

int EnQueue(SqQueue *Q, Elemtype e)
{
    //留一个空间便于判断队列是否已满
    //如果没有，设置标志量flag辅助判断
    if ((Q->rear + 1) % MAXSIZE == Q->front)
    {
        return 1;
    }
    Q->data[Q->rear] = e;
    Q->rear = (Q->rear + 1) % MAXSIZE;
    return 0;
}

int DeQueue(SqQueue *Q, Elemtype *e)
{
    if (Q->front == Q->rear)
        return 1;
    *e = Q->data[Q->front];
    Q->front = (Q->front + 1) /MAXSIZE;
    return 0;
}


