
#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
	int id;
	int password;
	Node *next;
}Node, *Linklist;

Linklist createLinklist(int n, int m)
{
	Linklist L = NULL;
	Node *pNew, *pLast;	//新节点、尾节点
	int i, password;
	for (i = 1; i<= n; i++)
	{
		//创建新节点
		scanf("%d", &password);
		pNew = (Node*)malloc(sizeof(Node));
		pNew->next = NULL;
		pNew->id = i;
		pNew->password = password;
		//插入
		if (L == NULL)
		{
			pLast = L = pNew;
		}
		else
		{
			pLast = pLast->next = pNew;
		}
	}
	//循环链表
	if (n >= 1)
		pLast->next = L;
	return L;
}

void printLinklist(Linklist L)
{
	Node *p = L;
	while (p)
	{
		printf("id = %d, password = %d\n", p->id, p->password);
		p = p->next;	
	}
}

void solveProblem(Linklist L, int m)
{
	Node *pCur = L, *pDel, *pPrev;
	int i, count = m;
	while (pCur != NULL)
	{
		//找到需要删除的节点
		for (i = 1; i <= count - 1; i++)
		{
			pPrev = pCur;
			pCur = pCur->next;
		}
		pDel = pCur;
		//删除节点
		if (pCur == pCur->next)
		{
			L = NULL;
		}
		else
		{
			pPrev->next = pCur->next;
			pCur = pCur->next;
		}
        if (pDel)
        {
		    count = pDel->password;
		    printf("%d ", pDel->id);
		    free(pDel);
        }
    }
}

int main()
{
	int n, m;
	scanf("%d %d", &n, &m);
	Linklist L = createLinklist(n, m);
	solveProblem(L, m);
	return 0;
}
