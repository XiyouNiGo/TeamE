/*************************************************************************
    > File Name: 4.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月20日 星期四 11时24分12秒
 ************************************************************************/

#include<stdio.h>
double get_num(double num1, double num2);

int main()
{
	double num1, num2;

	puts("Enter the first number");
	scanf("%lf", &num1);
	puts("Enter the second number");
	scanf("%lf", &num2);
	printf("The value is %lf\n", get_num(num1, num2));

	return 0;
}

double get_num(double num1, double num2)
{
	return (num1+num2)/(2*num1*num2);
}
