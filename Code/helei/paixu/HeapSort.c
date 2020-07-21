/*************************************************************************
	> File Name: HeapSort.c
	> Author: 
    > Mail: 
	> Created Time: 2020年06月22日 星期一 23时10分41秒
 ************************************************************************/

#include<stdio.h>
int swap(int tree[],int i,int j)
{
    int temp ;
    temp = tree[i];
    tree[i] = tree[j];
    tree[j] = temp;
}
void  heapify(int tree[],int n,int i)
{
    int max = i;
    int c1 = 2 * i + 1;
    int c2 = 2 * i + 2;
    if(i >=  n)
    {
        return ;
    }
    if(c1 < n && tree[c1] > tree[max])
    {
        max = c1;
    }
    if(c2 < n && tree[c2] > tree[max])
    {
        max = c2;
    }
    if(max != i)
    {
        swap(tree,max,i);
        heapify(tree,n,max);
    }

}
void build_heap(int tree[],int n)
{
    int last_node = n - 1;
    int parent = (last_node - 1) / 2;
    for(int i = parent;i >= 0;i--)
    {
        heapify(tree,n,i);
    }
}
void heap_sort(int tree[],int n)
{
    build_heap(tree,n);
    int i;
    for(int i = n - 1;i >=0;i--)
    {
        swap(tree,i,0);
        heapify(tree,i,0);
    }
}
int main(void)
{
    int tree[] = {4,1,3,5,2,10};
    int n = 6;
    heap_sort(tree,n);
    for(int i = 0;i < n;i++)
    {
        printf("%d\t",tree[i]);
    }
}
