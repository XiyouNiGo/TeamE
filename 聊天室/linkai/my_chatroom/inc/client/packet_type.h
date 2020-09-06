#ifndef PACKET_TYPE_H_
#define PACKET_TYPE_H_

struct packet
{
    int len;
    int flag;
    char buf[1024];
}packet;

#endif
