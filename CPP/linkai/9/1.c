/*************************************************************************
    > File Name: 1.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月20日 星期四 11时01分40秒
 ************************************************************************/

#include<stdio.h>
double min(double x, double y);

int main()
{
	double num1, num2;

	puts("Enter the first number");
	scanf("%lf", &num1);
	puts("Enter the second number");
	scanf("%lf", &num2);
	printf("The smaller value is %lf\n", min(num1, num2));

	return 0;
}

double min(double x, double y)
{
	return (x < y)? x : y;
}
