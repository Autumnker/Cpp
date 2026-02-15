#define _CRT_SECURE_NO_WARNINGS 1
///*队、入队、出队、打印队*/
//#include<stdio.h>
//#include<stdlib.h>
//
//typedef struct Node {
//	int data;
//	struct Node* next;
//}Node;
//
////初始化头节点
//Node* initQueue()
//{
//	Node* a = (Node*)malloc(sizeof(Node));
//	a->data = 0;
//	a->next = NULL;
//	return a;
//}
//
////尾插法（入队）
//void RuDui(Node* node, int data)
//{
//	Node* a = (Node*)malloc(sizeof(Node));
//	a->data = data;
//	Node* p = node;
//	int i = 0;
//	for (i = 0; i < node->data; i++)
//	{
//		p = p->next;
//	}
//	a->next = p->next;
//	p->next = a;
//
//	node->data++;
//}
//
////判断是否为空队
//int KongDui(Node* node)
//{
//	if (node->data == 0)
//	{
//		return 1;
//	}
//	else
//	{
//		return 0;
//	}
//}
//
////打印队
//void PrintDui(Node* node)
//{
//	Node* p = node->next;
//	if (KongDui(node))
//	{
//		printf("空队\n");
//	}
//	else
//	{
//		while (p)
//		{
//			printf("%d ", p->data);
//			p = p->next;
//		}
//	}
//	
//	printf("\n");
//}
//
////出队
//void ChuDui(Node* node,int n)
//{
//	int i = 0;
//	if (!KongDui(node))
//	{
//		for (i = 0; i < n; i++)
//		{
//			Node* p = node->next;
//			node->next = node->next->next;
//			free(p);
//		}
//	}
//	else
//	{
//		printf("队已空\n");
//	}
//	
//}
//
//int main(void)
//{
//	Node* q = initQueue();
//
//	RuDui(q, 1);
//	RuDui(q, 2);
//	RuDui(q, 3);
//	RuDui(q, 4);
//	RuDui(q, 5);
//
//	PrintDui(q);
//
//	ChuDui(q, 2);
//	PrintDui(q);
//
//	return 0;
//}