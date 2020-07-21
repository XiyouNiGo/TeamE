/*************************************************************************
	> File Name: Bubble_Sort.c
	> Author: 
	> Mail: 
	> Created Time: 2020年06月18日 星期四 21时46分24秒
 ************************************************************************/

#include<stdio.h>
//增量gap ，gap 控制这每次前进的大小，和分组的大小，经过粗略的整理之后变得有了一定的规律，最后利用冒泡排序对数组进行整合，得到最终结果
void ShellSort(int *k,int n)
{
    for(int gap = n/2;gap > 0;gap /= 2)
    {
        for(int i = gap;i < n;i++)
        {
            for(int j = i-gap;j >= 0;j-=gap)
            {
                if(k[j] > k[j + gap])
                {
                    int temp = k[j];
                    k[j] = k[j + gap];
                    k[j + gap] = temp;
                }
            }
        }
    }
}

int main(void)
{
    int a[100];
    int n ;
    printf("输入要排序的数据个数：");
    scanf("%d",&n);
    for(int i = 0; i < n;i++)
    {
        scanf("%d",&a[i]);
    }
    ShellSort(a,n);
    for(int i = 0;i< n;i++)
    {
        printf("%d\t",a[i]);
    }
    printf("\n");
/*    Bubble_Sort_Good(a,n);
    for(int i = 0;i< n;i++)
    {
        printf("%d\t",a[i]);
    }
    printf("\n");
  */  
}
