/*************************************************************************
	> File Name: Bubble_Sort.c
	> Author: 
	> Mail: 
	> Created Time: 2020年06月18日 星期四 21时46分24秒
 ************************************************************************/

#include<stdio.h>
void InsertionSort(int * k,int n)
{
    for(int i = 1;i < n;i++)
    {
        int insertValue = k[i];
        int insertIndex = i - 1;
        while(insertIndex >= 0 && insertValue <k[insertIndex])
        {
            k[insertIndex + 1] = k[insertIndex];
            insertIndex --;
        }
        k[insertIndex + 1] = insertValue;
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
    InsertionSort(a,n);
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
