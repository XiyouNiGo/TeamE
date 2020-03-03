/*************************************************************************
最长上升序列问题(LIS)    
 ************************************************************************/

#include<stdio.h>
#include"pre.h"

int main()
{
	int n, ans = 0;
	puts("请输入序列长度：");
	scanf("%d", &n);
	int arr[n], dp[n];
	for (int i = 0; i < n; i++)
		scanf("%d", &arr[i]);
	for (int i = 0; i < n; i++)
	{
		dp[i] = 1;
		for (int j = 0; j < i; j++)
		{
			if (arr[j] < arr[i])
			{
				dp[i] = max_val(dp[i], dp[j]+1);
			}
		}
	}
	for (int i = 0; i < n; i++)
		ans = max_val(ans, dp[i]);
	printf("最长上升序列长为%d\n", ans);
	return 0;
}
