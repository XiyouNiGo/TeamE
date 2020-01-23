//完美的人生没考试
#include<stdio.h>
int main()
{
	int n,m,i,j,count=0,max,t,x;
	scanf("%d%d",&n,&m);
	int stu[100]={0};
	char mark[100][100];
	for(i=0;i<n;i++)
	{
		scanf("%s",mark[i]);
	}
	for(i=0;i<m;i++)
	{
		max=0;
		for(j=0;j<n;j++)
		{
			if(max<mark[j][i]-'0')
			{
				max=mark[j][i]-'0';
				t=j;
			}
		}
		for(x=0,j=0;j<n;j++)
		{
			if(max==mark[j][i]-'0') x++;
		}
		if(x>1) continue;
		stu[t]++;
	}
	for(i=0;i<n;i++)
	{
		if(stu[i]>0) count++;
	}
	printf("%d\n",count);
}
