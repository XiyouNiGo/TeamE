/*************************************************************************
    > File Name: H.c
    > Author: zll
    > Mail: zhnllion@126.com 
    > Created Time: 2020年01月17日 星期五 22时06分17秒
 ************************************************************************/

#include<stdio.h>
int main()
{
	int n,s1,s2,s3,c=0;
	scanf("%d",&n);
	while(n--)
	{
		scanf("%d%d%d",&s1,&s2,&s3);
		int count=0;
		if(s1) count++;
		if(s2) count++;
		if(s3) count++;
		if(count>=2) c++;
	}
	printf("%d",c);
}
