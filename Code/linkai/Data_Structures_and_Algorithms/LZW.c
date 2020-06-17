
/*************************************************************************
    > File Name: LZW.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年06月17日 星期三 22时22分24秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//定义字典结构
typedef struct
{
    char **sequence; //实际指向字符串的二级指针
    int *code;      //给字符串编号
    int size;       //当前长度
    int max_size;   //最大长度
}Dictionary;
//向字典中插入字符串str
void insert_sequence(Dictionary *dict, char *str)
{
    int i = dict->size;
    dict->sequence[i] = (char*)malloc(sizeof(char) * strlen(str) + 1);
    dict->code[i] = i;
    dict->size++;
    strcpy(dict->sequence[i], str);
}
//初始化字典
void init_dictionary(Dictionary *dict, int max_size)
{
    dict->max_size = max_size;
    dict->size = 0;
    dict->sequence = (char**)malloc(sizeof(char*) * max_size);
    dict->code = (int*)malloc(sizeof(int) * max_size);
    //插入哑值(任意)
    insert_sequence(dict, (char*)"#");
    //插入A到D字母(只有)
    char letter[2] = "A";
    for (int i = 0; i < 26; i++)
    {
        insert_sequence(dict, letter);
        letter[0]++;
    }
}
//打印字典
void print_dictionary(Dictionary *dict)
{
    printf("====================\n");
    printf(" Code       Sequence\n");
    printf("====================\n");
    for (int i = 0; i < dict->size; i++)
    {
        printf("%5d%7c%s\n", i, ' ', dict->sequence[i]);
    }
    printf("====================\n");
}

int main(int argc, char *argv[])
{
    Dictionary dict;
    init_dictionary(&dict, 100);
    print_dictionary(&dict);

    return 0;
}
