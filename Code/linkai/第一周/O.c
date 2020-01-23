//数字游戏
#include<stdio.h>
#include<string.h>
int getnum(int num)
{
	int ret=0;
	if(num<10) return num;
	while(num>0)
	{
		ret+=num%10;
		num/=10;
	}
	return getnum(ret);
}
int main()
{
	char num[1001];
	int ret,i,len;
	while(scanf("%s",num)!=EOF)
	{
	    len=strlen(num),ret=0;
		if(*num=='0') return 0;
		for(i=0;i<len;i++)
		{
			ret+=num[i]-'0';
		}
		printf("%d\n",getnum(ret));
		memset(num,'\0',sizeof(num));
	}
}

