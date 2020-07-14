//Kruscal算法生成最小生成树
#include <stdio.h>
#include "MGraph.c"

#define MAXEDGE 1000
#define MAXVEX 1000

typedef struct
{
    int begin;
    int end;
    int weight;
}Edge;

int Find(int *parent, int f)
{
    while (parent[f] > 0)
    {
        f = parent[f];
    }
    return f;
}

void MiniSpanTreee_Kruscal(MGraph G)
{
    int i, n, m;
    Edge edges[MAXEDGE];
    int parent[MAXVEX]; //判断是否形成回路

    //将邻接矩阵G转化为边集数组并按权从小到大排序
    
    for (i = 0; i < G.numVertexes; i++)
        parent[i] = 0;
    for (i = 0; i < G.numEdges; i++)
    {
        n = Find(parent, edges[i].begin);
        m = Find(parent, edges[i].end);
        if (n != m)
        {
            parent[n] = m;
            printf("(%d, %d) %d", edges[i].begin, edges[i].end, edges[i].weight);
        }
    }
}
