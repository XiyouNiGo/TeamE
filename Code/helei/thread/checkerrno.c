/*************************************************************************
	> File Name: checkerrno.c
	> Author: 
	> Mail: 
	> Created Time: 2020年05月19日 星期二 09时47分11秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>


int main(void)
{
    FILE *stream;
    char * filename = "test";
    errno = 0;
    stream = fopen(filename,"r");
    if(stream == NULL)
    {
        printf("open file %s failed,errno is %d\n",filename,errno);

    }
    else
        printf("open file %s successfully\n",filename);
}
