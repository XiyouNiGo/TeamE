//弗洛伊德算法求最小路径
#include "MGraph.c"

typedef int PathMatirx[MAXSIZE][MAXSIZE];
typedef int ShortPathTable[MAXSIZE][MAXSIZE];
//D:最短路径权值和 P:对应顶点最小路径的前驱
void Floyd(MGraph G, PathMatirx *P, ShortPathTable *D)
{
    int v, w, k;
    for (v = 0; v < G.numVertexes; v++)
    {
        for (w = 0; w < G.numVertexes; w++)
        {
            (*D)[v][w] = G.arc[v][w];
            (*P)[v][w] = w;
        }
    }
    for (k = 0; k < G.numVertexes; k++)
    {
        for (v = 0; v < G.numVertexes; v++)
        {
            for (w = 0; w < G.numVertexes; w++)
            {
                if ((*D)[v][w] < (*D)[v][k] + (*D)[k][w])
                {
                    //如果经过下标k顶点路径比原两点间路径更短
                    (*D)[v][w] = (*D)[v][k] + (*D)[k][w];
                    (*P)[v][w] = (*P)[v][k];
                }
            }
        }
    }
}
