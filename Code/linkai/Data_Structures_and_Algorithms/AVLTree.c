
/*************************************************************************
    > File Name: AVLTree.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年11月03日 星期二 22时14分04秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define TRUE 0
#define FALSE -1
#define LH +1   //左高
#define EH 0    //等高
#define RH -1   //右高

typedef int status;
typedef struct BiTNode
{
    int data;   //数据域
    int bf;     //平衡因子（左子树深度减右子树深度）
    struct BiTNode *lchild, *rchild;
}BiTNode, *BiTree;
//右旋（LL，旋转领导和小弟)
void R_Rotate(BiTree *p)
{
    BiTree L;                   //L小弟，p领导
    L = (*p)->lchild;           //L指向p左子树根节点
    (*p)->lchild = L->rchild;   //L右子树挂在p左子树上（L右子树比L大比p小）
    L->rchild = (*p);           //p挂在L右子树上（右旋）
    *p = L;                     //将根节点修改为L
}
//左旋（RR，旋转领导和小弟）
void L_Rotate(BiTree *p)
{
    BiTree R;                   //R小弟，p领导
    R = (*p)->rchild;           //R指向p右子树根节点
    (*p)->rchild = R->lchild;   //R左子树挂在p右子树上（R左子树比R小比p大）
    R->lchild = (*p);           //p挂在R左子树上（左旋）
    *p = R;                     //将根节点修改为R
}
//对指针T所指树做左平衡处理(包括LL和LR)
void LeftBalance(BiTree *T)
{
    
}

int main(int argc, char *argv[])
{

}

