/*************************************************************************
   从顶部向下走，每次只能走下面或者右下，走完全程，权值最大为多少
   三角形像这样：
   *
   **
   ***
   ****
   *****
 ************************************************************************/

#include<stdio.h>
#define MAX 51

int triangle[MAX][MAX] = {0};
int dp[MAX][MAX] = {0};

int max_val(int a, int b)
{
	return a > b ? a : b;
}
int main()
{
	int n;
	puts("请输入三角形规模：");
	scanf("%d", &n);
	for (int i = 1; i <= n; i++)
	{
		printf("请输入%d个数字\n", i);
		for (int j = 1; j <= i; j++)
		{
			scanf("%d", &triangle[i][j]);
		}
	}	
	for (int i = n; i >= 1; i++)
	{
		for (int j = i; j >= 1; j--)
		{
			dp[i][j] = max_val(dp[i+1][j], dp[i+1][j+1])+triangle[i][j];
		}
	}
	printf("最大权值为%d\n", dp[1][1]);
	return 0;
}
