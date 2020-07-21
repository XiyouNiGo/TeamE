/*************************************************************************
	> File Name: BinarySearch.c
	> Author: 
	> Mail: 
	> Created Time: 2020年06月26日 星期五 10时41分19秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
int BinarySearch (int * arr,int n,int x)
{
    int low = 0,high = n-1;
    int mid;
    while(low <= high)
    {
        mid = (low + high ) / 2;
        if( arr[mid] > x )
        {
            high = mid -1;
        }
        else if(arr[mid] < x)
        {
            low = mid + 1;
        }
        else if(arr[mid] = x)
        {
            return mid;
        }
    }
    exit(-1);
}
int recursionBinarySearch(int * arr,int value,int low,int high)
{
    if(low >= high)
        return -1;
    int mid = (low + high) / 2;
    if(arr[mid] == value)
        return mid;
    else if(arr[mid] < value)
        return recursionBinarySearch(arr,value,mid + 1,high);
    else if(arr[mid] > value)
        return recursionBinarySearch(arr,value,low,mid-1);

}
int main(void)
{
    int n;
    int arr[100]={0};
    printf("请输入数组大小：\n");
    scanf("%d",&n);
    printf("输入数据：");
    for(int i = 0; i < n ; i++)
    {
        scanf("%d",&arr[i]);
    }
    int x;
    printf("请输入要查找的元素：");
    scanf("%d",&x);
    int index = recursionBinarySearch (arr,x,0,n-1);
    printf("查找的元素下标为 : %d\n",index);
}
