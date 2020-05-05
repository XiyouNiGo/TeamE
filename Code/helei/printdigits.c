/*************************************************************************
	> File Name: printdigits.c
	> Author: 
	> Mail: 
	> Created Time: 2020年04月26日 星期日 23时41分42秒
 ************************************************************************/

#include<stdio.h>
void printdigits(int n);
int main(void)
{
    printdigits(512);
}
void printdigits(int n)
{
    int cnt = 1;
    while(n>0)
    {
        n=n-cnt*10;
        cnt++;
    }
    printf("%d",cnt);
}
