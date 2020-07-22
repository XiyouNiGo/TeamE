#include <stdio.h>
#include <stdlib.h>

void my_err(const char *str, int line, char *file)
{
    fprintf(stderr, "Flie %s, line %d: ", line, file);
    perror(str);
}

void my_err_exit(const char *str, int line, char *file)
{
    fprintf(stderr, "Flie %s, line %d: ", line, file);
    perror(str);
    exit(1);
}
