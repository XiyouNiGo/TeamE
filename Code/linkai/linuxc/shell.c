
/*************************************************************************
  > File Name: my_shell.c
  > Author: NiGo
  > Mail: nigo@xiyoulinux.org
  > Created Time: 2020年04月25日 星期六 10时10分36秒
 ************************************************************************/
//这是一个失败品

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <pwd.h>

#define normal               0//一般的命令
#define out_redirect         1//输出重定向
#define in_redirect          2//输入重定向
#define have_pipe            3//命令中有管道
#define CLOSE "\001\033[0m\002"                 // 关闭所有属性
#define BLOD  "\001\033[1m\002"                 // 强调、加粗、高亮
#define BEGIN(x,y) "\001\033["#x";"#y"m\002"    // x: 背景，y: 前景

void get_input(char *);
void explain_input(char *, int *, char [][256]);
void do_ex_cmd(int, char [][256]);
int do_in_cmd(int, char [][256]);
int find_command(char *);
void init_shell();
void set_prompt(char*);
void history();
int init_daemon();
void do_cmd_mult_pipe(int, int, char [][256]);

char *line_read = (char *)NULL;
char prompt[256];
char history_path[256];

void my_err(char *str)
{
    perror(str);
    //防止记录丢失
    write_history(NULL);
    exit(1);
}

int main(int argc, char *argv[])
{
    init_shell();
    int i;
    int argcount = 0;
    char arglist[100][256];
    char **arg = NULL;
    char *buf = NULL;

    if ((buf = (char*)malloc(256)) == NULL)
    {
        my_err("malloc error");
    }

    while (1)
    {
        memset(buf, 0, 256);
        get_input(buf);
        //printf("**%s**", buf);

        for (i = 0; i < 100; i++)
        {
            arglist[i][0] = '\0';
        }
        argcount = 0;
        explain_input(buf, &argcount, arglist);
        //printf("argcount is %d\n", argcount);
        if (do_in_cmd(argcount, arglist) == 0)
            do_ex_cmd(argcount, arglist);
    }
    if (buf)
    {
        free(buf);
        buf = NULL;
    }

    write_history(NULL);

    return 0;
}

