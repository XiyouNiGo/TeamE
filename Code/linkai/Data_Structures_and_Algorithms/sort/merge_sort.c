
/*************************************************************************
    > File Name: merge.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年05月31日 星期日 10时37分34秒
 ************************************************************************/
//归并排序
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>

void merge(int arr[], int L, int M, int R)
{
    int LEFT_SIZE = M - L;
    int RIGHT_SIZE = R - M + 1;
    int left[LEFT_SIZE];
    int right[RIGHT_SIZE];

    //分成左右两个数组
    for (int i = L; i < M; i++)
    {
        left[i-L] = arr[i];
    }
    for (int i = M; i < R+1; i++)
    {
        right[i-M] = arr[i];
    }

    /*for (int i = 0; i < LEFT_SIZE; i++)
        printf(" %d", left[i]);
    putchar('\n');
    for (int i = 0; i < RIGHT_SIZE; i++)
        printf(" %d", right[i]);
    putchar('\n');*/

    //填入原数组
    int i = 0, j = 0, k = L;
    while (i < LEFT_SIZE && j < RIGHT_SIZE)
    {
        if (left[i] < right[j])
        {
            arr[k] = left[i];
            i++;
            k++;
        }
        else
        {
            arr[k] = right[j];
            j++;
            k++;
        }
    }
    while (i < LEFT_SIZE)
    {
        arr[k] = left[i];
        i++;
        k++;
    }
    while (j < RIGHT_SIZE)
    {
        arr[k] = right[j];
        j++;
        k++;
    }
}

void merge_sort(int arr[], int L, int R)
{
   if (L == R) 
       return;
   int M = (L + R) / 2;
   merge_sort(arr, L, M);
   merge_sort(arr, M+1, R);
   merge(arr, L, M+1, R);
}

int main(int argc, char *argv[])
{
    int arr[8];
    int L = 0;
    int R = 7;
    int M = 4;

    printf("Enter random 8 numer:\n");
    for (int i = 0; i < 8; i++)
    {
        scanf("%d", &arr[i]);
    }

    printf("Before:");
    for (int i = 0; i < 8; i++)
        printf(" %d", arr[i]);
    putchar('\n');

    merge_sort(arr, L, R);

    printf("After:");
    for (int i = 0; i < 8; i++)
        printf(" %d", arr[i]);
    putchar('\n');

    return 0;
}
