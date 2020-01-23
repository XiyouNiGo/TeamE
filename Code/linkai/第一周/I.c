#include<stdio.h>
int main()
{
	int n,k,count=0,i,p[100];
	scanf("%d%d",&n,&k);
	for(i=0;i<n;i++)
	{
		scanf("%d",&p[i]);
	}
	int left=0,right=n-1;
	while(k>=p[left]&&left<=right)
	{
		left++;
		count++;
	}
	while(k>=p[right]&&left<=right)
	{
		right--;
		count++;
	}
	printf("%d\n",count);
}

