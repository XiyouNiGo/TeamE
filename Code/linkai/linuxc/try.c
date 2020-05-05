
/*************************************************************************
  > File Name: try.c
  > Author: NiGo
  > Mail: nigo@xiyoulinux.org
  > Created Time: 2020年04月22日 星期三 11时35分33秒
 ************************************************************************/

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

#define CLOSE "\001\033[0m\002"                 // 关闭所有属性
#define BLOD  "\001\033[1m\002"                 // 强调、加粗、高亮
#define BEGIN(x,y) "\001\033["#x";"#y"m\002"    // x: 背景，y: 前景

static char *line_read = (char *)NULL;
static char prompt[256];

void set_prompt(char*);

char *rl_gets ()
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

    return (line_read);
}

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

int main(void)
{
    /*read_history(NULL);
    while (1)
    {
        //char * str = readline(BEGIN(49, 34),str, CLOSE);
        char *str = rl_gets();
        if ((!strcmp("exit", str)) || (!strcmp("logout", str)))
            break;
    }
  
    write_history(NULL);
    //rl_gets();*/
    printf("%s", readline(""));
}
