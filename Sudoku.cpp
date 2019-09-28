#include "stdafx.h"
#include "cstdlib"
#include "stdio.h"
#include <math.h>
#pragma warning(disable : 4996)

int sudoku[10][10] = { 0 };					//记录原始的数独数据,其中0为未知
int sudoku_s[10][10] = { 0 };				//记录原始的数独状态,1为已确定,0为未知
int sudoku_r[10][10] = { 0 };				//记录临时的数独数据,其中0为未知
int sudoku_t[10][10][10] = { 0 };			//每个单元填入数据的合法性记录,值为1表示不可以选择该数字填入
int m, n;
FILE *ip, *op;
void read_sudoku(int sudoku[10][10], int sudoku_t[10][10][10], int m);
int guess_xy(int sudoku_t[10][10][10], int x, int y);
void refresh(int sudoku_t[10][10][10], int x, int y, int m, int k);
int unknown_count(int sudoku_s[10][10], int m);
void find(int sudoku_t[10][10][10], int m);

int main(int argc, char *argv[])
{
	m = (int)argv[2];			//m 宫格阶级（3~9的整数）
	n = (int)argv[4];			//n 待解答盘面数目
	char *i = argv[6];			//刚开始忘了argv是*char,以为是FILE*
	char *o = argv[8];
	if ((ip = fopen(i, "r")) == NULL) {
		printf("open file fail!\n");
		exit(EXIT_SUCCESS);
	}
	read_sudoku(sudoku, sudoku_t, m);;
	int g = unknown_count(sudoku_s, m);
	for (int h = 1; h <= g; h++)
	{
		find(sudoku_t, m);
	}
	if ((ip = fopen(i, "w+")) == NULL) {
		printf("open file fail!\n");
		exit(EXIT_SUCCESS);
	}

	fclose(ip);
	fclose(op);
	return 0;
}

void read_sudoku(int sudoku[10][10],int sudoku_t[10][10][10],int m) {
	for (int i = 1; i <= m; i++)				//每次读文件麻烦,直接保存在内存
	{
		for (int j = 1; j <= m; j++)
		{
			sudoku_r[i][j] = sudoku[i][j] = fgetc(ip); fgetc(ip);
			if (sudoku[i][j] != 0)
			{
				sudoku_s[i][j] = 1;
				for (int k = 1; k <= m; k++)
				{
					sudoku_t[i][j][k] = 1;		//排除法记录每格允许填入的数字,配合猜测函数guess()使用
				}								
			}
		}
	}
}

int guess_xy(int sudoku_t[10][10][10],int x,int y) {		//若(x,y)对应的格子只有唯一一种可能则填入
	int k = 0;
	//for (int i = 1; i <= m; i++)
	//{
		for (int j = 1; j <= 9; j++)
		{
			k = k + sudoku_t[x][y][j];
		}
	//}
	if ( k == 8 )
	{
		for (int i = 1; i <= 9; i++) {
			if (sudoku_t[x][y][i] == 0)
			{
				sudoku_r[x][y] = i;		//修改数据记录
				sudoku_s[x][y] = 1;		//修改已知记录，注意合法性记录还没改
				return i;
			}
		}
	}
	else {
		return 0;
	}
}

