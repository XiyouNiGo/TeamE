//邻接表(类似孩子表示法)
#include <stdio.h>
#include <stdlib.h>
#include "SqQueue.c"

#define MAXSIZE 100

typedef char VertexType;

typedef int EdgeType;

typedef struct EdgeNode
{
    int adjvex;
    EdgeType weight;    //权值,非网图不需要
    struct EdgeNode *next;
}EdgeNode;

typedef struct VertexNode
{
    VertexType data;
    EdgeNode *firstedge;
}VertexNode, AdjList[MAXSIZE];  //数组类型

typedef struct
{
    AdjList adjlist;
    int numVertexes, numEdges;
}GraphAdjList;

int visited[MAXSIZE];

void CreateALGraph(GraphAdjList *G)
{
    int i, j, k;
    EdgeNode *e;
    printf("输入顶点数和边数:\n");
    scanf("%d%d", &G->numVertexes, &G->numEdges);
    for (i = 0; i < G->numVertexes; i++)
    {
        scanf("%c", &G->adjlist[i].data);
        G->adjlist[i].firstedge = NULL;
    }
    for (k = 0; k < G->numEdges; k++)
    {
        printf("输入边(vi, vj)上的顶点序号:\n");
        scanf("%d%d", &i, &j);
        e = (EdgeNode*)malloc(sizeof(EdgeNode));
        e->adjvex = j;
        e->next = G->adjlist[i].firstedge;
        G->adjlist[i].firstedge = e;
        e = (EdgeNode*)malloc(sizeof(EdgeNode));
        //无向图对称
        e->adjvex = i;
        e->next = G->adjlist[j].firstedge;
        G->adjlist[j].firstedge = e;
    }
}

void DFS(GraphAdjList GL, int i)
{
    EdgeNode *p;
    visited[i] = 1;
    printf("%c", GL.adjlist[i].data);
    p = GL.adjlist[i].firstedge;
    while (p)
    {
        if (!visited[p->adjvex])
            DFS(GL, p->adjvex);
        p = p->next;
    }
}

void DFSTraverse(GraphAdjList GL)
{
    int i;
    for (i = 0; i < GL.numVertexes; i++)
    {
        visited[i] = 0;
    }
    for (i = 0; i< GL.numVertexes; i++)
        if (!visited[i])
            DFS(GL, i);
}

void BFSTraverse(GraphAdjList GL)
{
    int i;
    EdgeNode *p;
    SqQueue Q;
    for (i = 0; i < GL.numVertexes; i++)
        visited[i] = 0;
    InitQueue(&Q);
    for (i = 0; i < GL.numVertexes; i++)
    {
        if (!visited)
        {
            visited[i] = 1;
            printf("%c", GL.adjlist[i].data);
            EnQueue(&Q, i);
            while (!QueueLength)
            {
                DeQueue(&Q, &i);
                p = GL.adjlist[i].firstedge;
                while (p)
                {
                    if (!visited[p->adjvex])
                    {
                        visited[p->adjvex] = 1;
                        printf("%c", GL.adjlist[p->adjvex].data);
                        EnQueue(&Q, p->adjvex);
                    }
                }
            }
        }
    }
}
