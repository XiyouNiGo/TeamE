
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
    char **sequence;//实际指向字符串的二级指针
    int *code;      //给字符串编号
    int size;       //当前长度
    int max_size;   //最大长度
}Dictionary;
//压缩后的编码
int key[1000];
//对应下标
int count = 0;
//向key中插入编码
void insert_key(int code)
{
    key[count++] = code;
}
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
//已知字符串,查询字符串是否存在,存在返回编码(压缩用)
int get_code(Dictionary *dict, char *str)
{
    for (int i = 0; i < dict->size; i++)
    {
        if (strcmp(dict->sequence[i], str) == 0)
        {
            return dict->code[i];
        }
    }
    return -1;
}
//已知编码,获取字符串(解压用)
char *get_sequence(Dictionary *dict, int code)
{
    return dict->sequence[code];
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

void print_key()
{
    for (int i = 0; i < count; i++)
    {
        printf("%d ", key[i]);
    }
    putchar('\n');
}
//压缩编码
void lzw_encode(char *text, Dictionary *dict)
{
    char current[1000];
    char next;
    int code;
    int i = 0;
    while (i < strlen(text) - 2)
    {
        //先把第i个字符存入(对于后续循环，该处也有一个清空current的作用)
        sprintf(current, "%c", text[i]);
        next = text[i+1];
        //反复查询并添加current,直到current为最新,退出循环
        while (get_code(dict, current) != -1)
        {
            //  i   current     next
            //  T         T      O  
            //  O        TO      B
            //  B       TOB      C
            sprintf(current, "%s%c", current, next);
            i++;
            next = text[i+1];
        }
        insert_sequence(dict, current);
        char copy[1000];
        //编码并存入数组key(去掉最后的字母,因为是字典里的)
        strcpy(copy, current);
        copy[strlen(current) - 1] = '\0';
        insert_key(get_code(dict, copy));
    }
}
//解压编码
void lzw_decode(Dictionary *dict)
{
    int code;
    char prev[1000];
    char *output;
    output = get_sequence(dict, key[0]);
    //第一项无前一项，单独输出   
    printf("%s", output);
    for (int i = 1; i < count; i++)
    {
        code = key[i];
        strcpy(prev, output);
        output = get_sequence(dict, code);
        printf("%s", output);
        //此处把prev作为一个临时字符串(反正上面strcpy时会更新)
        sprintf(prev, "%s%c", prev, output[0]);
        insert_sequence(dict, prev);
    }
}

int main(int argc, char *argv[])
{
    Dictionary dict;
    char *text = "TOBEORNOTTOBEORTOBEORNOT";
    init_dictionary(&dict, 100);
    lzw_encode((char*)text, &dict);
    print_dictionary(&dict);
    printf("Text : \n%s\n", text);
    printf("Code : \n");
    print_key();
    //重新初始化用于解码
    init_dictionary(&dict, 100);
    printf("Decode : \n");
    lzw_decode(&dict);
    return 0;
}
