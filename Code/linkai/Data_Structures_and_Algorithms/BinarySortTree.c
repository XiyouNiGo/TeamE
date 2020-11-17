
/*************************************************************************
    > File Name: BinarySortTree.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年11月03日 星期二 20时16分29秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define TRUE 0
#define FALSE -1

typedef int status;
typedef struct BiTNode
{
    int data;
    struct BiTNode *lchild, *rchild;
}BiTNode, *BiTree;
//递归查找二叉排序树T中是否存在ket
//f指向T的双亲(初始为NULL)
//成功返回True，p为该节点
//失败返回False，p为查找路径上最后一个节点
status SearchBST(BiTree T, int key, BiTree f, BiTree *p)
{
    if (!T) //为空
    {
        *p = f;
        return FALSE;
    }
    else if (key == T->data)    //查找成功
    {
        *p = T;
        return TRUE;
    }
    else if (key < T->data) //较小，在左子树找
    {
        return SearchBST(T->lchild, key, T, p);
    }
    else
        return SearchBST(T->rchild, key, T, p);
}
//插入key节点
//若存在，返回TRUE，否则返回FALSE
status InsertBST(BiTree *T, int key)
{
    BiTree p, s;
    if (!SearchBST(*T, key, NULL, &p))  //未查找到
    {
        s = (BiTree)malloc(sizeof(BiTNode));
        s->data = key;
        s->lchild = s->rchild = NULL;
        if (!p) //树空
            *T = s;
        //此时Seaearr
        else if (key < p->data)
            p->lchild = s;
        else
            p->rchild = s;
        return TRUE;
    }
    else    //树中也有key相同节点，不再插入
        return FALSE;
}
//BST删除：
//1.叶子节点直接删
//2.仅左子树或右子树，子承父业
//3.左右都有，找到节点p直接前驱或直接后继s（事实上是中序遍历前后位）
//用s替换p，再删除p（重接）
status Delete(BiTree *p)
{
    BiTree q, s;
    //叶子节点直接删
    if ((*p)->lchild == NULL && (*p)->rchild == NULL)
    {
        free(*p);
        *p = NULL;
    }
    //单边树空只需重接子树
    else if ((*p)->rchild == NULL)
    {
        q = *p;
        *p = (*p)->lchild;
        free(q);
    }
    else if ((*p)->lchild == NULL)
    {
        q = *p;
        *p = (*p)->rchild;
        free(q);
    }
    //左右都不为空(以直接前驱为例)
    else
    {
        q = *p; //此时q指向s的双亲节点（方便删除后重接）
        //先左转，再向右走到尽头（找到直接前驱节点）
        s = (*p)->lchild;
        while (s->rchild)
        {
            q = s;  //q也一起移动的
            s = s->rchild;
        }
        //用s替换p
        (*p)->data = s->data;
        //q为p,也就是说s替换的是s的双亲节点（此时s作为左子树）
        if (q == *p)
            q->lchild = s->lchild;
        //否则接右子树（正常情况）
        else
            q->rchild = s->lchild;
    }
}
//和查找几乎只有一行之差
status DeleteBST(BiTree *T, int key)
{
    if (!*T)
        return FALSE;
    else
    {
        if (key == (*T)->data)
            return Delete(T);
        else if (key < (*T)->data)
            return DeleteBST(&(*T)->lchild, key);
        else
            return DeleteBST(&(*T)->rchild, key);
    }
}

int main(int argc, char *argv[])
{
    
}
