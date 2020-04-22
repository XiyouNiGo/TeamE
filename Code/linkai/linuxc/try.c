
/*************************************************************************
    > File Name: try.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年04月22日 星期三 11时35分33秒
 ************************************************************************/

#include <stdio.h>
#include <signal.h>

int main()
{
    printf("%zd\n", sizeof(sigset_t)/8);
    return 0;
}
