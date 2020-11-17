
/*************************************************************************
    > File Name: try.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年10月27日 星期二 20时34分27秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 100

typedef struct BiTNode
{
	char data;
	struct BiTNode *lchild, *rchild;
}BiTNode, *BiTree;

typedef struct{
	BiTNode data[MAX_SIZE];
	int top;
}SeqStack;

void InitStack(SeqStack *S)
{
	S->top = -1;
}

int IsEmpty(SeqStack *S)
{
	return S->top == -1;
}

int Push(SeqStack *S, BiTree p)
{
	if(S->top == MAX_SIZE-1)
	{
		return -1;
	}
	else
	{
		S->top++;
		S->data[S->top] = (*p);
		return 0;
	}
}

int Pop(SeqStack *S, BiTNode *p)
{
	if (S->top == -1)
	{
		return -1;
	}
	else
	{
		*p = S->data[S->top];
		S->top--;
		return 0;
	}
}

int GetTop(SeqStack *S, BiTNode *p)
{
	if(S->top != -1)
	{
		*p = S->data[S->top];
		return 0;
	}
	return -1;
}

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
	SeqStack S;
	InitStack(&S);
	BiTNode *p = T;
	while (p || !IsEmpty(&S))
	{
		if (p)
		{
			visit(p);
			Push(&S, p);
			p = p->lchild;
		}
		else
		{
			Pop(&S, p);
			p = p->rchild;
		}
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

int main(int argc, char **argv)
{
	BiTree T;
	CreateBiTree(&T);
	PreOrder(T);
	putchar('\n');
	InOrder(T);
	putchar('\n');
	return 0;
}
