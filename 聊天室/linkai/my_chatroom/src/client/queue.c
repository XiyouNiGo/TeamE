#include "../../inc/client/queue.h"
#include "../../inc/client/client_config.h"

int InitQueue(SqQueue *Q)
{
    Q->front = 0;
    Q->rear = 0;
    return 0;
}

int QueueLength(SqQueue Q)
{
    return (Q.rear - Q.front + MAXSIZE_QUEUE) % MAXSIZE_QUEUE;
}

int EnQueue(SqQueue *Q, Elemtype e)
{
    //留一个空间便于判断队列是否已满
    //如果没有，设置标志量flag辅助判断
    if ((Q->rear + 1) % MAXSIZE_QUEUE == Q->front)
    {
        return -1;
    }
    Q->data[Q->rear] = e;
    Q->rear = (Q->rear + 1) % MAXSIZE_QUEUE;
    return 0;
}

int DeQueue(SqQueue *Q, Elemtype *e)
{
    if (Q->front == Q->rear)
        return -1;
    *e = Q->data[Q->front];
    Q->front = (Q->front + 1) % MAXSIZE_QUEUE;
    return 0;
}


