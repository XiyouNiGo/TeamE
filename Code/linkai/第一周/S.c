#include<stdio.h>
void swap(int *a,int *b)
{
	int t=*a;
	*a=*b;
	*b=t;
}
int main()
{
	int n,i,j;
	scanf("%d",&n);
	int num[1000005];
	for(i=0;i<n;i++)
	{
		scanf("%d",&num[i]);
	}
	for(i=0;i<n-1;i++)
	{
		for(j=i;j<n;j++)
		{
			if(num[i]>num[j]) swap(&num[i],&num[j]);
		}
	}
	if(n%2==1) printf("%d\n",num[n/2]);
	else printf("%d\n",num[n/2-1]);
}


