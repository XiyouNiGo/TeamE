#include <string.h>
#include <stdio.h>

void parse_string(char *str, char **output, int *num)
{
    char *temp;
    *num = 0;
    temp = (char*)strtok(str, "\036");
    while (temp)
    {
        *output++ = temp;
        (*num)++;
        temp = (char*)strtok(NULL, "\036");
    }
}

void format_string(char *str, char *output[], int num)
{
    int i;
    for (i = 0; i < num; i++)
    {
        if (i == 0)
        {
            sprintf(str, "'%s'", output[i]);
        }
        else
        {
            if (strcmp("default", output[i]))
                sprintf(str, "%s,'%s'", str, output[i]);
            else
                sprintf(str, "%s,%s", str, output[i]);
        }
    }
}

int main () {
   char str[80] = {0};
   strcat(str, "aaa");
   strcat(str, "\036");
   strcat(str, "aaa");
   strcat(str, "\036");
   printf("%lu\n", strlen(str));
   printf("%s\n", str);
   /*char *output[5];
   int num;
   parse_string(str, output, &num);
   printf("num = %d\n", num);
   for (int i = 0; i < num; i++)
       printf("%s\n", output[i]);
   char temp[80];
   format_string(temp, output, num);*/
   return(0);
}
