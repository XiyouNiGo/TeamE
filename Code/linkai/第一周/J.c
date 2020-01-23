/*************************************************************************
    > File Name: J.c
    > Author: zll
    > Mail: zhnllion@126.com 
    > Created Time: 2020年01月18日 星期六 20时11分52秒
 ************************************************************************/
#include<stdio.h>
int main()
{
	int n,a,b,max=0,t=0;
	scanf("%d",&n);
	while(n--)
	{
		scanf("%d%d",&a,&b);
		t+=b-a;
		if(t>max) max=t;
	}
	printf("%d\n",max);
}
