#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
//双向链表结点结构体
typedef struct Node
{
    int data;           //数据
    struct Node* prev;  //前驱指针
    struct Node* next;  //后继指针
}Node;
//双向链表结构体
typedef struct Doublelist
{
    Node* head;     //头结点
    Node* tail;     //尾结点
    int size;    //链表长度
}Doublelist;

void menu();
char getch();
Node *create_node();
Doublelist *init_list();
void free_list(Doublelist *list);
void insert_head(Doublelist *list);
void insert_tail(Doublelist *list);
void delete_head(Doublelist *list);
void delete_tail(Doublelist *list);
void show_list(Doublelist *list);
void modify_node(Doublelist *list);
Node *get_node_by_index(Doublelist *list, int index);
void *print_node_by_index(Doublelist *list);
int main()
{
	int choice;
	Doublelist *list = init_list();
	menu();	
	while (scanf("%d%*c", &choice) == 1 && choice != 0)
	{
		switch (choice)
		{
			case 1: insert_head(list);
					break;
			case 2: insert_tail(list);
					break;
			case 3: delete_head(list);
					break;
			case 4: delete_tail(list);
					break;
			case 5: free_list(list);
					break;
			case 6: show_list(list);
					break;
			case 7: modify_node(list);
					break;
			case 8: print_node_by_index(list);
					break;
			default: puts("输入错误！");
					 system("sleep 2s");
		}
		menu();
	}
	return 0;
}
//初始化链表
Doublelist *init_list()
{
	Doublelist *list = (Doublelist*)malloc(sizeof(Doublelist));
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	return list;
}
//创建节点
Node *create_node()
{
	Node *node = (Node*)malloc(sizeof(Node));
	puts("请输入节点的值：");
	scanf("%d%*c", &(node->data));
	node->prev = NULL;
	node->next = NULL;
	return node;
}
//释放链表
void free_list(Doublelist *list)
{
	while (list->size)
	{
		Node *temp = list->head;
		list->head = list->head->next;
		free(temp);
		temp = NULL;
		list->size--;
	}
}
//头插法插入节点
void insert_head(Doublelist *list)
{
	Node *new_node = create_node();
	if (list->size == 0)
	{
		list->head = new_node;
		list->tail = new_node;
	}
	else
	{
		new_node->next = list->head;
		list->head->prev = new_node;
		list->head = new_node;
	}
	list->size++;
}
//尾插法插入节点
void insert_tail(Doublelist *list)
{
	Node *new_node = create_node();
	if (list->size == 0)
	{
		list->head = new_node;
		list->tail = new_node;
	}
	else
	{
		list->tail->next = new_node;
		new_node->prev = list->tail;
        list->tail = new_node;
	}
	list->size++;
}
//删除头结点
void delete_head(Doublelist *list)
{
	if (list->size == 0)
	{
		puts("链表为空！");
		return;
	}
	if (list->size == 1)
	{
		free(list->head);
		list->head = NULL;
		list->tail = NULL;
		list->size--;
		return;
	}
	Node *p = list->head;
	list->head = p->next;
	list->head->prev = NULL;	//如果只有一个节点那这句变成段错误,另一个差别是多个节点不需要修改尾指针和prev
	free(p);
	p = NULL;
	list->size--;
}
//删除尾节点
void delete_tail(Doublelist *list)
{
	if (list->size == 0)
	{
		puts("链表为空！");
		return;
	}
	if (list->size == 1)
	{
		free(list->head);
		list->head = NULL;
		list->tail = NULL;
		list->size--;
		return;
	}
	Node *p = list->tail;
	list->tail = p->prev;
	list->tail->next = NULL;
	free(p);
	p = NULL;
	list->size--;
}
//浏览链表
void show_list(Doublelist *list)
{
	int num = list->size;
	Node *p = list->head;
	if(num == 0)
	{
		puts("链表为空！");
		getch();
		return;
	}
	for (int i = 0; i < num; i++)
	{
		printf("节点%d： %d\n", i+1, p->data);
		p = p->next;
	}
	getch();
}
//菜单
void menu()
{
	system("clear");
	printf("**************************************\n");
	printf("%-30s%-30s\n", "1.头插法插入节点", "2.尾插法插入节点");
	printf("%-30s%-30s\n", "3.删除头结点", "4.删除尾节点");
	printf("%-30s%-30s\n", "5.释放链表", "6.浏览链表");
	printf("%-30s%-30s\n", "7.按序号修改节点", "8.按序号查找节点");
	printf("**************************************\n");
}

char getch()
{
	char c;
    system("stty -echo");
    system("stty -icanon");
    c=getchar();
    system("stty icanon");
    system("stty echo");
    return c;
}

void modify_node(Doublelist *list)
{
	int index, data;
	Node *p;
	puts("请输入要修改的节点下标：");
	scanf("%d%*c", &index);
	puts("请输入需要修改的值：");
	scanf("%d%*c", &data);
	if (p = get_node_by_index(list, index))
	{
		p->data = data;
	}	
	return;
}

Node *get_node_by_index(Doublelist *list, int index)
{
	Node *p;
	if (index > list->size || index <= 0)
	{
		puts("下标错误！");
		getch();
		return NULL;
	}
	if (index < list->size/2)
	{
		p = list->head;
		for (int i = 1; i < index; i++)
		{
			p = p->next;
		}
	}
	else
	{
		p = list->tail;
		for (int i = 1; i < list->size-index+1; i++)
		{
			p = p->prev;
		}
	}	
	return p;
}

void *print_node_by_index(Doublelist *list)
{
	Node *p;
	int index;
	puts("请输入需要查找的节点下标：");
	scanf("%d", &index);
	if (p = get_node_by_index(list, index))
	{
		printf("节点%d： %d\n", index, p->data);
	}
}
