//#define _CRT_SECURE_NO_WARNINGS 1
///*二叉排序树(简单)*/
//
///*
//步骤：
//1.定义节点；
//2.从根节点开始逐级将新数据与节点中的数据进行比较
//3.按照左小右大的顺序进行比较、插入
//*/
//
//#include<stdio.h>
//#include<stdlib.h>
////定义节点
//typedef struct node {
//	int data;
//	struct node* L;
//	struct node* R;
//}TNode;
//
////初始化节点
//TNode* initTNode()
//{
//	TNode* p = (TNode*)malloc(sizeof(TNode));
//	p->data = 0;
//	p->L = NULL;
//	p->R = NULL;
//
//	return p;
//}
//
////遍历节点(前序遍历)
//void show(TNode* p)
//{
//	if (p)
//	{
//		show(p->L);
//		printf("%d ", p->data);
//		show(p->R);
//	}
//	else
//	{
//		return;
//	}
//}
//
////创建排序树
////递归插入
//void Tpush(TNode* p, TNode* cur)
//{
//	if (p->data > cur->data)
//	{
//		if (cur->R == NULL)
//		{
//			cur->R = p;
//		}
//		else
//		{
//			Tpush(p, cur->R);
//		}
//	}
//	else if (p->data < cur->data)
//	{
//		if (cur->L == NULL)
//		{
//			cur->L = p;
//		}
//		else
//		{
//			Tpush(p, cur->L);
//		}
//	}
//}
//
//void Tcreat(TNode* T,int* a, int n)
//{
//	int i = 0;
//	T->data = a[0];
//	TNode* cur = T;
//
//	for (i = 1; i < n; i++)
//	{
//		cur = T;
//		//将新数据从头节点开始按规则进行比对，插入正确位置
//		TNode* p = initTNode();
//		p->data = a[i];
//		
//		Tpush(p, cur);
//	}
//}
//
//int main(void)
//{
//	TNode* T = initTNode();
//	int* a;
//	int n = 0;
//	printf("\n请输入数组长度：\n");
//	scanf("%d", &n);
//	a = (int*)malloc(sizeof(int) * n);
//	int i = 0;
//	printf("\n请输入数据 ：\n");
//	for (i = 0; i < n; i++)
//	{
//		scanf("%d", &a[i]);
//	}
//
//	Tcreat(T, a, n);
//	show(T);
//
//	free(a);
//	return 0;
//}