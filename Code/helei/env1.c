/*************************************************************************
	> File Name: env.c
	> Author: 
	> Mail: 
	> Created Time: 2020年04月26日 星期日 09时25分26秒
 ************************************************************************/

#include<stdio.h>
#include<malloc.h>
int main(int argc,char * argv[],char * envp[])
{
    int i;
    printf("Argument:\n");
    for(i = 0;i<argc;i++ )
    {
        printf("argv[%d] is %s \n",i,argv[i]);
    }
    printf("Environment:\n");
    for(i = 0;envp[i]!=NULL;i++)
    {
        printf("%s\n",envp[i]);
    }
    return 0;

}
