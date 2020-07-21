/*************************************************************************
	> File Name: BiTree.c
	> Author: 
	> Mail: 
	> Created Time: 2020年06月27日 星期六 10时12分38秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<malloc.h>
typedef struct BiTNode
{
    char data;
    struct BiTNode *lchild,*rchild;
}BiTNode,*BiTree;
/*
 *
* 使用二级指针的原因
*
* 如果使用一级指针，主函数把T交给CreateBiTree ,该函数会自己创建一个指针，而不是使用主函数的指针，当函数销毁，该函数创建的指针也随之销毁，就找不到创建的二叉树
* 使用一级指针，想着是对主函数创建的指针进行了操作，实际上创建函数自己创建一个指针去使用
* */
struct BiTNode * CreateBiTree(BiTNode **T)
{
    char c;
    scanf("%c",&c);
    if(c == ' ')    //利用空格表示该节点为空
    {
        *T = NULL;
    }
    else
    {
        *T = (BiTNode *)malloc(sizeof(BiTNode));
        (*T)->data = c;
        CreateBiTree (&(*T)->lchild);
        CreateBiTree (&(*T)->rchild);
    }
}
void visit(BiTNode *T,int level)
{
    printf("%c 在第%d 层\n",T->data,level);
}
void PreTraverse(BiTNode *T,int level)
{
    if(T != NULL)
    {
        visit(T,level);
        PreTraverse(T->lchild,level+1);
        PreTraverse(T->rchild,level+1);
    }
}
int main(void)
{
    int level = 1;
    BiTNode *T,*Tree;
     CreateBiTree(&T);
    PreTraverse(T,1);
}
