
/*************************************************************************
    > File Name: hill_sort.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年05月31日 星期日 21时24分30秒
 ************************************************************************/
//希尔排序(特殊的插入排序)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>

void shell_sort(int arr[], int len)
{
    for (int gap = len / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < len; i += gap)
        {
            int temp = arr[i];
            int j = i - gap;
            while (j >= 0 && arr[j] > temp)
            {
                arr[j + gap] = arr[j];
                j -= gap;
            }
            arr[j + gap] = temp;
        }
    }
}

int main(int argc, char *argv[])
{
    int arr[8];
    int len = 8;

    printf("Enter random 8 numer:\n");
    for (int i = 0; i < 8; i++)
    {
        scanf("%d", &arr[i]);
    }

    printf("Before:");
    for (int i = 0; i < 8; i++)
        printf(" %d", arr[i]);
    putchar('\n');

    shell_sort(arr, len);

    printf("After:");
    for (int i = 0; i < 8; i++)
        printf(" %d", arr[i]);
    putchar('\n');

    return 0;
}
