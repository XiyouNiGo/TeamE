/*************************************************************************
最长公共子序列问题（LCS）
给定字符串1和字符串2，求LCS
最长公共子序列就是两个或者多个串中，最长的相同的子序列
子序列可以不连续，但是有先后的次序关系
 ************************************************************************/

#include<stdio.h>
#include<string.h>
int max_val(int a, int b)
{
	return a > b ? a : b;
}
char *s_gets(char *str, int n)
{
	char *ret_val;
	int i = 0;

	if (ret_val = fgets(str, n, stdin))
	{
		while (str[i] != '\0' && str[i] != '\n')
			i++;
		if (str[i] == '\n')
			str[i] = '\0';
		else
			while(getchar() != '\n')
				continue;
	}

	return ret_val;
}
int main()
{
	char str1[50], str2[50];
	puts("请输入字符串1：");
	s_gets(str1, 50);
	puts("请输入字符串2：");
	s_gets(str2, 50);
	int len1 = strlen(str1), len2 = strlen(str2);
	int dp[len1+1][len2+1];
	for (int i = 0; i <= len1+1; i++)
	{
		for (int j = 0; j <= len2+1; j++)
		{
			if (i == 0 || j == 0)
				dp[i][j] = 0;
			else
			{
				if (str1[i] == str2[j])
					dp[i][j] = dp[i-1][j-1]+1;
				else
					dp[i][j] = max_val(dp[i-1][j], dp[i][j-1]);
			}
		}
	}
	printf("字符串%s和%s的最大公共子序列长为%d\n", str1, str2, dp[len1][len2]);
	return 0;
}
