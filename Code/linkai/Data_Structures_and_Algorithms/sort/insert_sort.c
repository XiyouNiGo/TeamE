
/*************************************************************************
    > File Name: insert_sort.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年05月31日 星期日 21时39分22秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>

void insert_sort(int arr[], int len)
{
    for (int i = 1; i < len; i++)
    {
        int temp = arr[i];   //后移时防止数据丢失
        int j = i - 1;
        while (j >= 0 && arr[j] > temp)
        {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = temp;    //注意要加一
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

    insert_sort(arr, len);

    printf("After:");
    for (int i = 0; i < 8; i++)
        printf(" %d", arr[i]);
    putchar('\n');

    return 0;
}
