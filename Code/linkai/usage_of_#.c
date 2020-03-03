/*************************************************************************
#:自动把参数转换为字符串 
##:链接
 ************************************************************************/

#include<stdio.h>
#define printNum(x) printf("%s is %d\n", #x, x)
#define I(x) I##x
int main()
{
	int num1 = 10;
	printNum(num1);
	int I(1) = 1;
	printNum(I1);
	return 0;
}
