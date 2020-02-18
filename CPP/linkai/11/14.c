/*************************************************************************
    > File Name: 14.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月18日 星期二 20时49分46秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int main(int argc, char * argv[])
{
    double base = atof(argv[1]);	//ASCII to float
    double index = atoi(argv[2]);	//ASCII to int

    printf("The result is %lf\n", pow(base, index));
}

