
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

}
int main(int argc, char *argv[])

