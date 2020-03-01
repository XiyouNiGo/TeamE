/*************************************************************************
    > File Name: M.c
    > Author: zll
    > Mail: zhnllion@126.com 
    > Created Time: 2020年01月19日 星期日 21时56分19秒
 ************************************************************************/

#include<stdio.h>
int main()
{
	long long  n;
	scanf("%lld",&n);
	if(n==0) printf("0\n");
	else if((n+1)%2==0) printf("%lld\n",(n+1)/2);
	else printf("%lld\n",n+1);
}
