
/*************************************************************************
    > File Name: tree.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年07月06日 星期一 20时18分09秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
/*双亲表示法
#define MAXSIZE 100
typedef int Elemtype;
typedef  struct PTNode
{
    Elemtype data;
    int parent;
}
typedef struct
{
    PTNode nodes[MAXSIZE];
    int r, n;   //根位置和结点数
}PTree;
*/

/* 孩子表示法(孩子兄弟表示法只需在每个结点加上一个表示右兄弟的指针,类似二叉树)
#define MAXSIZE 100
typedef struct CTNode
{
    int child;
    struct CTNode *next;
}CTNode;
typedef struct CTBox
{
    Elemtype data;
    CTNode *firstchild;
}CTBox;
typedef struct
{
    CTBox nodes[MAXSIZE];
    int r, n;
}CTree;
*/
typedef char Elemtype;

typedef struct BiTNode
{
    Elemtype data;
    struct BiTNode *lchild, *rchild;
}BiTNode, *BiTree;
//前序遍历
void PreOrderTraverse(BiTree T)
{
    if (T == NULL)
    {
        return;
    }
    //“访问”
    printf("%c", T->data);
    PreOrderTraverse(T->lchild);
    PreOrderTraverse(T->rchild);
}
//中序遍历
void InOrderTraverse(BiTree T)
{
    if (T == NULL)
    {
        return;
    }
    InOrderTraverse(T->lchild);
    printf("%c", T->data);
    InOrderTraverse(T->rchild);
}

void PostOrderTraverse(BiTree T)
{
    if (T == NULL)
    {
        return;
    }
    PostOrderTraverse(T->lchild);
    PostOrderTraverse(T->rchild);
    printf("%c", T->data);
}

void CreateBiTree(BiTree *T)
{
    Elemtype ch;
    scanf("%c", &ch);
    //虚结点
    if (ch == '#')
        *T = NULL;
    else
    {
        if ((*T = (BiTree)malloc(sizeof(BiTNode)) ) == 0)
            exit(1);
        (*T)->data = ch;
        CreateBiTree(&(*T)->lchild);
        CreateBiTree(&(*T)->rchild);
    }
}
/* 线索二叉树
typedef enum {Link, Thread} PointeRTag;

typedef struct BiThrNode
{
    Elemtype data;
    struct BiThrNode *lchild, *rchild;
    PointeRTag LTag;
    PointeRTag RTag;
}BiThrNode, *BiThrTree;

BiThrTree pre;
//中序线索化
void InThreading(BiThrTree p)
{
    if (p)
    {
        InThreading(p->lchild);

        if (!p->lchild)
        {
            p->LTag = Thread;
            p->lchild = pre;
        }
        if (!pre->rchild)
        {
            pre->RTag = Thread;
            pre->rchild = p;
        }
        pre = p;

        InThreading(p->rchild);
    }
}
//T指向头结点，头结点lchild指向根节点，rchild指向中序遍历的最后一个节点
int InOrderTraverse_Thr(BiThrTree T)
{
    BiThrTree p = T->lchild;    //指向根节点
    while (p != T)  //空树或遍历结束
    {
        //找到中序序列的第一个节点
        while (p->LTag == Link)
            p = p->lchild;
        printf("%c", p->data);
        while (p->RTag == Thread && p->rchild != T)
        {
            p = p->rchild;
            printf("%c", p->data);
        }
        p = p->rchild;
    }
    return 1;
}
*/
