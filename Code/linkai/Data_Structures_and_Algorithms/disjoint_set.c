
/*************************************************************************
    > File Name: disjoint_set.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年06月03日 星期三 12时04分25秒
 ************************************************************************/
//并查集

#include <stdio.h>
#include <memory.h>

#define VERTICES 6  //顶点

int get_root(int x, int parent[])
{
    int x_root = x;
    int current = x;
    int temp;
    while (parent[x_root] != -1)
    {
        x_root = parent[x_root];
    }
    //路径压缩算法
    while (current != x_root)
    {
        temp = parent[current]; //记录一下父节点
        parent[current] = x_root;  //当前节点父节点设置为x_root
        current = temp; //重复步骤直到父节点为x_root
    }
    return x_root;
}

int union_vertices(int x, int y, int parent[])
{
    int x_root = get_root(x, parent);
    int y_root = get_root(y, parent);
    if (x_root == y_root)
    {
        return 0;
    }
    else
    {
        parent[x_root] = y_root;
        return 1;
    }
}

int main(int argc, char *argv[])
{
    int flag = 0;
    int parent[VERTICES];
    int edges[6][2] = {
        {0, 1}, {1, 2}, {1, 3},
        {3, 4}, {2, 5}
    };
    
    //用-1表示该点根节点是自己
    memset(&parent, -1, sizeof(parent));    //只有0和-1会得到正确的结果（每位都相同）

    for (int i = 0; i < 5; i++)
    {
        if (union_vertices(edges[i][0], edges[i][1], parent) == 0)  //同根节点说明有环
        {
            printf("Cycle found\n");
            flag = 1;
            break;
        }
    }

    if (flag == 0)
    {
        printf("No cycle found\n");
    }

    return 0;
}
