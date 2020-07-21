/*************************************************************************
	> File Name: mls.c
	> Author: 
	> Mail: 
	> Created Time: 2020年04月03日 星期五 22时51分22秒
 ************************************************************************/

#include<stdio.h>
#include<stdio.h>
#include<signal.h>
//用来屏蔽Ctrl+c杀死程序
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<unistd.h>
#include<sys/types.h>
#include<dirent.h>
#include<grp.h>
#include<pwd.h>
#include<errno.h>
#include<sys/stat.h>
#include<limits.h>


//颜色参数
#define WHITE 0
#define BLUE  1
#define GREEN 2
#define RED   3
#define LBLUE 4
#define YELLOW 5

//命令行参数
#define PARAM_NONE 0
#define PARAM_A    1
#define PARAM_L    2
#define PARAM_R    4
#define PARAM_r    8
#define PARAM_F    16
#define PARAM_i    32

//设置每行最多的字符数为80
#define MAXROWLEN  80


//用于控制换行
int g_leave_len = MAXROWLEN;
int g_maxlen;


void printf_name(char *name,int color);
int get_color(struct stat buf);
void display_dir(int flag_param,char *path);
void display_dir_R(int flag,char *name);
void display_attribute(struct stat buf,  char *name);
void display_single(int flag,struct stat buf,char *name,int color);
void display(int flag,char *pathname);


