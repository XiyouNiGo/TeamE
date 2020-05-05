/*************************************************************************
	> File Name: cdvc.c
	> Author: 
	> Mail: 
	> Created Time: 2020年05月04日 星期一 08时44分46秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<linux/limits.h>
#include<stdlib.h>


void my_err(const char *err_string,int line )
{
    fprintf(stderr,"line : %d ",line);
    perror(err_string);
    exit(1);

}

int main(int argc,char ** argv)
{
    char buf[PATH_MAX + 1];
    while(1)
    {
        printf("myshell$$") ;   
    
    if(argc<2)
    {
        printf("my_cd<target path>\n");
        exit(1);
    }
    if(chdir(argv[1])<0)
    {
        my_err("chdir",__LINE__);
    }
    if(getcwd(buf,512)<0)
    {
        my_err("getcwd",__LINE__);
    }
        printf("%s\n",buf);

    }
}

