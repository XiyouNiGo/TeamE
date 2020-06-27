
/*************************************************************************
    > File Name: static_linkedlist.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年06月27日 星期六 20时03分08秒
 ************************************************************************/
//静态链表
#include <stdio.h>
#define ERROR 1
#define OK 0
#define MAXSIZE 1000

typedef int elemtype;
typedef int status;
//第一个元素的cursor用于存储备用链表第一个结点的位置
//最后一个元素相当于头结点
typedef struct
{
    elemtype data;
    int cursor;    //游标(0表示无指向)
}component, static_linklist[MAXSIZE];

int malloc_sll(static_linklist space)
{
    int i = space[0].cursor;
    if (i)
    {
        space[0].cursor = space[i].cursor;
    }
    return i;   //返回空闲下标的值
}

int free_sll(static_linklist space, int k)
{
    space[k].cursor = space[0].cursor;
    space[0].cursor = k;
}

int length_list(static_linklist L)
{
    int j = 0;
    int i = L[MAXSIZE - 1].cursor;
    while (i)
    {
        i = L[i].cursor;
        i++;
    }
    return j;
}
//将一维数组space中各分量链成一备用链表
status init_list(static_linklist space)
{
    int i;
    for (i = 0; i < MAXSIZE - 1; i++)
    {
        space[i].cursor = i + 1;
    }
    //当前静态链表为空
    space[MAXSIZE - 1].cursor = 0;
    return OK;
}
//在L中的第i个元素前插入数据元素e
status insert_list(static_linklist L, int i, elemtype e)
{
    int j, k, l;
    k = MAXSIZE - 1;
    if (i < 1 || i > length_list(L) + 1)
    {
        return ERROR;
    }
    j = malloc_sll(L);
    if (j)
    {
        L[j].data = e;
        for (l = i; l <= i - 1; i++)
        {
            k = L[k].cursor;
        }
        L[j].cursor = L[k].cursor;
        L[k].cursor = j;
        return OK;
    }
    return ERROR;
}

status delete_link(static_linklist L, int i)
{
    int j, k;
    if (i < 1 || i > length_list(L))
    {
        return ERROR;
    }
    k = MAXSIZE - 1;
    for (j = 1; j < i - 1; j++)
    {
        k = L[k].cursor;
    }
    j = L[k].cursor;
    L[k].cursor = L[j].cursor;
    free_sll(L, j);
    return OK;
}

