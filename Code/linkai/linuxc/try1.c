
/*************************************************************************
    > File Name: try1.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年05月04日 星期一 20时07分08秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void MyFun1(int x);  
void MyFun2(int x);  
void MyFun3(int x);  
typedef void (*FunType)(int ); //②. 定义一个函数指针类型FunType,与①函数类型一至
void CallMyFun(FunType fp,int x);

int main(int argc, char* argv[])
{
   CallMyFun(MyFun1,10);   //⑤. 通过CallMyFun函数分别调用三个不同的函数
   CallMyFun(MyFun2,20);   
   CallMyFun(MyFun3,30);   
}
void CallMyFun(FunType fp,int x) //③. 参数fp的类型是FunType。
{
  fp(x);//④. 通过fp的指针执行传递进来的函数，注意fp所指的函数是有一个参数的
}
void MyFun1(int x) // ①. 这是个有一个参数的函数，以下两个函数也相同
{
   printf(“函数MyFun1中输出：%d\n”,x);
}
void MyFun2(int x)  
{
   printf(“函数MyFun2中输出：%d\n”,x);
}
void MyFun3(int x)  
{
   printf(“函数MyFun3中输出：%d\n”,x);
}
