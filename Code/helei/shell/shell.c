/*************************************************************************
	> File Name: myshell.c
	> Author: 
	> Mail: 
	> Created Time: 2020年05月01日 星期五 22时19分25秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<signal.h>
//屏蔽ctrl + c 终止进程
#include<dirent.h>
#include<pwd.h>
#include<readline/history.h>



#define normal         0
#define out_redirect   1
#define in_redirect    2
#define have_pipe      3
#define double_have_pipe 4

void print_prompt();
void get_input(char *buf);
void explain_input(char *buf,int *argcount,char arglist[100][256]);
void do_cmd(int argcount,char arglist[100][256]);
int find_command(char *command);
void history();

int main(int argc, char *argv[])
{
    signal(SIGINT,SIG_IGN);
    //ctrl + c 
    int i;
    int argcount = 0;
    char arglist[100][256];
    char **arg = NULL;
    char *buf = NULL;
    buf = (char *)malloc(sizeof(char)*256);
    if(buf == NULL)
    {
        perror("malloc failed:");
        exit(-1);
    }
    while(1)
    {
        memset(buf,0,256);
        print_prompt();
        get_input(buf);
        //write_history("history_command");
        if(strcmp(buf,"exit\n") == 0 || strcmp(buf,"logout\n") == 0)
            break;
        for(i=0;i<100;i++)
        {
            arglist[i][0] = '\0';
        }
        argcount = 0;
        explain_input(buf,&argcount,arglist);
        do_cmd(argcount,arglist);
    }

    if(buf != NULL)
    {
        free(buf);
        buf = NULL;
    }
    exit(0);
}
void print_prompt()
{
    char *buf = (char *)malloc(sizeof(char )*PATH_MAX);
    getcwd(buf,PATH_MAX);
    printf("\033[;34m %s\033[0m $ ",buf);

}
void get_input(char *buf)
{
    int len = 0;
    int ch;
    ch = getchar ();
    while(len < 256 && ch != '\n')
    {
        buf[len++] = ch;
        ch = getchar ();
    }
    if(len == 256)
    {
        printf("command is too long\n");
        exit(-1);
    }
    buf[len] = '\n';
    len ++;
    buf[len] = '\0';
    add_history(buf);
}

void explain_input(char *buf,int *argcount, char arglist[100][256])
{
    char *p = buf;
    char *q = buf;
    int number = 0;
    while(1)
    {
        if(p[0] == '\n')
            break;
        if(p[0] == ' ')
            p++;
        else
        {
            q = p;
            number = 0;
            while((q[0]!=' ') && (q[0]!='\n'))
            {
                number++;
                q++;
            }
            strncpy(arglist[*argcount],p,number+1);
            arglist[*argcount][number] = '\0';
            *argcount = *argcount + 1;
            p = q;
        }
    }
}

void do_cmd(int argcount,char arglist[100][256])
{
    int flag = 0;
    int how = 0;
    int background = 0;
    int status;
    int i;

    int fd;
    
    char *arg[argcount+1];
    char * argnext[argcount + 1];
    char * argnext2[argcount + 1];
    char *file;
    pid_t pid;
    for(i=0;i<argcount;i++)
    {
        arg[i] = (char *)arglist[i];
    }
    arg[argcount] = NULL;

    for(i=0;i<argcount;i++)
    {
        if(strncmp(arg[i],"&",1) == 0)
        {
            if(i == argcount -1 )
            {
                background = 1;
                arg[argcount-1] = NULL;
                break;
            }
            else{
                printf("Wrong command\n");
                return ;
            }
        }
    }
    if(strncmp(arg[0],"cd",2) == 0)
    {
        if(arg[1] == NULL)
        {
            printf("Usage : cd <target path>");
        /*    char *home_path = getenv("HOME");
            if((chdir(home_path)<0))
            {
                perror("chdir failed");
            }
            return ;*/
        }
        else
        {
            if(chdir(arg[1])<0)
            {
                perror("chdir failed");
                return ;
            }
            else
            {
                /*char *path_name = getenv("HOME");
                printf("%s",path_name);*/
                char *path_name = getcwd(NULL,0);
                printf("%s\n",path_name);
            }
        }
        /*else if(arg[1][0] == '~')
        {
            char *home_path = getenv("HOME");
            char *path = (char *)malloc(sizeof(char )*PATH_MAX);
            strcpy(path,home_path);
            strcat(path,arg[1]+1);
            if(chdir(path)<0)
            {
                perror("chdir");
            }
            return ;
            else
            {
                if(chdir(arg[1])<0)
                {
                    perror("chdir failed");
                }
                return ;
            }
    }*/
    }
    if(strcmp(arg[0],"history") == 0)
    {
        history();
    }
    for(int i = 0 ; arg[i] != NULL ; i++)
    {
        if(strcmp(arg[i],">") == 0)
        {
            flag++;
            how = out_redirect;
            if(arg[i+1] == NULL)
                flag++;
        }
        if(strcmp(arg[i],"<") == 0)
        {
            flag++;
            how = in_redirect;
            if(i == 0)
                flag++;
        }
        if(strcmp(arg[i],"|") == 0)
        {
            flag++;
            how = have_pipe;
            if(arg[i+1] == NULL)
                flag++;
            if(i == 0)
                flag++;
        }
    }

    /*if(flag>1)
    {
        printf("Wrong command\n");
        return ;
    }*/
    if(how == out_redirect)
    {
        for(i = 0;arg[i]!=NULL;i++)
        {
            if(strcmp(arg[i],">") == 0)
            {
                file = arg[i+1];
                arg[i] = NULL;
            }
        }
    }

    if(how == in_redirect)
    {
        for( i = 0;arg[i]!=NULL;i++ )
        {
            if(strcmp(arg[i],"<") == 0)
            {
                file = arg[i+1];
                arg[i] = NULL;

            }
        }
    }
    // 多重管道 
    if(how == have_pipe)
    {
        /*for(i = 0;arg[i]!=NULL;i++ )
        {
            if(strcmp(arg[i],"|") == 0)
            {
                arg[i] = NULL;
                int j;
                for(j = i+1;arg[j]!=NULL;j++)
                {
                    argnext[j-i-1] = arg[j];
                }
                argnext[j-i-1] = arg[j];
                break;
            }
        }*/
        int i= 0;
        
        for(i = 0;arg[i] != NULL;i++)
        {
            if(strcmp(arg[i],"|") == 0)
            {
                arg[i] = NULL;
                int j;
                for(j = i+1;arg[j] != NULL; j++)
                {
                    if(strcmp(arg[j],"|" ) == 0)
                    {
                        arg[j] = NULL;
                        int k;
                        for(k = j+1;arg[k] != NULL; k++)
                        {
                            argnext2[k-j-1] = arg[k];
                            how = double_have_pipe;
                        }

                    }
                    else
                        argnext[j-i-1] =arg[j];
                    
                }
            }
        }
    
    }

    if((pid = fork())<0)
    {
        printf("fork error\n");
        return ;
    }
    switch(how)
    {
        case 0:
            if(pid == 0)
        {
            if((!(find_command(arg[0]))) || (strcmp(arg[0],"cd") == 0))
            {
                //printf("%s : command1 not found\n",arg[0]);
                exit(0);
            }
            else
            {
            execvp(arg[0],arg);
            exit(0);
             }
        }
        break;
        case 1:
            if(pid == 0)
        {
            if(!(find_command(arg[0])))
            {
                printf("%s : command not found\n",arg[0]);
                exit(0);
            }
            fd = open(file,O_RDWR | O_CREAT | O_TRUNC, 0644);
            dup2(fd,1);
            execvp(arg[0],arg);
            exit(0);
        }
        break;
        
        case 2:
            if(pid == 0)
        {
            if(!(find_command(arg[0])))
            {
                printf("%s : command not found\n",arg[0]);
                exit(0);
            }
            fd = open(file,O_RDONLY);
            dup2(fd,0);
            execvp(arg[0],arg);
            exit(0);
        }
        break;
        
        case 3:
            if(pid == 0)
        {
            int pid2;
            int status2;
            int fd2;
            if((pid2 = fork())<0)
            {
                printf("fork2 error\n");
                return ;
            }
            else if(pid2 == 0)
            {
                if(!(find_command(arg[0])))
                {
                    printf("%s : command not found\n",arg[0]);
                    exit(0);
                }
                fd2 = open("/tmp/youdonotknowfile",O_WRONLY | O_CREAT |O_TRUNC,0644);
                dup2(fd2,1);
                execvp(arg[0],arg);
                exit(0);
            }
            if(waitpid(pid2,&status2,0) == -1)
            {
                printf("wait for child process error\n");
            }
            if(!(find_command(argnext[0])))
            {
                printf("%s : command not found \n",argnext[0]);
                exit(0);
            }
            fd2 = open("/tmp/youdonotknowfile",O_RDONLY);
            dup2(fd2,0);
            execvp(argnext[0],argnext);
            if(remove("/tmp/youdonotknowfile"))
                printf("remove error\n");
            exit(0);
        }
        break;
    case 4:
        if(pid == 0)
        {
            int pid1,pid2;
            int status1,status2;
            int fd1,fd2;
            if(pid1 = fork()<0)
            {
                printf("%d : fork failed",__LINE__);
            }
            else if(pid1 == 0)
            {
                pid2 = fork();
                if(pid2<0)
                {
                    printf("%d : fork failed",__LINE__);
                    exit(0);
                }
                else if(pid2 == 0)
                {
                    fd2 = open("/tmp/youdonotknowfile1",O_RDWR);
                    dup2(fd2,1);
                    execvp(arg[0],arg);
                    exit(0);
                }
                if(waitpid(pid2,&status2,0) == -1)
                {
                    printf("%d : wait child process failed",__LINE__);
                    exit(0);
                }
                if(!find_command(argnext[0]))
                {
                    printf("%d : find  command  failed");
                    exit(0);
                }
                fd2 = open("/tmp/youdonotknowfile1",O_RDONLY);
                fd1 = open("/tmp/youdonotknowfile2",O_RDWR);
                dup2(fd2,0);
                dup2(fd1,1);
                execvp(argnext[0],argnext);
                exit(0);
            }
            if(waitpid(pid2,&status1,0) == -1)
            {
                printf("%d : wait child process failed");
                exit(0);
            }
            if(!(find_command(argnext2[0])))
            {
                printf("%d :  command not found");
                exit(0);
            }
            fd1 = open("/tmp/youdonotknowfile2",O_RDONLY);
            dup2(fd1,0);
            execvp(argnext2[0],argnext2);
            exit(0);
        }
        break;
    default:
            break;
    }

    if(background == 1)
    {
        printf("process id %d\n",pid);
        return;
    }
    if(waitpid(pid,&status,0) == -1)
        printf("wait for child process error\n");
}

int find_command(char *command)
{
    DIR * dp;
    struct dirent * dirp;
    char *path[] = {"./","/bin","/usr/bin",NULL};
    if(strncmp(command,"./",2) == 0)
        command = command +2;
    int i  = 0;
    while(path[i]!=NULL)
    {
        if((dp = opendir(path[i]))==NULL)
            printf("Can not open /bin \n");
        while((dirp = readdir(dp))!=NULL)
        {
            if(strcmp(dirp->d_name,command) == 0)
            {
                closedir(dp);
                return 1;
            }
        }
        closedir(dp);
        i++;
    }
    return 0;
}
void history()
{
    int fd = 0;
    char *buf = (char *)malloc(sizeof(char )*1000);
    if(fd = open("/tmp/history",O_RDWR)<0)
    {
        perror("find history failed");
        exit(0);
    }
    read(fd,buf,sizeof(buf));
    write(1,buf,strlen(buf));
    free(buf);
}
