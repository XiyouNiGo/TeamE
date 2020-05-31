
/*************************************************************************
    > File Name: qsort.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年05月31日 星期日 16时10分43秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>
//升序
int compare(const void *a, const void *b)
{
    int *pa = (int*)a;
    int *pb = (int*)b;
    int num1 = *pa;
    int num2 = *pb;
    return num1 - num2; //大于0时，a排后面
}

int compare2(const void *key, const void *element)
{
    int num1 = *(int*)key;
    int num2 = *(int*)element;

    return num1 - num2;
}
int main(int argc, char *argv[])
{
    int arr[8] = {8, 3, 5, 4, 1, 2, 7, 6};
    
    printf("Before:");
    for (int i = 0; i < 8; i++)
        printf(" %d", arr[i]);
    putchar('\n');

    qsort(arr, 8, sizeof(int), compare);

    printf("After:");
    for (int i = 0; i < 8; i++)
        printf(" %d", arr[i]);
    putchar('\n');

    int key = 5;
    int *add;

    if (add = (int*)bsearch(&key, arr, 8, sizeof(int), compare2))
    {
        printf("add of 8 is %p\n", add);
    }
    else
    {
        printf("no such number\n");
    }

    return 0;
}
