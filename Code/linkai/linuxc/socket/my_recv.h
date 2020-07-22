#ifndef _MY_RECV_H_
#define _MY_RECV_H_

#define BUFSIZE 1024

void my_err(const char *str, int line);
void my_err_exit(const char *str, int line);
int my_recv(int connect_fd, char *data_buf, int len);

#endif
