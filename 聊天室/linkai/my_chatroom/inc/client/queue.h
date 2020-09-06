#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "packet_type.h"
#define MAXSIZE_QUEUE 20

typedef struct packet Elemtype;

typedef struct SqQueue
{
    Elemtype data[MAXSIZE_QUEUE];
    int front;
    int rear;
}SqQueue;

extern SqQueue *Q;

int InitQueue(SqQueue *Q);

int QueueLength(SqQueue Q);

int EnQueue(SqQueue *Q, Elemtype e);

int DeQueue(SqQueue *Q, Elemtype *e);

#endif
