/*************************************************************************
    > File Name: 9.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月16日 星期日 15时05分17秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#define MAX 41
int main(void)
{
    FILE *fp;
    char words[MAX];
    int count = 0;

    if ((fp = fopen("wordy", "a+")) == NULL)
    {
		fprintf(stderr,"Can't open \"words\" file.\n");
		exit(EXIT_FAILURE);
	}
	rewind(fp);
	while (fgets(words, MAX, fp) != NULL)
		count++;
	rewind(fp);
	puts("Enter words to add to the file; press the #");
	puts("key at the beginning of a line to terminate.");
	while ((fscanf(stdin,"%40s", words) == 1) && (words[0] != '#'))
		fprintf(fp, "%3d: %s\n", ++count, words);
	puts("File contents:");
	rewind(fp);
	while (fgets(words, MAX, fp) != NULL)
		fputs(words, stdout);
	if (fclose(fp) != 0)
	fprintf(stderr,"Error closing file\n");

	puts("Done");
	return 0;
}

