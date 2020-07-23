
/*************************************************************************
    > File Name: try.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年07月22日 星期三 22时14分14秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "my_config.h"
#include "thread_pool.h"

void *work(void *arg)
{
    printf("%d\n", *(int*)arg);
}
int main()
{
    thread_pool_t *pool = create_thread_pool(10, 100, 100);
    sleep(5);//模拟线程处理任务
    thread_pool_destory(pool);
    return 0;
}
