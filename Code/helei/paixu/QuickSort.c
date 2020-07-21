/*************************************************************************
	> File Name: Bubble_Sort.c
	> Author: 
	> Mail: 
	> Created Time: 2020年06月18日 星期四 21时46分24秒
 ************************************************************************/

#include<stdio.h>
void quick_sort(int *s,int l,int r);
int AdjustArray(int *s,int l,int r);
void Quick_Sort(int *s,int l ,int r);
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
   // quick_sort(a,0,n-1);

    
    Quick_Sort(a,0,n-1);

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




/*
 *
 *
 *
 * 分开写快排
int  AdjustArray(int *s,int l,int r)
{
    int i = l,j = r;
    int x = s[l];
    while(i < j)
    {
        //从右向左找小于x的数来填s[i]
        while(i < j && s[j] >= x)
        {
            j--;
        }
        if(i < j)
        {
            s[i] = s[j]; //将s[j] 填到 s[i] 中，s[j] 就形成了一个新的坑
            i++;
        }

        //从左向右找大于或等于x的数来填s[j]
        while(i < j && s[i] < x)
        {
            i++;
        }
        if(i < j)
        {
            s[j] = s[i];  // 将s[i]填到 s[j]中，s[j]就形成了一个新的坑
            j--;
        }
    }
    //退出时，i== j，将x填到这个坑中
    s[i] = x;
    return i;
}
void quick_sort(int *s,int l,int r)
{
    if( l < r)
    {
        int i = AdjustArray(s,l,r);
        quick_sort(s,l,i-1);
        quick_sort(s,i+1,r);
    }
}*/

//整合快排

void Quick_Sort(int *s,int l,int r)
{
    if(l < r)
    {
        int i = l,j = r,x = s[l];
        while(i < j)
        {
            while(i < j && s[j] >= x)
                j--;
            if(i < j)
            {
                s[i] = s[j];
                i++;
            }
            while(i<j && s[i] < x)
                i++;
            if(i < j)
            {
                s[j] = s[i];
                j--;
            }
        }
        s[i] = x;
        Quick_Sort(s,l,i-1);
        Quick_Sort(s,i+1,r);
    }
    for(int i = l; i<=r;i++ )
    {
        printf("%d\t",s[i]);
    }
    printf("\n");
}
