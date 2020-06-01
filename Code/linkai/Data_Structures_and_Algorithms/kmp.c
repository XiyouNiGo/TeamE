
/*************************************************************************
    > File Name: kmp.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年06月01日 星期一 20时54分15秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>

//初始化前缀表
//pattern[]: 模式字符串(要比较的)
//prefix_table[]: 前缀表
//n: 字符串的长度
void prefix_table_init(char pattern[], int prefix_table[], int n)
{
    prefix_table[0] = 0;    //最大共同前后缀不包括本身，只有一个字符时前缀长度肯定为0
    
    int i = 1;      //填第几位的prefix_table[]
    int j = 0;      //从数组的第几位开始匹配前面的字符串,初始当然是0

    while (i < n)
    {
        if (pattern[i] == pattern[j])   //如果相同，i和j同时后移，继续匹配后面的字符串
        {
            j++;
            prefix_table[i] = j;
            i++;
        }
        else
        {
            if (j == 0) //若第一位就不匹配
            {
                prefix_table[i] = 0;
                i++;
            }
            else
            {
                j = prefix_table[j - 1];    //重新从j之前的字符串中，最大前缀的后一位开始比较
                //继续匹配该处字符，i无需++
            }
        }
    }
}
//target[]: 目标字符串(你要从那里找的)
void kmp_search(char pattern[], char target[], int prefix_table[])
{
    int len_pattern = strlen(pattern);
    int len_target = strlen(target);

    int i = 0; //target的定位
    int j = 0; //pattern的定位
    int count = 1;  
    //跟prefix_table_init()里面的类似
    while (i < len_target)
    {
        if ((j == len_pattern - 1) && (target[i] == pattern[j])) //可以不要前缀表最后的原因就在这，最后一位匹配则匹配
        {
            printf("在下标%d开始第%d次找到%s\n", i - j, count++, pattern);
            //未达终点，继续寻找
            //仍是kmp匹配的思维
            j = prefix_table[j];
        }
        if (target[i] == pattern[j])
        {
            i++;
            j++;
        }
        else
        {
            if (j == 0)
            {
                i++;
                j++;
            }
            //否则从....开始匹配
            else
            {
                j = prefix_table[j];
            }
        }
    }
    
    if (count == 1)
    {
        printf("%s中未出现%s\n", target, pattern);
    }
}

int main(int argc, char *argv[])
{
    char pattern[] = "ABABC";
    char target[] = "ABABBABABC";

    printf("pattern: %s\ntarget: %s\n", pattern, target);

    int len = strlen(pattern);
    int prefix_table[len] = {-1};   //prefix_table[0]初始化为-1

    prefix_table_init(pattern, prefix_table + 1, len - 1);  //最后一位其实用不着

    kmp_search(pattern, target, prefix_table);

    return 0;
}
