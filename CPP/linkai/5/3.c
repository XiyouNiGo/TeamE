#include<stdio.h>
int main()
{
	int days;
	while(printf("请输入需要转化的天数:\n"),scanf("%d",&days)!=EOF)
	{
		if(days<=0) break;
		printf("%d days are %d weeks,%d days\n",days,days/7,days%7);
	}
	return 0;
}

