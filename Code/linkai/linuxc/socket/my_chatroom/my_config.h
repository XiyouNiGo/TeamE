#ifndef _MY_CONFIG_H_
#define _MY_CONFIG_H_

#define TRUE 1
#define FALSE 0
#define OPEN 1
#define CLOSE 0

#define my_err(format)     { \
                                    fprintf(stderr, "(File:%s, Line:%d) ", __FILE__, __LINE__); \
                                    perror(format); \
                                    exit(1); \
                                }

#endif