void refresh(int sudoku_t[10][10][10], int x, int y,int m,int k) {		//刷新合法性记录,注意k表示(x,y)处只能选k
	for (int i = 1; i <= m ; i++)
	{
		if(i != y)sudoku_t[x][i][k] = 1;
		if(i != x)sudoku_t[i][y][k] = 1;
		if (m == 4) {
			if ((x == 1 || x == 3) && (y == 1 || y == 3))
			{
				sudoku_t[x + 1][y][k] = sudoku_t[x][y + 1][k] = sudoku_t[x + 1][y + 1][k] = 1;
			}
			else if ((x == 1 || x == 3) && (y == 2 || y == 4))
			{
				sudoku_t[x + 1][y][k] = sudoku_t[x][y - 1][k] = sudoku_t[x + 1][y - 1][k] = 1;
			}
			else if ((x == 2 || x == 4) && (y == 1 || y == 3))
			{
				sudoku_t[x - 1][y + 1][k] = sudoku_t[x - 1][y][k] = sudoku_t[x][y + 1][k] = 1;
			}
			else if ((x == 1 || x == 3) && (y == 1 || y == 3))
			{
				sudoku_t[x - 1][y][k] = sudoku_t[x][y - 1][k] = sudoku_t[x - 1][y - 1][k] = 1;
			}
		}
		if (m == 6)
		{
			if ((x == 1 || x == 4) && (y == 1 || y == 3 || y == 5 ))
			{
				sudoku_t[x + 1][y][k] = sudoku_t[x + 2][y][k] = sudoku_t[x][y + 1][k] = sudoku_t[x + 1][y + 1][k] = sudoku_t[x + 2][y + 1][k] = 1;
			}
			else if ((x == 2 || x == 5) && (y == 1 || y == 3 || y == 5))
			{
				sudoku_t[x - 1][y][k] = sudoku_t[x + 1][y][k] = sudoku_t[x - 1][y + 1][k] = sudoku_t[x][y + 1][k] = sudoku_t[x + 1][y + 1][k] = 1;
			}
			else if ((x == 3 || x == 6) && (y == 1 || y == 3 || y == 5))
			{
				sudoku_t[x - 2][y][k] = sudoku_t[x - 1][y][k] = sudoku_t[x - 2][y + 1][k] = sudoku_t[x - 1][y + 1][k] = sudoku_t[x][y + 1][k] = 1;
			}
			else if ((x == 1 || x == 4) && (y == 2 || y == 4 || y == 6))
			{
				sudoku_t[x + 1][y][k] = sudoku_t[x + 2][y][k] = sudoku_t[x][y - 1][k] = sudoku_t[x + 1][y - 1][k] = sudoku_t[x + 2][y - 1][k] = 1;
			}
			else if ((x == 2 || x == 5) && (y == 2 || y == 4 || y == 6))
			{
				sudoku_t[x - 1][y][k] = sudoku_t[x + 1][y][k] = sudoku_t[x - 1][y - 1][k] = sudoku_t[x][y - 1][k] = sudoku_t[x + 1][y - 1][k] = 1;
			}
			else if ((x == 3 || x == 6) && (y == 2 || y == 4 || y == 6))
			{
				sudoku_t[x - 2][y][k] = sudoku_t[x - 1][y][k] = sudoku_t[x - 2][y - 1][k] = sudoku_t[x - 1][y - 1][k] = sudoku_t[x][y - 1][k] = 1;
			}
		}
		if (m == 8)
		{
			if ((x == 1 || x == 3 || x == 5 || x == 7) && (y == 1 || y == 5))
			{
				sudoku_t[x + 1][y][k] = sudoku_t[x][y + 1][k] = sudoku_t[x][y + 2][k] = sudoku_t[x][y + 3][k] = sudoku_t[x + 1][y + 1][k] = sudoku_t[x + 1][y + 2][k] = sudoku_t[x + 1][y + 3][k] = 1;
			}
			else if ((x == 1 || x == 3 || x == 5 || x == 7) && (y == 2 || y == 6))
			{
				sudoku_t[x + 1][y][k] = sudoku_t[x][y + 1][k] = sudoku_t[x][y + 2][k] = sudoku_t[x][y - 1][k] = sudoku_t[x + 1][y + 1][k] = sudoku_t[x + 1][y + 2][k] = sudoku_t[x + 1][y - 1][k] = 1;
			}
			else if ((x == 1 || x == 3 || x == 5 || x == 7) && (y == 3 || y == 7))
			{
				sudoku_t[x][y - 2][k] = sudoku_t[x][y - 1][k] = sudoku_t[x][y + 1][k] = sudoku_t[x + 1][y - 2][k] = sudoku_t[x + 1][y - 1][k] = sudoku_t[x + 1][y][k] = sudoku_t[x + 1][y + 1][k] = 1;
			}
			else if ((x == 1 || x == 3 || x == 5 || x == 7) && (y == 4 || y == 8))
			{
				sudoku_t[x][y - 3][k] = sudoku_t[x][y - 2][k] = sudoku_t[x][y - 1][k] = sudoku_t[x + 1][y - 3][k] = sudoku_t[x + 1][y - 2][k] = sudoku_t[x + 1][y - 1][k] = sudoku_t[x + 1][y][k] = 1;
			}
			else if ((x == 2 || x == 4 || x == 6 || x == 8) && (y == 1 || y == 5))
			{
				sudoku_t[x - 1][y][k] = sudoku_t[x][y + 1][k] = sudoku_t[x][y + 2][k] = sudoku_t[x][y + 3][k] = sudoku_t[x - 1][y + 1][k] = sudoku_t[x - 1][y + 2][k] = sudoku_t[x - 1][y + 3][k] = 1;
			}
			else if ((x == 2 || x == 4 || x == 6 || x == 8) && (y == 2 || y == 6))
			{
				sudoku_t[x - 1][y][k] = sudoku_t[x][y + 1][k] = sudoku_t[x][y + 2][k] = sudoku_t[x][y - 1][k] = sudoku_t[x - 1][y + 1][k] = sudoku_t[x - 1][y + 2][k] = sudoku_t[x - 1][y - 1][k] = 1;
			}
			else if ((x == 2 || x == 4 || x == 6 || x == 8) && (y == 3 || y == 7))
			{
				sudoku_t[x][y - 2][k] = sudoku_t[x][y - 1][k] = sudoku_t[x][y + 1][k] = sudoku_t[x - 1][y - 2][k] = sudoku_t[x - 1][y - 1][k] = sudoku_t[x - 1][y][k] = sudoku_t[x - 1][y + 1][k] = 1;
			}
			else if ((x == 2 || x == 4 || x == 6 || x == 8) && (y == 4 || y == 8))
			{
				sudoku_t[x][y - 3][k] = sudoku_t[x][y - 2][k] = sudoku_t[x][y - 1][k] = sudoku_t[x - 1][y - 3][k] = sudoku_t[x - 1][y - 2][k] = sudoku_t[x - 1][y - 1][k] = sudoku_t[x - 1][y][k] = 1;
			}
		}
		if (m == 9)
		{
			int u = (int)floor((x-1) / 3) + 1;
			int v = (int)floor((y-1) / 3) + 1;
			sudoku_t[u - 1][v - 1][k] = sudoku_t[u - 1][v][k] = sudoku_t[u - 1][v + 1][k] = sudoku_t[u][v - 1][k] = sudoku_t[u][v + 1][k] = sudoku_t[u + 1][v - 1][k] = sudoku_t[u + 1][v][k] = sudoku_t[u + 1][v + 1][k] = 1;
		}
	}
}

