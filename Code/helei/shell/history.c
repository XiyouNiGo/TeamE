/*************************************************************************
	> File Name: history.c
	> Author: 
	> Mail: 
	> Created Time: 2020年05月05日 星期二 10时40分24秒
 ************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>
#include <readline/readline.h>
#include <readline/history.h>

int main()
{
    int fd = 0;
    char *buf = (char *) malloc (sizeof(char ) *1000);
    fd = open("root/bash_history",O_RDONLY);
    if(fd < 0)
    {
        perror("open failed");
        return 0;
    }
    read(fd,buf,sizeof(buf));
    write(STDOUT_FILENO,buf,strlen(buf));
    free(buf);
}
