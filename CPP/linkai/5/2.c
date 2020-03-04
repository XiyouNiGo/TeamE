#include<stdio.h>
int main()
{
	int num,i;
	printf("请输入十个整数中最小值:\n");
	scanf("%d",&num);
	for(i=num;i<=num+10;i++)
	{
		printf("%d ",i);
	}
	return 0;
}

