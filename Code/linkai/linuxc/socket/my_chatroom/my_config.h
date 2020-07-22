#ifndef _MY_CONFIG_H_
#define _MY_CONFIG_H_

#define my_err(format)  fprintf(stderr, "(File:%s, Line:%d) ", __FILE__, __LINE__); \
                        perror(format);

//void my_err(const char *str, int line);

void my_err_exit(const char *str, int line);

#endif
