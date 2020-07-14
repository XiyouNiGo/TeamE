//哈夫曼树及哈夫曼编码

#include <stdio.h>

#define MAXVAL 65535

typedef int ElemType;

typedef struct HuffmanNode
{
    ElemType weight;
    ElemType id;    //区分权值相同的节点
    struct HuffmanTree *lchild;
    struct HuffmanTree *rchild;
}HuffmanNode;

HuffmanNode *CreateHuffmanTree(int )
