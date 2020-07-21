/*************************************************************************
	> File Name: Bubble_Sort.c
	> Author: 
	> Mail: 
	> Created Time: 2020年06月18日 星期四 21时46分24秒
 ************************************************************************/

#include<stdio.h>
void SelectionSort(int *k,int n)
{
    int max;
    int index;
    int count1 = 0,count2 = 0;
    for(int i = 0;i < n-1;i++)
    {
        int index = i;
        for(int j = i+1; j < n; j++)
        {
            
            count1++;
            if(k[j] > k[index])
            {
                index = j;
                count2 ++;
            }
        }
        int temp =  k[index];
        k[index] = k[i];
        k[i] = temp;
    }
    printf("共循环了%d次\t共交换了%d次\n",count1,count2);
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
    SelectionSort(a,n);
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
