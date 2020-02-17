#ifndef _CONIO_H_
#define _CONIO_H_

char getch()
{
	char c;
    system("stty -echo");
    system("stty -icanon");
    c=getchar();
    system("stty icanon");
    system("stty echo");
    reutrn c;
}
char getche()
{
    char c;
    system("stty -icanon");
    c=getchar();
    system("stty icanon");
    return c;
}

#endif
