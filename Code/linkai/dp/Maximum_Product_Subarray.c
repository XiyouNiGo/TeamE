/*************************************************************************
给定一串数字，找出连续子串的最大值
 ************************************************************************/

#include<stdio.h>
#include"pre.h"
int main()
{
	int n;
	puts("请输入数字个数：");
	scanf("%d", &n);
	int num[n], dp[n];
	puts("请依次输入数字：");
	for (int i = 0; i < n; i++)
		scanf("%d", &num[i]);
	dp[0] = num[0];
	for (int i = 1; i < n; i++)
	{
		dp[i] = max_val(dp[i-1]+num[i], num[i]);
	}
	printf("最大连续子串的最大值为%d\n", dp[n-1]);
	return 0;
}