/*void get_input(char *buf)
  {
  int len = 0;
  int ch;

  ch = getchar();
  while (len < 256 && ch != '\n')
  {
  buf[len++] = ch;
  ch = getchar();
  }

  if (len == 256)
  {
//输入命令过长则退出程序
printf("command is too long\n");
exit(-1);
}

buf[len] = '\n';
len++;
buf[len] = '\0';
}*/
//解析buf中命令，将结果存入arglist,命令以\n结束
void explain_input(char *buf, int *argcount, char arglist[][256])
{
    char *p = buf;
    char *q = buf;
    int number = 0;

    while (1)
    {
        if (p[0] == '\n')
        {
            break;
        }
        if(p[0] == ' ')
        {
            p++;
        }
        else
        {
            q = p;
            number = 0;
            while ((q[0] != ' ') && (q[0] != '\n'))
            {
                number++;
                q++;
            }
            strncpy(arglist[*argcount], p, number+1);
            arglist[*argcount][number] = '\0';
            *argcount = *argcount + 1;
            p = q;
        }
    }
}
//执行內建命令,返回1表示成功或有这个命令
int do_in_cmd(int argcount, char arglist[][256])
{
    //如果输入exit或logout就退出程序
    if ((strcmp(arglist[0], "exit") == 0 || strcmp(arglist[0], "logout") == 0) && argcount == 1)
    {
        write_history(NULL);
        exit(0);
    }
    //history，只支持c参数
    if (strcmp(arglist[0], "history") == 0)
    {
        if (argcount == 1)
        {
            history();
        }
        else if (argcount == 2 && strcmp(arglist[1], "-c") == 0)
        {
            if (open(history_path, O_RDWR|O_CREAT|O_TRUNC, 0644) == -1)
                my_err("open erorr");
        }
        else
        {
            printf("Usage: history [-c]\n");
        }
        return 1;
    }
    //cd命令
    if (strcmp(arglist[0], "cd") == 0)
    {
        if (argcount != 2)
        {
            puts("Usage: ./my_cd <target path>");
            return 1;
        }
        if (chdir(arglist[1]) < 0)
        {
            my_err("chdir error");
        }
        char *new_path;
        if ((new_path = getcwd(NULL, 0)) == NULL)
        {
            my_err("getcwd");
        }
        printf("%s\n", new_path);
        return 1;
    }
    //printf("here\n");
    return 0;
}
//执行外部命令
void do_ex_cmd(int argcount, char arglist[][256])
{
    int flag = 0;
    int how = 0;        //指示命令中是否有< > |
    int pipe_in = 0;       //判断是否有管道
    int background = 0; //标识命令中是否有后台运行标识符&
    int status;
    int i;
    int fd;
    char *arg[argcount+1];
    char *argnext[argcount+1];
    char *file;
    pid_t pid;

    //将命令取出
    for (i = 0; i < argcount; i++)
    {
        arg[i] = (char*)arglist[i];
    }
    arg[argcount] = NULL;

    //查看命令是否有后台运行符
    for (i = 0; i < argcount; i++)
    {
        if (strncmp(arg[i], "&", 1) == 0)
        {
            if (i == argcount-1)
            {
                background = 1;
                arg[argcount-1] = NULL;
                break;
            }
            else
            {
                printf("Wrong command\n");
                return;
            }
        }
    }
    //查看命令是否有< > |
    for (i = 0; arg[i] != NULL; i++)
    {
        if (strcmp(arg[i], ">") == 0)
        {
            flag++;
            how = out_redirect;
            if (arg[i+1] == NULL)
                flag++;
        }
        if (strcmp(arg[i], "<") == 0)
        {
            flag++;
            how = in_redirect;
            if (i == 0)
                flag++;
        }
        if (strcmp(arg[i], "|") == 0)
        {
            //第一次出现让flag++
            if (pipe_in == 0)
            { 
                flag++;
                pipe_in = 1;
            }
            if (strcmp(arg[i+1], "|") == 0)
            {
                //紧挨着也不行
                flag++;
            }
            how = have_pipe;
            if (arg[i+1] == NULL)
            {
                flag++;
            }
            if (i == 0)
            {
                flag++;
            }
        }
    }
    //flag大于1说明有多个> < |符号或者命令格式不对
    if (flag > 1)
    {
        printf("Wrong command\n");
        return;
    }
    //后台运行
    if (background == 1)
    {
        int pid3;

        if ((pid3 = fork()) < 0)
        {
            my_err("fork error");
        }
        else if (pid3 == 0)
        {
            init_daemon();
        }
        else
            return;
    }
    //孙进程打印这句话
    if (background == 1)
    {
        printf("[process id %d]\n", getpid());
    }
    if (how == out_redirect)
    {
        for (i = 0; arg[i] != NULL; i++)
        {
            if (strcmp(arg[i], ">") == 0)
            {
                file = arg[i+1];
                arg[i] = NULL;
            }
        }
    }
    if (how == in_redirect)
    {
        for (i = 0; arg[i] != NULL; i++)
        {
            if (strcmp(arg[i], "<") == 0)
            {
                file = arg[i+1];
                arg[i] = NULL;
            }
        }
    }

    if (how == have_pipe)
    {
        do_cmd_mult_pipe(0, argcount, arglist);
        return;
    }

    if ((pid = fork()) < 0)
    {
        my_err("fork error");
    }

    switch (how)
    {
    case normal:
        {
            if (pid == 0)
            {
                if (!(find_command(arg[0])))
                {
                    printf("%s: command not found\n", arg[0]);
                    exit(0);
                }
                execvp(arg[0], arg);
                exit(0);
            }
        }
        break;
    case out_redirect:
        {
            if (pid == 0)
            {
                if (!(find_command(arg[0])))
                {
                    printf("%s: command not found\n", arg[0]);
                    exit(0);
                }
                if ((fd = open(file, O_RDWR|O_CREAT|O_TRUNC, 0644)) == -1)
                {
                    my_err("open error");
                }
                dup2(fd, STDOUT_FILENO);
                execvp(arg[0], arg);
                exit(0);
            }
        }
        break;
    case in_redirect:
        {
            if (pid == 0)
            {
                if (!(find_command(arg[0])))
                {
                    printf("%s: command not found\n", arg[0]);
                    exit(0);
                }
                if ((fd = open(file, O_RDONLY)) == -1)
                {
                    my_err("open error");
                }
                dup2(fd, STDIN_FILENO);
                execvp(arg[0], arg);
                exit(0);
            }
        }
        break;
    default:
        break;
    }

    ///父进程等待子进程结束
    if (waitpid(pid, &status, 0) == -1)
    {
        my_err("waitpid error");
    }
}
//初始化shell
void init_shell()
{
    sigset_t intmask;
    sigemptyset(&intmask);/* 将信号集合设置为空 */
    sigaddset(&intmask,SIGINT);/* 加入中断 Ctrl+C 信号*/
    sigaddset(&intmask,SIGSEGV);/* 加入中断 Ctrl+\ 信号*/
    /*阻塞信号*/
    sigprocmask(SIG_BLOCK,&intmask,NULL);
    //写入history
    read_history(NULL);
    //初始化路径名
    char *username = getenv("USER");
    sprintf(history_path, "%s%s%s", "/home/", username, "/.history");
    //设置环境变量
    //在Linux/Unix系统中，system函数会调用fork函数产生子进程，由子进程来执行command命令，命令执行完后随即返回原调用的进程。
    //所以说没用.....
    char command[256] = "export PATH=\"$PATH:";
    char cwd[256];
    getcwd(cwd, sizeof(cwd));
    strcat(command, cwd);
    strcat(command, "\"");
    //printf("%s\n", command);
    system(command);
}
//查找命令中的可执行程序
int find_command(char *command)
{
    DIR *dp;
    struct dirent *dirp;
    char *path[] = {"./", "/bin", "/usr/bin", NULL};

    //使当前目录下程序可以运行
    if (strncmp(command, "./", 2) == 0)
    {
        command+=2;
    }
    //分别在当前目录、/bin和/usr/bin目录查找要执行的程序
    int i = 0;
    while (path[i] != NULL)
    {
        if ((dp = opendir(path[i])) == NULL)
        {
            my_err("opendir error");
        }
        while ((dirp = readdir(dp)) != NULL)
        {
            if (strcmp(dirp->d_name, command) == 0)
            {
                closedir(dp);
                return 1;
            }
        };
        closedir(dp);
        i++;
    }
    return 0;
}
//设置提示符
void set_prompt(char *prompt)
{
    char hostname[100];
    char cwd[100];
    char super = '#';
    char delims[] = "/";	
    struct passwd* pwd;
    //可以考虑用getenv函数去实现
    if(gethostname(hostname,sizeof(hostname)) == -1){
        strcpy(hostname,"unknown");
    }
    pwd = getpwuid(getuid());	
    if(!(getcwd(cwd,sizeof(cwd)))){
        strcpy(cwd,"unknown");	
    }
    //使用strtok时，原字符串会被破坏，因此拷贝一份
    char cwdcopy[100];
    strcpy(cwdcopy,cwd);
    char *first = strtok(cwdcopy,delims);
    char *second = strtok(NULL,delims);
    //若在主目录下就以~/...显示
    if(!(strcmp(first,"home")) && !(strcmp(second,pwd->pw_name))){
        int offset = strlen(first) + strlen(second)+2;
        char newcwd[100];
        char *p = cwd;
        char *q = newcwd;

        p += offset;
        while(*(q++) = *(p++));
        char tmp[100];
        strcpy(tmp,"~");
        strcat(tmp,newcwd);
        strcpy(cwd,tmp);			
    }	

    if(getuid() == 0)
        super = '#';
    else
        super = '$';
    sprintf(prompt, "%s%s%s@%s%s:%s%s%s%s%c ", BLOD, BEGIN(49, 32), pwd->pw_name, hostname, CLOSE, BEGIN(49, 34), BLOD, cwd, CLOSE, super);	
}
//获取输入
void get_input(char *buf)
{
    if (line_read)
    {
        free (line_read);
        line_read = (char *)NULL;
    }
    set_prompt(prompt);
    line_read = readline (prompt);

    if (line_read && *line_read)
        add_history (line_read);
    strcpy(buf, line_read);
    //readline返回的字符串没有换行符，必须加上
    strcat(buf, "\n");
}
//打印历史记录
void history()
{
    FILE *fp;

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
        printf("%5d  %s\n", i++, temp);
    }

    fclose(fp);
}
//成为后台进程
int init_daemon(void)
{
    pid_t pid;
    int i;

    //*忽略终端I/O信号
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);   //防止产生僵尸进程

    if ((pid = fork()) == -1)
    {
        my_err("fork error");
    }
    else if (pid > 0)
    {
        //结束父进程，使子程序成为后台程序
        exit(-1);
    }
    else
    {
        //建立一个新的进程组，在新的进程组，子进程成为首进程，使其脱离终端
        setsid();
        //退出该进程，fork出非会话组长的子进程，让该进程无法打开新的终端
        if ((pid = fork()) == -1)
        {
            my_err("fork error");
        }
        else if (pid > 0)
        {
            exit(-1);
        }
        else
        {
            //防止孙进程终结
            signal(SIGHUP, SIG_IGN);
            //活下来的只有孙进程
            return 0;
        }
    }
}
//递归实现多重管道
void do_cmd_mult_pipe(int left, int right, char arglist[][256])
{
    if (left > right)
    {
        return;
    }
    int index_pipe = -1;
    for (int i = left; i < right; i++)
    {
        if (strcmp(arglist[i], "|") == 0)
        {
            index_pipe = i;
            break;
        }
    }
    //无管道符
    if (index_pipe == -1)
    {
        pid_t pid;
        //把有效参数复制到arg
        char *arg[right-left+2];
        arg[right-left+1] = NULL;
        for (int j = left; j < right; j++)
        {
            arg[j-left] = arglist[j];
        }

        if ((pid = fork()) == -1)
        {
            my_err("fork error");
        }
        else if (pid == 0)
        {
            if (!(find_command(arg[0])))
            {
                printf("%s: command not found\n", arg[0]);
                exit(0);
            }
            execvp(arg[0], arg);
            exit(0);
        }
        else 
        {
            return;
        }
    }
    //有管道符
    else
    {
        int fd[2];
        if (pipe(fd) == -1)
        {
            perror("pipe error");
            return;
        }
        pid_t pid = vfork();
        if (pid == -1)
        {
            perror("vfork error");
            return;
        }
        else if (pid == 0)
        {
            pid_t pid1;

            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);

            char *arg[index_pipe-left+2];
            arg[index_pipe-left+1] = NULL;
            for (int j = left; j < index_pipe; j++)
            {
                arg[j-left] = arglist[j];
            }

            if ((pid1 = fork()) == -1)
            {
                my_err("fork error");
            }
            else if (pid1 == 0)
            {
                if (!(find_command(arg[0])))
                {
                    printf("%s: command not found\n", arg[0]);
                    exit(0);
                }
                execvp(arg[0], arg);
                exit(0);
            }
            else 
            {
                exit(0);
            }
        }
        else
        {
            wait(NULL);
            close(fd[1]);
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            do_cmd_mult_pipe(index_pipe+1, right, arglist);
        }
    }
}
