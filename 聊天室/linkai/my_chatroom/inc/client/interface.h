#ifndef _INTERFACE_H_
#define _INTERFACE_H_
//显示
#define CLOSE() printf("\033[0m")
#define BLOD() printf("\033[1m")
#define UNDERLINE() printf("\033[4m")
#define FLASH() printf("\033[5m")
#define REVERSE() printf("\033[7m")
#define BEGIN(x,y) printf("\033["#x";"#y"m")
//清屏
#define CLEAR() printf("\033[2J")
#define CLEAR_LINE() printf("\033[K")
//光标
#define MOVEUP(x) printf("\033[%dA", (x))
#define MOVEDOWN(x) printf("\033[%dB", (x))
#define MOVELEFT(y) printf("\033[%dD", (y))
#define MOVERIGHT(y) printf("\033[%dC",(y))
#define MOVETO(x,y) printf("\033[%d;%dH", (x), (y))
#define SAVE_CURSOR() printf("\033[s")
#define RESTORE_CURSOR() printf("\033[u")
#define HIDE_CURSOR() printf("\033[?25l")
#define SHOW_CURSOR() printf("\033[?25h")
//窗口大小
#define HEIGHT 30
#define WIDTH 94

char getch();

char *s_gets(char *st, int n);

void secret_pwd(char *pwd);

char get_first_alpha();

void press_to_continue();

void show_line(const char *line);

char *get_filename(char *prompt);

void initial_animation();

int get_int();

int get_choice_int(int min, int max);

#endif
