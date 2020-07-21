
/*************************************************************************
    > File Name: try.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年06月08日 星期一 08时52分18秒
 ************************************************************************/




#include<mysql/mysql.h>
#include <stdio.h>
 

int
main(int
argc,char**argv)

{

        printf("MySQL client version: %s\n",mysql_get_client_info());

        return
0;

 

}

