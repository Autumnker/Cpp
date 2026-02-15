//#define _CRT_SECURE_NO_WARNINGS 1
//
///*2n皇后问题*/
//#include<stdio.h>
//#include<stdlib.h>
//
//typedef struct block {
//	int black;
//	int white;
//}Node;
//
//Node chushihua()
//{
//	Node a;
//	a.black = 0;
//	a.white = 0;
//	return a;
//}
//
////避开自己的染色区域
//int run(int q, int** b, int i, int j)
//{
//	int flag = 0;
//
//	if (q == 1)
//	{
//		if (b[i][j] == 3 || b[i][j] == 7 || b[i][j] == 1)
//			flag = 1;
//	}
//	else
//	{
//		if (b[i][j] == 4 || b[i][j] == 7 || b[i][j] == 2)
//			flag = 1;
//	}
//	return flag;
//}
//
////皇后染色
//void stain(int q,int x, int y, Node** board, int size)//x、y的最大值 = size - 1
//{
//	int i = 0, j = 0;
//
//	//落子处染色
//	if (q == 1)
//	{
//		board[x][y].black = 1;
//
//		//行染色
//		for (j = 0; j < size; j++)
//		{
//			if (j == y)
//			{
//				continue;
//			}
//			else
//			{
//				board[x][j].black = 1;
//			}
//		}
//
//		//列染色
//		for (i = 0; i < size; i++)
//		{
//			if (i == x)
//			{
//				continue;
//			}
//			else
//			{
//				board[i][y].black = 1;
//			}
//		}
//
//		//主对角线染色
//		//右下
//		for (i = x, j = y; i < size && j < size; i++, j++)
//		{
//			if (i == x && j == y)
//				continue;
//			else
//			{
//				board[i][j].black = 1;
//			}
//		}
//		//左上
//		for (i = x, j = y; i >= 0 && j >= 0; i--, j--)
//		{
//			if (i == x && j == y)
//				continue;
//			else
//			{
//				board[i][j].black = 1;
//			}
//		}
//		//副对角线染色
//		//左下
//		for (i = x, j = y; i < size && j >= 0; i++, j--)
//		{
//			if (i == x && j == y)
//				continue;
//			else
//			{
//				board[i][j].black = 1;
//			}
//		}
//		//右上
//		for (i = x, j = y; i >= 0 && j < size; i--, j++)
//		{
//			if (i == x && j == y)
//				continue;
//			else
//			{
//				board[i][j].black = 1;
//			}
//		}
//	}
//	else if (q == 2)
//	{
//		board[x][y].white = 1;
//
//		//行染色
//		for (j = 0; j < size; j++)
//		{
//			if (j == y)
//			{
//				continue;
//			}
//			else
//			{
//				board[x][j].white = 1;
//			}
//		}
//
//		//列染色
//		for (i = 0; i < size; i++)
//		{
//			if (i == x)
//			{
//				continue;
//			}
//			else
//			{
//				board[i][y].white = 1;
//			}
//		}
//
//		//主对角线染色
//		//右下
//		for (i = x, j = y; i < size && j < size; i++, j++)
//		{
//			if (i == x && j == y)
//				continue;
//			else
//			{
//				board[i][j].white = 1;
//			}
//		}
//		//左上
//		for (i = x, j = y; i >= 0 && j >= 0; i--, j--)
//		{
//			if (i == x && j == y)
//				continue;
//			else
//			{
//				board[i][j].white = 1;
//			}
//		}
//		//副对角线染色
//		//左下
//		for (i = x, j = y; i < size && j >= 0; i++, j--)
//		{
//			if (i == x && j == y)
//				continue;
//			else
//			{
//				board[i][j].white = 1;
//			}
//		}
//		//右上
//		for (i = x, j = y; i >= 0 && j < size; i--, j++)
//		{
//			if (i == x && j == y)
//				continue;
//			else
//			{
//				board[i][j].white = 1;
//			}
//		}
//	}
//}
//
////展示棋盘
//void show(int** b,int n)
//{
//	int i = 0, j = 0;
//	for (i = 0; i < n; i++)
//	{
//		for (j = 0; j < n; j++)
//		{
//			printf("%4d", b[i][j]);
//		}
//		printf("\n\n");
//	}
//}
//
////判断放满
//int full(int** b, int n)
//{
//	int i = 0, j = 0;
//	int flag = 0;
//
//	for (i = 0; i < n; i++)
//	{
//		for (j = 0; j < n; j++)
//		{
//			if (b[i][j] == 0)
//			{
//				flag = 1;
//			}
//		}
//	}
//
//	if (flag == 0)
//	{
//		return 1;
//	}
//	else
//	{
//		return 0;
//	}
//}
//
////放置棋子
//int put(int q, int** b, int n, int i, int j, int total)
//{
//	if (b[i][j] == 0)
//	{
//		stain(q, i, j, b, n);
//		if (full(b, n))
//		{
//			total++;
//			/*测试*/
//			show(b, n);
//			printf("\n\ntotal_in_put = %d\n\n", total);
//		}
//	}
//
//	return total;
//}
//
//int main(void)
//{
//	int q1 = 1, q2 = 2;
//	int i = 0, j = 0;
//
//	//创建棋盘
//	int n = 0;
//	scanf("%d", &n);
//	Node** b = (Node**)malloc(sizeof(Node*) * n);
//	for (i = 0; i < n; i++)
//	{
//		b[i] = (Node*)malloc(sizeof(Node) * n);
//
//		//初始化该行棋盘
//		for (j = 0; j < n; j++)
//		{
//			b[i][j] = chushihua();
//		}
//	}
//
//	//染色
//	//stain(q1, n/2, n/2, b, n);
//	//stain(q2, n / 2 - 1, n / 2 - 1, b, n);
//
//	int total = 0;
//	int times = 0;
//
//	//放置棋子
//	for (i = 0; i < n; i++)
//	{
//		for (j = 0; j < n; j++)
//		{
//			if (times % 2 == 0)
//			{
//				total = put(q1, b, n, i, j, total);
//				times++;
//			}
//			else
//			{
//				total = put(q2, b, n, i, j, total);
//				times++;
//			}
//		}
//	}
//
//	printf("\n\ntotal_out_side = %d\n", total);
//
//	//释放内存
//	for (i = 0; i < n; i++)
//	{
//		free(b[i]);
//	}
//	free(b);
//
//	return 0;
//}

//以上为错误示范,应该借助树的数据结构
