#include<stdio.h>
#define sixty 60
int main()
{
	int minutes;
	while(printf("请输入需要转化的分钟数:\n"),scanf("%d",&minutes)!=EOF)
	{
		if(minutes<=0) break;
		printf("%d分钟即%d小时%d分钟\n",minutes,minutes/sixty,minutes%60);
	}
	return 0;
}