int unknown_count(int sudoku_s[10][10],int m) {			//统计未知的格子个数
	int k = 0;	
	for (int i = 1; i <=m; i++)
	{
		for (int j = 1; j <= m; j++)
		{
			if (sudoku_s[i][j] == 0)
			{
				k++;
			}
		}
	}
	return k;
}

void find(int sudoku_t[10][10][10],int m) {
	for (int x = 1; x <= m; x++)
	{
		for (int y = 1; y <= m; y++)
		{
			int k = guess_xy(sudoku_t, x, y);
			if (k)
			{
				refresh(sudoku_t, x, y, m, k);
			}
		}
	}
}

void write_sudoku() {
	for (int i = 1; i <= m; i++)
	{
		for (int j = 0; j <m; j++)
		{
			fputc((char)sudoku_r[i][j], op);
			fputc(' ', op);
		}
		fputc((char)sudoku_r[i][m], op);
		fputc('\n', op);
	}
}





/*
short guess(short sudoku[10][10], short sudoku_s[10][10], bool sudoku_t[10][10][10], short m) {
for (short i = 1; i <= m; i++)
{
for (short j = 1; j <= m; j++)
{
short k = 1;
if (sudoku_s[i][j] == 0)
{
do
{
if (sudoku_t[i][j][k] == 0) {
sudoku_r[i][j] = k;
break;
k++;
}
} while (1);
}
}
}

}
*/
/*
short check_x(short* sudoku_t[10][10][10]) {
int i = 1;
int j = 1;
int k = 1;
while (k<=m) {
if (sudoku_t[i][j][k])
{
return 1;
}
k++;
}
return 0;
}
*/
//int guess_x(int sudoku_t[10][10][10],int x,int y) {
//	int k = 0;
//	for (int i = 1; i <= m; i++)
//	{
//		for (int j = 1; j <= 9; j++)
//		{
//			k = k + sudoku_t[x][y][j];
//		}
//	}
//	if ( k == 8 )
//	{
//		for (int i = 1; i <= 9; i++) {
//			if (sudoku_t[x][y][i] == 0)
//			{
//				sudoku_r[x][y] = sudoku_t[x][y][i];
//				//sudoku_s[x][y] = 1;
//				return i;
//			}
//		}
//	}
//	else {
//		return 0;
//	}
//}