/*************************************************************************
    > File Name: Q.c
    > Author: zll
    > Mail: zhnllion@126.com 
    > Created Time: 2020年01月18日 星期六 22时30分29秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
int main()
{
	char sentence[100];
	while(fgets(sentence,100,stdin)!=NULL)
	{
		int len=strlen(sentence),i;
		sentence[0]-=32;
		for(i=0;i<len-1;i++)
		{
			if(sentence[i]==' ')
			{
				sentence[i+1]-=32;
			}
		}
		printf("%s\n",sentence);
	}
}
