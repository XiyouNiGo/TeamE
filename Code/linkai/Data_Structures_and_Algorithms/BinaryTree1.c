
/*************************************************************************
    > File Name: try.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年10月27日 星期二 20时34分27秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct BiTNode
{
	char data;
	struct BiTNode *lchild, *rchild;
}BiTNode, *BiTree;

void CreateBiTree(BiTree *T)
{
	char data;
	scanf("%c", &data);
	if (data == '#')
		*T = NULL;
	else
	{
		*T = (BiTree)malloc(sizeof(BiTNode));
		if (!*T)
			exit(-1);
		(*T)->data = data;
		CreateBiTree(&(*T)->lchild);
		CreateBiTree(&(*T)->rchild);
	}
}

void visit(BiTree T)
{
	printf("%c", T->data);
}

void PreOrder(BiTree T)
{
	if (T)
	{
		visit(T);
		PreOrder(T->lchild);
		PreOrder(T->rchild);
	}
}

void InOrder(BiTree T)
{
	if (T)
	{
		InOrder(T->lchild);
		visit(T);
		InOrder(T->rchild);
	}
}

void PostOrder(BiTree T)
{
	if (T)
	{
		PostOrder(T->lchild);
		PostOrder(T->rchild);
		visit(T);
	}
}

int main(int argc, char **argv)
{
	BiTree T;
	CreateBiTree(&T);
	PreOrder(T);
	putchar('\n');
	InOrder(T);
	putchar('\n');
	PostOrder(T);
	putchar('\n');
	return 0;
}
