#include<stdio.h>
void num_3(double num)
{
	printf("该数的立方为%lf\n",num*num*num);
}
int main()
{
	double num;
	scanf("%lf",&num);
	num_3(num);
	return 0;
}

