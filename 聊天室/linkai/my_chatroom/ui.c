
/*************************************************************************
    > File Name: ui.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年08月12日 星期三 19时39分26秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include "src/client/interface.c"
#include "src/client/md5.c"
#include "src/client/client_config.c"
 

int main(int argc,char **argv)
{
    printf("      %-20s%-20s\n", "1.  用户列表", "2.  好友列表");
    printf("      %-20s%-20s\n", "3.  添加好友", "4.  删除好友");
    printf("      %-20s%-20s\n", "5.  好友资料", "6.  发送文件");
    printf("      %-20s%-20s\n", "7.  私聊好友", "8.  私聊记录");
    printf("      %-20s%-20s\n", "9.  屏蔽好友", "10. 解除屏蔽");
    printf("      %-20s%-20s\n", "11. 私聊好友", "12. 私聊记录");
    putchar('\n');
    printf("      %-20s%-20s\n", "13. 群聊列表", "14. 我的群聊");
    printf("      %-20s%-20s\n", "15. 申请加群", "16. 退出群聊");
    printf("      %-20s%-20s\n", "17. 创建群聊", "18. 群聊资料");
    printf("      %-20s%-20s\n", "19. 发送群聊", "20. 群聊记录");
    putchar('\n');
    printf("      %-20s%-20s\n", "21. 注销登录", "22. 处理消息");
}
