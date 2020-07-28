
/*************************************************************************
    > File Name: cJSON.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年07月26日 星期日 21时35分40秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <cJSON/cJSON.c>

void my_err(const char *str)
{
     perror(str);
     exit(1);
}

int main(int argc, char *argv[])
{
    cJson *json = cJSON_CreateObject();

}
