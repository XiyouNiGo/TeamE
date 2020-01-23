/*************************************************************************
    > File Name: F.c
    > Author: zll
    > Mail: zhnllion@126.com 
    > Created Time: 2020年01月17日 星期五 21时35分16秒
 ************************************************************************/

#include<string.h>
int main()
{
    int n,count,i,len;
    char arr[10050];
    scanf("%d",&n);
    while(n--)
    { 
        count=0;
        scanf("%s",arr);
        len=strlen(arr);
        for(i=0;i<len;i++) 
        if(arr[i]<'A')
        count++;
        printf("%d\n",count);
    } 
} 
