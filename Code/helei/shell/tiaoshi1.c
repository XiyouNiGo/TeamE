/*************************************************************************
	> File Name: tiaoshi1.c
	> Author: 
	> Mail: 
	> Created Time: 2020年04月30日 星期四 11时14分32秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
int main(void)
{
    
    char *arg[]={"ls","-la","|","grep","history","|","wc","-l",NULL};
    int i=0;
    char *argnext[5];
    char *argnext2[5];
    for(i = 0;arg[i] != NULL;i++)
    {
        if(strcmp(arg[i],"|") == 0)
        {
            arg[i] = NULL;
            int j;
            for(j = i+1;arg[j] != NULL;j++)
            {
                if(strcmp(arg[j],"|") == 0)
                {
                    arg[j] = NULL;
                    int k;
                    for(k = j+1;arg[k] != NULL;k++)
                    {
                        argnext2[k-j-1] = arg[k];
                    }
                }
                else
                    argnext[j-i-1] = arg[j];
            }
        }
    }
    printf("arg = %s \n",*arg);
    printf("argnext = %s \n",*argnext);
    printf("argnext2 = %s \n",*argnext2);
}
