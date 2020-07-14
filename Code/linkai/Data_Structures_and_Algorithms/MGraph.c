//邻接矩阵
#include <stdio.h>
#include "SqQueue.c"

#define MAXSIZE 100
#define INFINITY 65535  //表示无边(Weight有时为0，不用0表示)

typedef int Boolean;

Boolean visited[MAXSIZE];

typedef char VertexType;

typedef int EdgeType;

typedef struct
{
    VertexType vexs[MAXSIZE];   //顶点表
    EdgeType arc[MAXSIZE][MAXSIZE]; //邻接矩阵
    int numVertexes, numEdges;  //顶点数和边数
}MGraph;

void CreateMGraph(MGraph *G)
{
    int i, j, k, w;
    printf("输入顶点数和边数:\n");
    scanf("%d%d", &G->numVertexes, &G->numEdges);
    for (i = 0; i < G->numVertexes; i++)
        scanf("%c", &G->vexs[i]);
    //初始化邻接矩阵
    for (i = 0; i < G->numVertexes; i++)
        for (j = 0; j < G->numVertexes; j++)
            G->arc[i][j] = INFINITY;
    for (k = 0; k < G->numEdges; k++)
    {
        printf("输入边(vi, vj)的下标i和上标j和权w:\n");
        scanf("%d%d%d", &i, &j, &w);
        G->arc[i][j] = w;
        G->arc[j][i] = G->arc[i][j];   //无向图矩阵对称
    }
}

void DFS(MGraph G, int i)
{
    int j;
    visited[i] = 1;
    printf("%c", G.vexs[i]);
    for (j = 0; j < G.numVertexes; j++)
        if (G.arc[i][j] == 1 && !visited[j])
            DFS(G, j);
}

void DFSTraverse(MGraph G)
{
    int i;
    for (i = 0; i < G.numVertexes; i++)
        visited[i] = 0;
    for (i = 0; i < G.numVertexes; i++)
        if (!visited[i])
            DFS(G, i);
}

void BFSTraverse(MGraph G)
{
    int i, j;
    SqQueue Q;
    for (i = 0; i < G.numVertexes; i++)
        visited[i] = 0;
    InitQueue(&Q);
    for (i = 0; i < G.numVertexes; i++)
    {
        if (!visited[i])
        {
            visited[i] = 1;
            printf("%c", G.vexs[i]);
            EnQueue(&Q, i);
            while (!QueueLength(Q))
            {
                DeQueue(&Q, &i);
                for (j = 0; j < G.numVertexes; j++)
                {
                    if (G.arc[i][j] == 1 && !visited[i])
                    {
                        visited[j] = 1;
                        printf("%c", G.vexs[j]);
                            EnQueue(&Q, j);
                    }
                }
            }
        }
    }
}
