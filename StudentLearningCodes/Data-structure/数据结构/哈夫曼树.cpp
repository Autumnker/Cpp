//#define _CRT_SECURE_NO_WARNINGS 1
//
///*哈夫曼树,自下而上地创建*/
//#include<stdio.h>
//#include<stdlib.h>
//
///*
//1.给数组排序
//2.取出其中较小的两个值，删除它们并在数组中存入它们的和（将一个元素赋值为-1，另一个赋值为它们的和）
//3.用两个较小数创建一个二叉树（并将二叉树的根节点的地址赋值给指针）
//4.将再次生成的二叉树与先前存在的二叉树进行连接（用指针存储新的根节点的地址）
//*/
//
////定义二叉树节点
//typedef struct node {
//	int data;
//	struct node* P;
//	struct node* L;
//	struct node* R;
//}HFNode;
//
////初始化节点
//HFNode* initHFNode()
//{
//	HFNode* p = (HFNode*)malloc(sizeof(HFNode));
//
//	p->data = 0;
//	p->L = NULL;
//	p->R = NULL;
//	p->P = NULL;
//
//	return p;
//}
//
////定义包含数组首地址和两个较小数的结构体
//typedef struct MIN {
//	int* a;
//	int min_1;
//	int min_2;
//}min;
//
////数组的排序函数(返回数组首地址)
//min paixu(int* a, int n)
//{
//	int i = 0, j = 0, temp = 0;
//	for (i = 0; i < n; i++)
//	{
//		for (j = i; j < n - 1; j++)
//		{
//			if (a[j] > a[j + 1])
//			{
//				temp = a[j];
//				a[j] = a[j + 1];
//				a[j + 1] = temp;
//			}
//		}
//	}
//
//	//将较小的两个数字赋值给b、c
//	int b = -1, c = -1;
//	i = 0;
//	while (i < n)
//	{
//		if (a[i] > 0 && b == -1)
//		{
//			b = a[i];
//			a[i] = -1;//将数组的两个较小元素删除并加入它们的和
//		}
//		else if(a[i] > 0 && b != -1 && c == -1)
//		{
//			c = a[i];
//			a[i] = b + c;//将数组的两个较小元素删除并加入它们的和
//		}
//		
//		if (b != -1 && c != -1)
//		{
//			break;
//		}
//
//		i++;
//	}
//
//	min m = { a,b,c };
//
//	return m;
//}
//
//
////单个二叉树的生成函数
//HFNode* CreatSingle(int min_1, int min_2)
//{
//	HFNode* P = initHFNode();
//	HFNode* L = initHFNode();
//	HFNode* R = initHFNode();
//
//	L->data = min_1;
//	L->P = P;
//	R->data = min_2;
//	R->P = P;
//
//	P->L = L;
//	P->R = R;
//	P->data = L->data + R->data;
//
//	return P;
//}
//
////相邻新旧根节点的链接函数
//HFNode* connect()
//{
//
//}
//
////先序遍历函数
//
//int main(void)
//{
//
//	return 0;
//}