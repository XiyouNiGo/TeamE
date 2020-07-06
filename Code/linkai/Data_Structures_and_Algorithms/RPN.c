
/*************************************************************************
    > File Name: RPN.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年07月03日 星期五 16时30分32秒
 ************************************************************************/
//逆波兰算法
//中缀表达式转化为后缀表达式
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h> 

typedef char Elemtype;

typedef struct StackNode
{
    Elemtype data;
    StackNode *next;
}StackNode;

typedef struct LinkStack
{
    StackNode *top;
    int count;
}LinkStack;

int StackEmpty(LinkStack *S)
{
    return S->count == 0;
}

int Push(LinkStack *S, Elemtype e)
{
    StackNode *new_node = (StackNode*)malloc(sizeof(StackNode));
    new_node->data = e;
    new_node->next = S->top;
    S->top = new_node;
    S->count++;
    return 1;
}

int Pop(LinkStack *S, Elemtype *e)
{
    StackNode *p;
    if (StackEmpty(S))
        return 0;
    *e = S->top->data;
    p = S->top;
    S->top = S->top->next;
    free(p);
    S->count--;
    return 1;
}

int InitStack(LinkStack *S)
{
    S->count = 0;
    S->top = NULL;
    return 1;
}
//判断ch优先级是否不高于栈顶符号(是的话返回1,不是返回0)
int Compare(LinkStack *S, char ch)
{
    char ch_top = S->top->data;
    //左括号优先级设为最低
    if ((ch == '*' || ch == '/') && (ch_top != '*' && ch_top != '/'))
        return 0;
    return 1;
}

int RPN(LinkStack *S, char text[])
{
    Elemtype e;
    int i = 0; //用于text计数
    while (text[i])
    {
        //是数字则直接输出
        while (isdigit(text[i]))
        {
            putchar(text[i]);
            i++;
        }
        //括号匹配
        if (text[i] == ')')
        {
            do
            {
                Pop(S, &e);
                if (e == '(')
                    break;
                else
                    putchar(e);
            }while (1);
        }
        //不必再判断是否结尾,因为数字后必有运算符
        //当栈不为空且栈顶符号优先级低于text[i]
        while (!StackEmpty(S) && Compare(S, text[i]))
        {
            Push(S, text[i]);
        }
        i++;
    }
    while (!StackEmpty(S))
    {
        Pop(S, &e);
        putchar(e);
    }
}

int main(int argc, char **argv)
{
    LinkStack *S = (LinkStack*)malloc(sizeof(LinkStack));
    InitStack(S);
    char text[] = "9+(3-1)*3+10/2";
    RPN(S, text);
    return 0;
}
