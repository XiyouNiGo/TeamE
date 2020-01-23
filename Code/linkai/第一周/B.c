/*************************************************************************
    > File Name: B.c
    > Author: zll
    > Mail: zhnllion@126.com 
    > Created Time: 2020年01月17日 星期五 21时12分23秒
 ************************************************************************/

#include<stdio.h>
int main()
{
	int N,i,M,t,sum;
	scanf("%d",&N);
    while(scanf("%d",&M)!=EOF)
    {
        if(M==0)
          return;
        for(i=0,sum=0;i<M;i++)
        {
            scanf("%d",&t);
            sum+=t;
        }
        printf("%d\n",sum);
    }
    
    return 0;
 }

