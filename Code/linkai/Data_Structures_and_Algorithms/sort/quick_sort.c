
/*************************************************************************
    > File Name: quick_sort.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年05月31日 星期日 21时55分13秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>

void swap(int arr[], int low, int high)
{
    int temp = arr[low];
    arr[low] = arr[high];
    arr[high] = temp;
}

void quick_sort(int arr[], int left, int right)
{
    if (left > right)
        return;
    int i = left;
    int j = right;
    int basic = arr[left];  //基准值
    
    while (i != j)
    {
        //以升序为例，一定是从右先开始，因为如果从左开始，arr[i]可能会大于basic
        while (arr[j] >= basic && i < j)
        {
            j--;
        }
        while (arr[i] <= basic && i < j)
        {
            i++;
        }
        //交换
        swap(arr, i, j);
    }
    //此时i==j
    swap(arr, left, i);

    quick_sort(arr, left, i-1);
    quick_sort(arr, i+1, right);
}
int main(int argc, char *argv[])
{
    int arr[8];

    printf("Enter random 8 numer:\n");
    for (int i = 0; i < 8; i++)
    {
        scanf("%d", &arr[i]);
    }

    printf("Before:");
    for (int i = 0; i < 8; i++)
        printf(" %d", arr[i]);
    putchar('\n');

    quick_sort(arr, 0, 7);

    printf("After:");
    for (int i = 0; i < 8; i++)
        printf(" %d", arr[i]);
    putchar('\n');

    return 0;
}
