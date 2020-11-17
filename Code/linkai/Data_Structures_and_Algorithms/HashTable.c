
/*************************************************************************
    > File Name: HashTable.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年11月07日 星期六 13时23分17秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 1
#define UNSUCCESS 0
#define HASHSIZE 12
#define NULLKEY -32768

typedef int status;
typedef struct
{
    int *elem;
    int count;
}HashTable;

int m = 0;  //散列表表长
//初始化散列表
status InitHashTable(HashTable *H)
{
    H->count = m = HASHSIZE;
    H->elem = (int*)malloc(m*sizeof(int));
    for (int i = 0; i < m; i++)
        H->elem[i] = NULLKEY;
    return SUCCESS;
}
//散列函数（除留余数法）
int Hash(int key)
{
    return key % m;
}
//插入哈希值进散列表
int InsertHash(HashTable *H, int key)
{
    int addr = Hash(key);   //求散列地址
    while (H->elem[addr] != NULLKEY)    //开放定址法
        addr = (addr+1) % m;
    H->elem[addr] = key;
}
//通过散列表查找关键字
status SearchHash(HashTable H, int key, int *addr)
{
    *addr = Hash(key);
    while (H.elem[*addr] != key)
    {
        *addr = (*addr+1) % m;
        //Hash不存在
        if (*addr == Hash(key) || H.elem[*addr] == NULLKEY)
        {
            return UNSUCCESS;
        }
    }
    return SUCCESS;
}

