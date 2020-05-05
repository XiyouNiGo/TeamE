/*************************************************************************
	> File Name: fib.c
	> Author: 
	> Mail: 
	> Created Time: 2020年04月26日 星期日 23时28分23秒
 ************************************************************************/

#include<stdio.h>
int fib(int n)
{
    if(n ==1)
        return 1;
    else if( n == 2 )
        return 1;
    else
        return fib(n-1)+fib(n-2);
}
void PrintFN(int m,int n)
{
    for(int i = m;i<n;i++)
    {
        if(fib(i) >= i )
    }
}
int main(void)
{
    int m,n,t;
    scanf("%d %d %d",&m,&n,&t);
    printf("fib(%d) = %d\n",t,fib(t));
    PrintFN(m,n);
}
