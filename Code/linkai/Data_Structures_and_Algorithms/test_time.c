/*************************************************************************
   clock()函数：
   返回从“开启这个程序进程”到“程序中调用clock()函数”时之间的CPU时钟计时单元（clock tick）数
   其中clock_t是用来保存时间的数据类型
   让测试函数重复运行多次(如果是一次可能显示为0,clock函数捕捉不到)
CLOCK_TAI: 机器时钟每秒所走的时钟打点数
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX 1e7

int max_val(int, int);
void test_time(int(*p)(int, int));

int main()
{
	test_time(max_val);
    return 0;
}

int max_val(int a, int b)
{
	return a > b ? a : b;
}

void test_time(int(*p)(int, int))
{
	int a = 1, b = 2;
	clock_t start, stop;
	double duration;
	start = clock();
	for (long i = 0; i < MAX; i++)
		(*p)(a, b);
	stop = clock();
	duration = (double)(stop-start)/CLOCK_TAI/MAX;
	printf("该函数运行时间为%lf秒\n", duration);
}

