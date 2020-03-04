/*************************************************************************
    > File Name: 6.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月20日 星期四 11时31分42秒
 ************************************************************************/

#include<stdio.h>
void swop(double *p1, double *p2);
void sequence(double *p1, double *p2, double *p3);

int main()
{
	double num1, num2, num3;

	puts("Enter the first number");
	scanf("%lf", &num1);
	puts("Enter the second number");
	scanf("%lf", &num2);
	puts("Enter the third number");
	scanf("%lf", &num3);

	sequence(&num1, &num2, &num3);
	printf("The number after ranked is %lf %lf %lf", num1, num2, num3);

	return 0;
}

void swop(double *p1, double *p2)
{
	double t = *p1;
	*p1 = *p2;
	*p2 = t;

	return;
}

void sequence(double *p1, double *p2, double *p3)
{
	if (*p1 > *p2)
		swop(p1, p2);
	if (*p1 > *p3)
		swop(p1, p3);
	if (*p2 > *p3)
		swop(p2, p3);
}
