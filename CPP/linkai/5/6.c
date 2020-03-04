#include<stdio.h>
int main()
{
	int sum=0,num;
	scanf("%d",&num);
	printf("在%d天里能赚",num);
	while(num--)
	{
		sum+=num*num;
	}
	printf("$%d",sum);
	return 0;
}
