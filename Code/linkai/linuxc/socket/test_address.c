#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <netinet/in.h>

void my_err(const char *str)
{
     perror(str);
     exit(1);
}

int main(int argc, char *argv[])
{
    char buffer[32];
    int ret = 0;
    int host = 0;
    int network = 0;
    unsigned int address = 0;
    struct in_addr in;
    in.s_addr = 0;

    printf("Please input your ip address:");
    fgets(buffer, 31, stdin);
    buffer[31] = '\0';

    if ( (ret = inet_aton(buffer, &in)) == 0)
        my_err("inet_aton error");
    printf("inet_aton:\t0x%lx\n", in.s_addr);

    if ( (address = inet_network(buffer)) < 0)
        my_err("inet_network error");
    printf("inet_network:\t0x%lx\n", address);

    if (inet_ntoa(in) == NULL)
        my_err("inet_ntoa error");
    printf("inet_ntoa:\t%s\n", inet_ntoa(in));

    host = inet_lnaof(in);
    network = inet_netof(in);
    printf("inet_lnaof:\t0x%x\n", host);
    printf("inet_netof:\t0x%x\n", network);

    in = inet_makeaddr(network, host);
    printf("inet_makeaddr:\t0x%x\n", in.s_addr);

    return 0;
}
