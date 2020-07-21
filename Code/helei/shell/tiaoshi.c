/*************************************************************************
	> File Name: tiaoshi.c
	> Author: 
	> Mail: 
	> Created Time: 2020年05月04日 星期一 11时12分36秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
int main(void)
{
    char *argnext1[100];
    char *argnext[100];
    
    char *arg[]={"ls","-l","|","grep","help","|","wc","-a",NULL};
    for(int i=0;arg[i]!=NULL;i++)
    {
        if(strcmp(arg[i],"|") == 0)
        {
            arg[i] = NULL;
            int j;
            for(j = i + 1;arg[j]!=NULL;j++)
            {
                
                if(strcmp(arg[j],"|") == 0)
                {
                    int k;
                    arg[j] = NULL;
                    for(k = j+1;arg[k] != NULL;k++)
                    {
                        argnext1[k-j-1] = arg[j];
                    }
                }
                else
                    argnext[j-i-1] = arg[i];
            }
            
        }

    }
    printf("argnext = %s\n",argnext[0]);
    printf("argnext1 = %s\n",argnext1[0]);
    
}
