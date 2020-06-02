
/*************************************************************************
  > File Name: dfs.c
  > Author: NiGo
  > Mail: nigo@xiyoulinux.org
  > Created Time: 2020年06月02日 星期二 18时41分26秒
 ************************************************************************/
//迷宫问题
#include <stdio.h>
#include <memory.h>

int p, q;   //终点坐标
int min;
int map[101][101];          //0表示空地，1表示障碍物
int has_visit[101][101];    //0表示未访问，1表示已访问
//方向数组
int dx[4] = {0, 1, 0, -1};
int dy[4] = {1, 0, -1, 0};

void map_init(int m, int n)
{
    memset(&min, 0x3f, sizeof(min)); //初始化一个"无穷大值"
    for (int i = 0; i < 101; i++)
    {
        map[0][i] = map[i][0] = map[m+1][i] = map[i][n+1] = 1;
    }
}
//x为行，y为列
void dfs(int x, int y, int step)
{
    if (x == p && y == q)
    {
        if (step < min)
        {
            min = step;
        }
        return; //回溯
    }
    //按右下左上顺序访问
    for (int i = 0; i < 4; i++)
    {
        if (map[x+dx[i]][y+dy[i]] == 0 && has_visit[x+dx[i]][y+dy[i]] == 0)
        {
            has_visit[x+dx[i]][y+dy[i]] = 1;
            dfs(x+dx[i], y+dy[i], step+1);
            has_visit[x+dx[i]][y+dy[i]] = 0;  //回溯点设置为未访问
        }
    }
}
int main(int argc, char *argv[])
{
    int m, n;

    puts("请输入行和列");
    scanf("%d %d", &m, &n);
    map_init(m, n);

    for (int i = 1; i <= m; i++)
    {
        printf("请输入第%d行的%d个数字\n", i, n);
        for (int j = 1; j <= n; j++)
        {
            scanf("%d", &map[i][j]);
        }
    }

    puts("请输入终点坐标");
    scanf("%d %d", &p, &q);

    dfs(1, 1, 0);

    printf("需要最少步数为%d\n", min);

    return 0;
}
