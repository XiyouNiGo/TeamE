/*************************************************************************
	> File Name: Bubble_Sort.c
	> Author: 
	> Mail: 
	> Created Time: 2020年06月18日 星期四 21时46分24秒
 ************************************************************************/

#include<stdio.h>
void Bubble_Sort ( int * ,int );
void Bubble_Sort_Good(int *k,int n);
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
    Bubble_Sort (a,n);
    for(int i = 0;i< n;i++)
    {
        printf("%d\t",a[i]);
    }
    printf("\n");
    Bubble_Sort_Good(a,n);
    for(int i = 0;i< n;i++)
    {
        printf("%d\t",a[i]);
    }
    printf("\n");
    
}
void Bubble_Sort(int *k,int n)
{
    int temp = 0;
    int count1 = 0 ,count2 = 0;
    for(int i = 0; i < n-1;i++ )
    {
        for(int j = 0; j < n - i - 1;j++)
        {
            count1++;
            if( k[j] > k[j+1] )
            {
                count2++;
                temp = k[j];
                k[j] = k[j+1];
                k[j+1] = temp;
            }
        }
    }
    printf("共循环了%d次\t共交换了%d次\n",count1,count2);
}
void Bubble_Sort_Good(int *k,int n)
{
    int temp = 0;
    int count1 = 0 ,count2 = 0;
    for(int i = 0;i < n-1;i++)
    {
        int flag = 1;
        for(int j = 0 ; j < n-1-i;j++)
        {
            count1 ++;
            if( k[j] > k[j+1] )
            {
//                count2 = count2 + 1;
                flag = 0;
                temp = k[j];
                k[j] = k[j+1];
                k[j+1] = temp;
                count2 ++;
                
            }
            if(flag == 1 )
            {
                break;
            }
        }
    }
    printf("共循环了%d次\t共交换了%d次\n",count1,count2);
}
