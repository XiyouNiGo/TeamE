//迪杰斯特拉算法生成最短路径
//求解有向网G的v0顶点到其余顶点v最短路径P[v]及带权长度D[v]
//P[v]:前驱下标顶点，D[v]:v0到v的最短路径长度

#include <stdio.h>
#include "MGraph.c"

#define  MAXVEX 9

typedef int Patharc[MAXVEX];    //存储最短路径下标
typedef int ShortPathTable[MAXVEX]; //最短路径权值和

void Dijkstra(MGraph G, int v0, Patharc *P, ShortPathTable *D)
{
    int v, w, k, min;
    int final[MAXVEX];  //final[w]=1表示求得v0至vw最短路径
    for (v = 0; v < G.numVertexes; v++)
    {
        final[v] = 0;
        (*D)[v] = G.arc[v0][v]; //将与v0有连线的顶点加上权值
        (*P)[v] = 0;
    }
    (*D)[v0] = 0;
    final[v0] = 1;
    //每次求得v0到某个顶点的最短路径
    for (v = 1; v < G.numVertexes; v++)
    {
        min = INFINITY;
        for (w = 0; w < G.numVertexes; w++)
        {
            if (!final[w] && (*D)[w] < min)
            {
                k = w;
                min = (*D)[w];
            }
        }
        final[k] = 1;
        //修正当前最短路径及距离
        for (w = 0; w < G.numVertexes; w++)
        {
            if (!final[w] && (min + G.arc[k][w] < (*D)[w]))
            {
                (*D)[w] = min + G.arc[k][w];
                (*P)[w] = k;
            }
        }
    }
}
