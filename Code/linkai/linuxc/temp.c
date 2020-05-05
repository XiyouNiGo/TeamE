#include <stdio.h>
#include <stdlib.h>
int main ()
{
   FILE *fp;
   char *username = getenv("USER");
   char history_path[256];

   sprintf(history_path, "%s%s%s", "/home/", username, "/.history");

    //fopen里面的路径我用~/.history不行????
    //用/home/xxx/.history就可以
   if ((fp = fopen(history_path, "r")) == NULL)
   {
        my_err("fopen error");
   }
    int i = 1;
    char temp[256];

   while (fscanf(fp, "%s", temp) != EOF)
   {
       printf("%d %s\n", i++, temp);
   }
}
