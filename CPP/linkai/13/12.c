/*************************************************************************
    > File Name: 12.c
    > Author: NiGo
    > Mail: nigo@xiyoulinux.org
    > Created Time: 2020年02月16日 星期日 16时43分46秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include"s_gets.h"
#define ROWS 20
#define COLS 30
#define LEVELS 10

const char trans[LEVELS + 1] = " .':~*=&%@";
void MakePic(int data[][COLS], char pic[][COLS], int rows);
void init(char arr[][COLS], char ch);
int SavePic(char picOut[][COLS], FILE * fp);

int main()
{
	int row, col;
	int picIn[ROWS][COLS];
	char picOut[ROWS][COLS];
	char filename1[81];
	char filename2[81];
	FILE * infile;
	FILE * outfile;

	init(picOut, '0');
	printf("Please enter name of file you need to analyse: \n");
	s_gets(filename1, 80);
	printf("Please enter name of file you need to save: \n");
	s_gets(filename2, 80);
	
	if ((infile = fopen(filename1, "r")) == NULL)
	{
		fprintf(stderr, "Could not open input file.\n");
		exit(EXIT_FAILURE);
	}
	if ((outfile = fopen(filename2, "w")) == NULL)
	{
		fprintf(stderr, "Could not ceate output file.\n");
		exit(EXIT_FAILURE);
	}
	for (row = 0; row < ROWS; row++)
	for (col = 0; col < COLS; col++)
	fscanf(infile, "%d", &picIn[row][col]);
	if (ferror(infile))
	{
		fprintf(stderr, "Error getting data from file.\n");
		exit(EXIT_FAILURE);
	}
	MakePic(picIn, picOut, ROWS);
	for (row = 0; row < ROWS; row++)
	{
		for (col = 0; col < COLS; col++)
			putchar(picOut[row][col]);
		putchar('\n');
	}

	if(SavePic(picOut, outfile) == 1)
	{
		fprintf(stderr, "Error writing data into file.\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}

void init(char arr[][COLS], char ch)
{
	int r, c;

	for (r = 0; r < ROWS; r++)
		for (c = 0; c < COLS; c++)
			arr[r][c] = ch;
}

void MakePic(int data[][COLS], char pic[][COLS], int rows)
{
	int row, col;

	for (row = 0; row < rows; row++)
		for (col = 0; col < COLS; col++)
			pic[row][col] = trans[data[row][col]];
}

int SavePic(char picOut[][COLS], FILE * fp)
{
	int row, col;

	for (row = 0; row < ROWS; row++)
		for (col = 0; col < COLS; col++)
		{
			fputc(picOut[row][col], fp);
			if (col == COLS-1)
				fputc('\n', fp);
			else
				fputc(' ', fp);
		}

	if (ferror(fp) != 0)
		return 1;

	return 0;
}
