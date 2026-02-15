#define _CRT_SECURE_NO_WARNINGS 1

///*求两个整数的异或，并按顺序输出*/
//
//#include<stdio.h>
//#include<stdlib.h>
//#include<string.h>
//
////开辟内存
//int* kaipi(int n)
//{
//	int i = 0;
//	int* a = (int*)malloc(sizeof(int) * (n + 1));
//	for (i = 0; i < n; i++)
//	{
//		scanf("%d", &a[i]);
//	}
//	return a;
//}
//
////对照并删除
//void yihuo(int* a, int* b,int n, int m)
//{
//	int i = 0, j = 0;
//	for (i = 0; i < n; i++)
//	{
//		for (j = 0; j < m; j++)
//		{
//			if (a[i] == b[j])
//			{
//				a[i] = NULL;
//				n--;
//				b[j] = NULL;
//				m--;
//			}
//		}
//	}
//}
//
////排序
//int* paixu(int* a, int* b, int n, int m, int len)
//{
//	int i = 0, j = 0;
//	len = n + m;
//	int* c = kaipi(len);
//
//	for (i = 0; i < len; i++)
//	{
//		if (i <= n)
//		{
//			c[i] = a[i];
//		}
//		else
//		{
//			c[i] = b[i];
//		}
//	}
//
//	for (i = 0; i < len; i++)
//	{
//		for (j = i; j < len-1; j++)
//		{
//			if (c[i] > c[i + 1])
//			{
//				int temp = c[i];
//				c[i] = c[i + 1];
//				c[i + 1] = temp;
//			}
//		}
//	}
//	return c;
//}
//
////输出
//void shuchu(int* c, int len)
//{
//	int i = 0;
//	for (i = 0; i < len; i++)
//	{
//		printf("%d ", c[i]);
//	}
//}
//
//int main(void)
//{
//	int n = 0, m = 0;
//	scanf("%d", &n);
//	int* a = kaipi(n);
//	scanf("%d", &m);
//	int* b = kaipi(m);
//
//	yihuo(a, b, n, m);
//	int len = 0;
//	int* c = paixu(a, b, n, m, len);
//
//	shuchu(c, len);
//
//	free(a);
//	free(b);
//	free(c);
//	return 0;
//}

///*字符串递归、递推*/
//#include <stdio.h>
//#include <math.h>
//
//void An(int n, int m)
//{
//	if (m >= n || n == 1)
//	{
//		printf("sin(%d)",n);
//	}
//	else
//	{
//		printf("sin(");
//		printf("%d", m);
//		m % 2 == 0 ? printf("+") : printf("-");
//		An(n, m + 1);
//		printf(")");
//	}
//}
//
//void Sn(int n, int m)
//{
//	int total = n;
//	int i = 0;
//
//	if (n < 1)
//	{
//		printf("+1");
//	}
//	else
//	{
//		if (m == 1)
//		{
//			for (i = 0; i < total - 1; i++)
//			{
//				printf("(");
//			}
//		}
//		else
//		{
//			printf(")");
//		}
//		//printf("A");
//		An(m, 1);
//		if (n > 1)
//		{
//			printf("+");
//			printf("%d", n);
//		}
//		Sn(n - 1, m + 1);
//	}
//}
//
//
//int main(void)
//{
//	int n = 0;
//	scanf("%d", &n);
//	//An(n, 1);
//	Sn(n,1);
//	return 0;
//}

///*十六进制转八进制*/
//#include<stdio.h>
//#include<stdlib.h>
//
//typedef struct n {
//	int data;
//	node* next;
//}node;
//
//node* biaotou()
//{
//	node* a;
//	a->data = 0;
//	a->next = NULL;
//
//	return a;
//}
//
////十六进制转二进制(字符到整型)
//void H(node* B,int* str)
//{
//
//}
//
////二进制转八进制(整形到整形)
//void O(int* str, int ba)
//{
//
//}