
/*************************************************************************
  > File Name: try.c
  > Author: NiGo
  > Mail: nigo@xiyoulinux.org
  > Created Time: 2020年04月22日 星期三 11时35分33秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <pwd.h>

#define CLOSE "\001\033[0m\002"                 // 关闭所有属性
#define BLOD  "\001\033[1m\002"                 // 强调、加粗、高亮
#define BEGIN(x,y) "\001\033["#x";"#y"m\002"    // x: 背景，y: 前景

int main()
{
    //srand( (unsigned int)time(NULL) );
    while (1)
    {
        printf("%d\n", rand()%1000+1);

        sleep(1);
    }
}
