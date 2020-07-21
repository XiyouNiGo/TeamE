/*************************************************************************
	> File Name: tree.c
	> Author: 
	> Mail: 
	> Created Time: 2020年06月26日 星期五 08时58分52秒
 ************************************************************************/

#include<stdio.h>
#include<malloc.h>
struct tree
{
    char data;
    struct tree * pLchild;
    struct tree * pRchild;
};
struct tree * createtree(void)
{
    struct tree * pA = (struct tree *)malloc(sizeof(struct tree));
    struct tree * pB = (struct tree *)malloc(sizeof(struct tree));
    struct tree * pC = (struct tree *)malloc(sizeof(struct tree));
    struct tree * pD = (struct tree *)malloc(sizeof(struct tree));
    struct tree * pE = (struct tree *)malloc(sizeof(struct tree));
    pA -> data = 'A';
    pB -> data = 'B';
    pC -> data = 'C';
    pD -> data = 'D';
    pE -> data = 'E';
    pA->pLchild = pB;
    pA->pRchild = pC;
    pB->pLchild = pB->pRchild = NULL;
    pC->pLchild = pD;
    pC->pRchild = pD->pLchild = NULL;
    pD->pRchild = pE;
    pE->pRchild = pE->pLchild = NULL;
    return pA;

}
void PrevTraverse(struct tree * p)
{
    if(p != NULL)
    {
        printf("%c\t",p -> data);
        PrevTraverse(p -> pLchild);
        PrevTraverse (p -> pRchild);
    }
}
void InTraverse(struct tree * p)
{
    if(p != NULL)
    {
        InTraverse(p -> pLchild);
        printf("%c\t",p -> data);
        InTraverse (p -> pRchild);
    }
}
void PostTraverse(struct tree * p)
{
    if(p != NULL)
    {
        PostTraverse(p -> pLchild);
        PostTraverse (p -> pRchild);
        printf("%c\t",p -> data);
    }
}
int main(void)
{
    struct tree * pHead = createtree();
    printf("先序：");
    PrevTraverse(pHead);
    printf("中序：");
    InTraverse (pHead);
    printf("后序:");
    PostTraverse(pHead);
}

