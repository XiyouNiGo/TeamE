/*************************************************************************
    > File Name: A.c
    > Author: zll
    > Mail: zhnllion@126.com 
    > Created Time: 2020年01月17日 星期五 21时09分26秒
 ************************************************************************/
#include<stdio.h>
int main()
{
    int N,i,t,sum;
    while(scanf("%d",&N)!=EOF)
    {
        if(N==0)
          return;
        for(i=0,sum=0;i<N;i++)
        {
            scanf("%d",&t);
            sum+=t;
        }
        printf("%d\n",sum);
    }
    return 0;
 }

