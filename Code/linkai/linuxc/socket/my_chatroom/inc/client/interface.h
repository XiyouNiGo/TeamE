#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#define CLOSE "\001\033[0m\002"                 // 关闭所有属性
#define BLOD  "\001\033[1m\002"                 // 强调、加粗、高亮
#define BEGIN(x,y) "\001\033["#x";"#y"m\002"    // x: 背景，y: 前景

char getch();
char * s_gets(char *st, int n);
void secret_pwd(char *pwd);
char get_first_alpha();
void press_to_continue();
void show_line(const char *line);
char *get_filename(char *prompt);

#endif
