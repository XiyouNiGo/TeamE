/*************************************************************************
    > File Name: X.c
    > Author: zll
    > Mail: zhnllion@126.com 
    > Created Time: 2020年01月18日 星期六 20时13分48秒
 ************************************************************************/

#include<stdio.h>
int main()
{
	int n,i,count=0,max=-1,min=1e9+1;
	scanf("%d",&n);
	int v[100005];
	for(i=0;i<n;i++)
	{
		scanf("%d",&v[i]);
		if(v[i]>max) max=v[i];
		if(v[i]<min) min=v[i];
	}
	for(i=0;i<n;i++)
	{
		if(v[i]<max&&v[i]>min) count++;
	}
	printf("%d\n",count);
}

