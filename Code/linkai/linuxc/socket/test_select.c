#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>
#include <sys/select.h>

void my_err(const char *str)
{
     perror(str);
     exit(1);
}

void display_time(const char *str)
{
    int seconds;
    seconds = time( (time_t*)NULL );
    printf("%s, %d\n", str, seconds);
}

int main(int argc, char *argv[])
{
    fd_set readfds;
    struct timeval timeout;
    int ret;

    //监视stdin是否有数据输入
    FD_ZERO(&readfds);
    FD_SET(0, &readfds);
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    while (1)
    {
        //事实上timeval初始化应放在循环内部
        //因为Linux中对select的实现会修改timeval为剩余时间
        //10ｓ后timeval相当于0(非阻塞)
        //timeout.tv_sec = 10;
        //timeout.tv_usec = 0;

        display_time("Before select");
        ret = select(1, &readfds,NULL, NULL, &timeout);
        display_time("After select");

        switch (ret)
        {
            case 0:
                printf("No data in 10 seconds.\n");
                exit(0);
                break;
            case -1:
                my_err("select error");
                break;
            default:
                getchar();
                printf("Data is avaliable now.\n");
        }
    }

    return 0;
}
