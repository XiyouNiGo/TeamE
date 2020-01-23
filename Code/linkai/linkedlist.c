#include<stdio.h>
typedef struct linkedlist
{
	int data;
	struct linkedlist *next;
}linkedlist;
//创建
linkedlist *creat(int data)
{
	linkedlist *node=(linkedlist*)malloc(sizeof(linkedlist));
	if(node==NULL) printf("创建失败\n");
	else
	{
		node->data=data;
		node->next=NULL;
	}
	return node;
}
//头插
linkedlist *linsert(linkedlist *node,int data)
{
	//先改左边会导致下面的节点丢失
	linkedlist *p=node;                  
	linkedlist *new=(linkedlist *)malloc(sizeof(linkedlist));
	if(new==NULL) printf("创建失败\n");
	else
	{
		new->data=data;
		new->next=NULL;
	}
	new->next=p->next;
	node=new;
}
//尾插
linkedlist *rinsert(linkedlist *node,int data)
{
	linkedlist *p=node;
	while(p->next!=NULL) p=p->next;
	p->next=creat(data);
}
//删除特定数据
int delete_data(linkedlist *node,int data)
{
	linkedlist *p1=node,*p2=node->next;
	while(p2!=NULL)
	{
		if(p2->data==data)
		{
			p1->next=p2->next;
			printf("删除成功!\n");
			free(p2);
			return 1;
		}
		p1=p2;
		p2=p2->next;
	}
	printf("未查找到该数据!\n");
	return 0;
}
//删除第i个节点
int delete_i(linkedlist *node,int i)
{
	linkedlist *p1=node,*p2=node->next;
	while(i--&&p2!=NULL)
	{
		p1=p2;
		p2=p2->next;
	}
	if(i!=0)
	{
		printf("删除失败!\n");
		return 0;
	}
	p1->next=p2->next;
	printf("删除成功!")
	free(p2);
	return 1;
}
//删除所有key值节点
int delete_key(linkedlist *node,int key)
{
	linkedlist *p1=node,*p2=node->next;
	while(p1->next!=NULL)
	{
		if(p2->data==data)
		{
			p1->next=p2->next;
			printf("删除成功!\n");
			free(p2);
		}
		p1=p2;
		p2=p2->next;
	}
}
//修改节点信息
int change(linkedlist *node,int i)
{
	linkedlist *p=node;
	while(i--&&p->next!=NULL);
	if(i==0)
	{
		printf("请输入修改后的值:\n");
		scanf("%d",&p->data);
		printf("修改成功!\n");
		return 1;
	}
	else
	{
		printf("修改失败!\n");
		return 0;
	}
}
