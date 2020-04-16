/*************************************************************************
    
 ************************************************************************/
//01背包问题(knapsack)
#include<stdio.h>
#include<string.h>
#define max 1001
int B[max][max];
int w[max],v[max];
int select_max(int num1,int num2)
{
	return (num1>num2 ? num1 : num2);
}
int main()
{
	int T,n,s;
	scanf("%d",&T);
	while(T--)
	{
		memset(B,0,sizeof(B));    //清0
		scanf("%d %d",&n,&s);      //n个配件,s个容量
		for(int i=1;i<=n;i++)      //输入配件信息
		{
			scanf("%d",&v[i]);
		}
		for(int i=1;i<=n;i++)
		{
			scanf("%d",&w[i]);
		}
		for(int i=1;i<=n;i++)      //动态规划,i为编号,j为容量
		{
			for(int j=1;j<=s;j++)
			{
				if(j<w[i])
				{
					B[i][j]=B[i-1][j];
				}
				else
				{
					B[i][j]=select_max(B[i-1][j],B[i-1][j-w[i]]+v[i]);
				}
			}
		}
		printf("%d\n",B[n][s]);
	}
}

