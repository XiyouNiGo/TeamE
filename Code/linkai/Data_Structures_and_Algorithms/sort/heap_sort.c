
/*************************************************************************
    > File Name: heap_sort.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年05月31日 星期日 14时59分37秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>

void swap(int tree[], int i, int j)
{
    int temp = tree[i];
    tree[i] = tree[j];
    tree[j] = temp;
}
//前提是所有子树都是堆
void heapify(int tree[], int n, int i)
{
    if (i >= n)
        return;

    int c1 = 2 * i + 1;
    int c2 = 2 * i + 2;
    int max = i;

    if (c1 < n && tree[c1] > tree[max])
    {
        max = c1;
    }
    if (c2 < n && tree[c2] > tree[max])
    {
        max = c2;
    }
    if (max != i)
    {
        swap(tree, max, i);
        heapify(tree, n, max);  //因为交换后的节点不一定比孙节点大，对子节点再做一次heapify
    }
}

void build_heap(int tree[], int n)
{
    int last_node = n-1;
    int parent = (last_node-1) / 2;
    
    for (int i = parent; i >= 0; i--)
    {
        heapify(tree, n, i);
    }
}

void heap_sort(int tree[], int n)
{
    build_heap(tree, n);
    for (int i = n-1; i > 0; i--)
    {
        swap(tree, i, 0);
        heapify(tree, i, 0);
    }
}
int main(int argc, char *argv[])
{
    int tree[8];

    printf("Enter random 8 numer:\n");
    for (int i = 0; i < 8; i++)
    {
        scanf("%d", &tree[i]);
    }

    printf("Before:");
    for (int i = 0; i < 8; i++)
        printf(" %d", tree[i]);
    putchar('\n');

    heap_sort(tree, 8);

    printf("After:");
    for (int i = 0; i < 8; i++)
        printf(" %d", tree[i]);
    putchar('\n');

    return 0;
}
