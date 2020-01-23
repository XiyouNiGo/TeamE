/*************************************************************************
    > File Name: N.c
    > Author: zll
    > Mail: zhnllion@126.com 
    > Created Time: 2020年01月19日 星期日 21时34分01秒
 ************************************************************************/

#include<stdio.h>
int main()
{
	int n,i,j,count;
	while(scanf("%d",&n)!=EOF)
	{
		int arr[31];
		arr[0]=1;
		for(i=1;i<n;i++)
		{
			arr[i]=0;
		}
		for(i=0;i<n;i++)
		{
			for(j=i;j>=1;j--)
			{
				arr[j]+=arr[j-1];
			}
			for(j=0,count=i;j<=i;j++)
			{
				if(j==count) printf("%d\n",arr[j]);
				else printf("%d ",arr[j]);
			}
		}
		printf("\n");
	}
}