int main(int argc,char  *argv[])
{
    signal(SIGINT,SIG_IGN);
    //屏蔽ctrl+c杀死程序
    int i,j,k,num;
    char path[PATH_MAX+1];
    char param[32];
    int flag_param = PARAM_NONE;
    struct stat buf;
    j=0;
    num=0;
    //找到参数存放在param数组中
    for(i=1;i<argc;i++)
    {
        if(argv[i][0] == '-')
        {
            for(k=1;k<strlen(argv[i]);k++,j++)
            {
                param[j] =argv[i][k];

            }
            num++;
        }
    }

    //让flag_param 得到总参数
    for(i=0;i<j;i++)
    {
        if(param[i] == 'a')
        {
            flag_param |=PARAM_A;
            continue;
        }
        else if(param[i] == 'l')
        {
            flag_param |=PARAM_L;
            continue;
        }
        else if(param[i] =='R')
        {
            flag_param |=PARAM_R;
        }
        else if(param[i] == 'r')
        {
            flag_param |=PARAM_r;
        }
        else if(param[i] == 'F')
        {
            flag_param |= PARAM_F;
        }
        else if(param[i] == 'i')
        {
            flag_param |= PARAM_i;
        }
        else 
        {
            printf("my_ls :invalid option -%c \n",param[i]);
            exit(1);

        }

    }
    param[j]='\0';
    //如果没有输入路径，就设置为当前路径
    if((num+1) == argc)
    {
        strcpy(path,"./");
        path[2]='\0';
        display_dir(flag_param,path);
        return 0;
    }
    i=1;
    do
    {
        //过滤掉参数  找到路径
        if(argv[i][0] == '-')
        {
            i++;
            continue;
        }
        else 
        {
            strcpy(path,argv[i]);
            if(stat (path,&buf) == -1)
                perror("stat fail:");
            //判断该路径是不是目录
            if(S_ISDIR(buf.st_mode))   
            {
                //是目录但是没带/  ，就加上/
                if(path[strlen(argv[i])-1] !='/')
                {
                    path[strlen(argv[i])]='/';
                    path[strlen(argv[i])+1] ='\0';
                }
                
                if(flag_param & PARAM_R)
                {
                    display_dir_R(flag_param,path);
                }
                else
                    display_dir(flag_param,path);
                
                i++;
                
            }
            else
            {
                display(flag_param,path);
                i++;
            }
            

        }
    }while(i<argc);
    return 0;
}
void display_attribute(struct stat buf,  char *name)
{
	char buf_time[32];
	struct passwd *psd;
    struct group *grp;
	
    //文件类型
	if(S_ISLNK(buf.st_mode))
	{
		printf("l");
	}
	else if(S_ISREG(buf.st_mode))
	{
		printf("-");
	}
	else if(S_ISDIR(buf.st_mode))
	{
		printf("d");
	}
	else if(S_ISCHR(buf.st_mode))
	{
		printf("c");
	}
	else if(S_ISBLK(buf.st_mode))
	{
		printf("b");
	}
	else if(S_ISFIFO(buf.st_mode))
	{
		printf("f");
	}
	else if(S_ISSOCK(buf.st_mode))
	{
		printf("s");
	}
	
    //user的权限
	if(buf.st_mode&S_IRUSR)
	{
		printf("r");
	}
	else
	{
		printf("-");
	}
	if(buf.st_mode&S_IWUSR)
	{
		printf("w");
	}
	else
	{
		printf("-");
	}
	if(buf.st_mode&S_IXUSR)
	{
		printf("x");
	}
	else
	{
		printf("-");
	}
	
    //group的权限
	if(buf.st_mode&S_IRGRP)
	{
		printf("r");
	}
	else
	{
		printf("-");
	}
	if(buf.st_mode&S_IWGRP)
	{
		printf("w");
	}
	else
	{
		printf("-");
	}
	if(buf.st_mode&S_IXGRP)
	{
		printf("x");
	}
	else
	{
		printf("-");
	}
	
    //others的权限
	if(buf.st_mode&S_IROTH)
	{
		printf("r");
	}
	else
	{
		printf("-");
	}
	if(buf.st_mode&S_IWOTH)
	{
		printf("w");
	}
	else
	{
		printf("-");
	}
	if(buf.st_mode&S_IXOTH)
	{
		printf("x");
	}
	else
	{
		printf("-");
	}
	printf("  ");


    psd = getpwuid(buf.st_uid);
    grp = getgrgid(buf.st_gid);
    printf("%4d",buf.st_nlink);
    printf("%-8s",psd->pw_name);
    printf("%-8s",grp->gr_name);
    printf("%6d",buf.st_size);
    strcpy(buf_time,ctime(&buf.st_mtime));
    buf_time[strlen(buf_time)-1]='\0';
    printf(" %s",buf_time);
    	
}
void display_single(int flag,struct stat buf,char *name,int color)
{
    int i,len;
    //判断是否带有i参数
    if(flag & PARAM_i)
    {
        printf("%-8d",buf.st_ino) ;
        printf_name(name,color);
        printf("\n");
    }
    else
    {

    
        if(g_leave_len<g_maxlen)
        {
            printf("\n");
            g_leave_len = MAXROWLEN;
        }
        len = g_maxlen - strlen(name);
        printf_name(name,color);
        //判断是否带有参数F
        if(flag & PARAM_F)
        {
            //链接文件在文件名后加@
        if(S_ISLNK(buf.st_mode))
            printf("@");
            //管道文件后面加|
        else if(S_ISFIFO(buf.st_mode))
            printf("|");
            //目录后面加/
        else if(S_ISDIR(buf.st_mode))
            printf("/");
            //套接字后面加=
        else if(S_ISSOCK(buf.st_mode))
            printf("=");
        else 
            printf(" ");
        }
        for(i=0;i<len;i++)
        {
            printf(" ");
        }
        printf("  ");
        g_leave_len -= (g_maxlen+3);
    }
}
void display(int flag,char *pathname)
{
    int i,j;
    struct stat buf;
    char name[NAME_MAX+1];

    //过滤掉路径，仅保留文件名到name
    for(i=0,j=0;i<strlen(pathname);i++)
    {
        if(pathname[i] == '/')
        {
            j=0;
            continue;

        }
        name[j++]=pathname[i];

    }
    name[j] = '\0';
    if(lstat(pathname,&buf) == -1)
        
    {
        //过滤掉权限不够的错误
        if(errno !=13)
            perror("opendir ");
        else 
            return ;
    }
    int color = get_color (buf);
    switch(flag)
    {
        case PARAM_NONE:
            if(name[0]!='.')
                display_single(flag,buf,name,color);
            break;
        case PARAM_A:
            display_single(flag,buf,name,color);
            break;
        case PARAM_L:
            if(name[0]!='.')
            {
            display_attribute(buf,name);
            printf("%-s\n",name);
            }
            break; 
        case PARAM_F:
            if(name[0]!='.')
        {
            display_single(flag,buf,name,color);
            break;
        }
        case PARAM_i:
            if(name[0]!='.')
            {
                display_single(flag,buf,name,color);
            }
        break;
        case PARAM_r:
            if(name[0]!='.')
        {
            display_single(flag,buf,name,color);
            break;
        }
        case PARAM_R:
            if(name[0]!='.')
            {
                display_single(flag,buf,name,color);
            }
            break;
        case PARAM_R+PARAM_A:
            display_single(flag,buf,name,color);
            break;
        case PARAM_L+PARAM_R:
            if(name[0]!='.')
            {
                display_attribute(buf,name);
            }
            break;
        case PARAM_L+PARAM_A:
            display_attribute(buf,name);
            break;
        case PARAM_L+PARAM_A+PARAM_R:
            display_attribute(buf,name);
            break;
        default:
            break;
    }
}
void display_dir(int flag_param,char *path)
{
    
    DIR *dir;
    g_leave_len=80;
    struct dirent *ptr;
    int count = 0;
    char filenames[256][PATH_MAX+1],temp[PATH_MAX+1];

    dir = opendir(path);
    if(dir == NULL)
    {
        //因为有多层函数调用，所以每一次都要过滤掉权限不够的错误
        if(errno!=13)
            perror("opendir fail");
        else
            return ;
    }
    //找到最长文件名，并且统计文件个数
    while((ptr = readdir(dir ))!=NULL)
    {
        if(g_maxlen< strlen(ptr->d_name))
            g_maxlen = strlen(ptr->d_name);
        count ++;

    }
    if(count >256)
    {
        printf("too many files under this dir\n");
        exit(0);
    }
    closedir(dir);
    int i,j;
    dir = opendir(path);
    if(dir == NULL)
        perror("opendir fail:");
    int len=strlen(path);
    for(i=0;i<count;i++)
    {
        ptr = readdir(dir);
        if(ptr == NULL)
            perror("readdir fail:");
        strncpy(filenames[i],path,len);
        filenames[i][len] = '\0';
        strcat(filenames[i],ptr->d_name);
        filenames[i][len+strlen(ptr->d_name)] = '\0';
    }

    //冒泡法排序
    for(i=0;i<count -1;i++)
    {
        for(j=0;j<count-1-i;j++)
        {
            if(strcmp(filenames[j],filenames[j+1])>0)
            {
                strcpy(temp,filenames[j+1]);
                temp[strlen(filenames[j+1])] = '\0';
                strcpy(filenames[j+1],filenames[j]);
                filenames[j+1][strlen(filenames[j])] = '\0';
                strcpy(filenames[j],temp);
                filenames[j][strlen(temp)] = '\0';
            }
        }
    }

    //-r   逆序输出
    if(flag_param & PARAM_r)
    {
        for(i=count-1;i>=0;i--)
            display(flag_param,filenames[i]);
    }
    for(i=0;i<count ;i++)
        display(flag_param,filenames[i]);

    closedir(dir);

    if((flag_param &PARAM_L) == 0)
        printf("\n");
}
void display_dir_R(int flag,char *name)
{
    DIR *dir;
    struct dirent *ptr;
    char temp_name[PATH_MAX+1];
    printf("%s:\n",name);
    display_dir(flag,name);
    printf("\n");
    if((dir = opendir(name)) == NULL)
    {
        if(errno!=13)
            perror("display_R fail:");
        else 
            return ;
    }
    int len = strlen(name);
    while((ptr = readdir(dir ))!=NULL)
    {
        if(ptr->d_type == DT_DIR && (ptr -> d_name[0]!='.'))
        {
            strncpy(temp_name,name,len);
            temp_name[len] = '\0';
            strcat (temp_name,ptr->d_name);
            temp_name[len+strlen(ptr->d_name)] = '/';
            temp_name[len+strlen(ptr->d_name)+1] = '\0';
            display_dir_R(flag,temp_name);
        }
    }
    closedir(dir);
    return ;
}

//对不同的文件类型给不同的颜色
int get_color(struct stat buf)
{
    int color = 0;
    if(S_ISLNK(buf.st_mode))
    {
        color = LBLUE;
    }
    else if(S_ISDIR(buf.st_mode))
    {
        color = BLUE;
    }
    else if(S_ISCHR(buf.st_mode) ||S_ISBLK(buf.st_mode) )
    {
        color = YELLOW;
    }
    else if(buf.st_mode & S_IXUSR)
    {
        color = GREEN;
    }
    return color;
}

void printf_name(char *name,int color)
{
    if(color == GREEN)
    {
        printf("\033[1m\033[32m%-s\033[0m",name);
    }
    else if(color == BLUE)
    {
        printf("\033[1m\033[34m%-s\033[0m",name);
    }
    else if(color == WHITE)
    {
        printf("%-s",name);
    }
    else if(color == LBLUE)
    {
        printf("\033[1m\033[36m%-s\033[0m",name);
    }
    else if(color == YELLOW)
    {
        printf("\033[1m\033[33m%-s\033[0m",name);
    }
}
