/*************************************************************************
你有三种硬币，分别面值2元，5元和7元，每种硬币都足够多，买一本书需要27元
如何用最少的硬币组合正好付清
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
int min_val(int a, int b)
{
	return (a < b ? a : b);
}
int dp[28] = {0};
int coin[3] = {2, 5, 7};
int main()
{
	dp[0] = 0;
	for (int i = 1; i <= 27; i++)
	{
		dp[i] = INT_MAX;	//存在推不出来的情况，又因为是求最小值，所以我们用INT_MAX作为哑值，可以用0x3f3f3f3f
		for (int j = 0; j < 3; j++)
		{
			if (dp[i-coin[j]] != INT_MAX && i >= coin[j])
				dp[i] = min_val(dp[i-coin[j]]+1, dp[i]);	//能够递推且前一种最佳方案存在
		}
	}
	printf("%d", dp[27]);
	return 0;
}
