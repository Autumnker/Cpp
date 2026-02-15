//#define _CRT_SECURE_NO_WARNINGS 1
//
///*中序线索二叉树*/
///*
//1.创建二叉树
//2.中序遍历二叉树，并顺便让空指针指向前驱和后继
//*/
//
//#include<stdio.h>
//#include<stdlib.h>
////定义树的节点
//typedef struct Tree {
//	char data;
//	struct Tree* L;
//	struct Tree* R;
//	int ltag;//tag为0时为指向子节点的指针，为1时为指向前驱和后继的指针
//	int rtag;
//}TNode;
//
////初始化节点
//TNode* initTNode()
//{
//	TNode* p = (TNode*)malloc(sizeof(TNode));
//	p->data = ' ';
//	p->L = NULL;
//	p->R = NULL;
//	p->ltag = 0;
//	p->rtag = 0;
//
//	return p;
//}
//
////创建栈节点
//typedef struct node {
//	int total;
//	TNode* data;
//	struct node* next;
//}Node;
////初始化栈节点
//Node* initNode()
//{
//	Node* p = (Node*)malloc(sizeof(Node));
//	p->data = NULL;
//	p->next = NULL;
//	p->total = 0;
//
//	return p;
//}
//
////元素入栈
//void creat(Node* node, TNode* T)
//{
//	if (T)
//	{
//		Node* p = (Node*)malloc(sizeof(Node));
//		p->data = T;
//		p->next = node->next;
//		node->next = p;
//		node->total++;
//	}
//}
//
////弹出栈顶元素
//TNode* pop(Node* node)
//{
//	if (node->total > 0)
//	{
//		Node* top = node->next;
//		TNode* p = top->data;
//		node->next = top->next;
//		free(top);
//		node->total--;
//
//		return p;
//	}
//	else
//	{
//		return NULL;
//	}
//}
//
////复制栈顶元素
//TNode* copy(Node* node)
//{
//	if (node->total > 0)
//	{
//		Node* top = node->next;
//		TNode* p = top->data;
//
//		return p;
//	}
//	else
//	{
//		return NULL;
//	}
//}
//
////将左根压入栈中
//void stack_in(Node* node, TNode* T)
//{
//	if (T)
//	{
//		creat(node, T);
//		stack_in(node, T->L);
//	}
//	else
//	{
//		return;
//	}
//}
//
////创建二叉树(先序)
//void Tcreat(TNode* T)
//{
//	char a = ' ';
//	scanf("%c", &a);
//	if (a == '*')
//	{
//		T = NULL;
//		free(T);
//		return;
//	}
//	else
//	{
//		T->data = a;
//
//		TNode* left = initTNode();
//		T->L = left;
//		Tcreat(left);
//
//		TNode* right = initTNode();
//		T->R = right;
//		Tcreat(right);
//	}
//}
//
////二叉树的递归遍历(中序)
//void show(TNode* T)
//{
//	if (T && T->data != ' ')
//	{
//		show(T->L);
//		printf("%c ", T->data);
//		show(T->R);
//	}
//	else
//	{
//		return;
//	}
//}
//
////非递归遍历，中序
//void view(Node* node, TNode* T)
//{
//	stack_in(node, T);
//
//	TNode* now = copy(node);
//
//	while (node->total > 0)
//	{
//		if (now->R == NULL)
//		{
//			//弹出时才访问
//			now = pop(node);
//			if (now->data != ' ')
//			{
//				printf("%c ", now->data);
//			}
//		}
//		else
//		{
//			//先弹出再入右节点
//			pop(node);
//			if (now->data != ' ')
//			{
//				printf("%c ", now->data);
//			}
//			stack_in(node, now->R);
//		}
//		now = copy(node);
//	}
//}
//
////线索化二叉树
//void xian_suo(TNode* T, TNode* pre)
//{
//
//}
//
//int main(void)
//{
//	TNode* T = initTNode();
//	Node* node = initNode();
//
//	Tcreat(T);
//	/*printf("\n二叉树的递归遍历：\n");
//	show(T);
//	printf("\n二叉树的非递归遍历：\n");
//	view(node,T);*/
//
//	TNode* pre = T;
//
//	return 0;
//}