/*************************************************************************
	> File Name: Bubble_Sort.c
	> Author: 
	> Mail: 
	> Created Time: 2020年06月18日 星期四 21时46分24秒
 ************************************************************************/

#include<stdio.h>
void merge(int *arr,int L,int M,int R)
{
    int LEFT_SIZE = M - L;
    int RIGHT_SIZE = R - M + 1;
    int left[LEFT_SIZE];
    int right[RIGHT_SIZE];
    
    //分开
    for(int i = 0;i < M;i++)
    {
        left[i-L] = arr[i];

    }
    for(int i = M;i <= R;i++)
    {
        right[i-M] = arr[i];
    }
    //合并
    int i = 0,j = 0,k = L;
    while( i < LEFT_SIZE && j < RIGHT_SIZE  )
    {
        if(left[i] < right[j])
        {
            arr[k] = left[i];
            k++;
            i++;
        }
        else 
        {
            arr[k] = right[j];
            j++;
            k++;
        }
    }
    while(i < LEFT_SIZE)
    {
        arr[k] = left[i];
        i++;
        k++;
    }
    while(j < RIGHT_SIZE)
    {
        arr[k] = right[j];
        j++;
        k++;

    }
}
void mergeSort(int * arr,int L,int R)
{
    if(L == R)
        return ;
    else
    {
        int M = (L+R) / 2;
        mergeSort(arr,L,M);
        mergeSort(arr,M+1,R);
        merge(arr,L,M+1,R);
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
    mergeSort(a,0,n-1);
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
