//圆桌会议
#include<stdio.h>
int main()
{
	int N,x,y;
	while(scanf("%d",&N)!=EOF)
	{
		x=N/2;
		y=N-x;
		printf("%d\n",x*(x-1)/2+y*(y-1)/2);
	}
}

