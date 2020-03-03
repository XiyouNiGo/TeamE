/*************************************************************************
    > File Name: 抽奖小程序.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年03月03日 星期二 19时14分37秒
 ************************************************************************/

#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#define SIZE 50
int random_number(int n);
void mainmenu();

int main()
{
	char candidate[SIZE][SIZE];
	int player, winner, temp;
	int output[SIZE] = {0};
	memset(candidate, 0, sizeof(candidate));
	mainmenu();
	puts("请输入抽奖总人数：");
	scanf("%d", &player);
	for (int i = 1; i <= player; i++)
	{
		printf("请输入%d号选手：", i);
		scanf("%s", candidate[i]);
	}
	puts("请输入中奖人数：");
	scanf("%d", &winner);
	for (int i = 1; i <= winner; i++)
	{
		while (output[i] == (temp = random_number(player)));
		output[i] = temp;
	}
	puts("抽奖中...");
	system("sleep 2s");
	system("clear");
	puts("叮～本次抽奖结果新鲜出炉！");
	putchar('\a');
	puts("中奖名单：");
	for (int i = 1; i <= winner; i++)
	{
		printf("%d号中奖选手:%s\n", i, candidate[output[i]]);
	}
	puts("让我们恭喜这些中奖选手！");

	return 0;
}
//返回一个1到n之间的随机数
int random_number(int n)
{
	srand((unsigned int)time(NULL));
	return (rand()%n)+1;
}
//主界面
void mainmenu()
{
	system("clear");
	puts("****************************");
	puts("-----简易(陋)抽奖小程序-----");
	puts("-----Coldplay一家人专用-----");
	puts("****************************");
}
