//#define _CRT_SECURE_NO_WARNINGS 1
//
///*无向图DFS\BFS*/
//#include<stdio.h>
//#include<stdlib.h>
//
////创建节点
//typedef struct node {
//	int data;
//	struct node* next;
//	struct node* parent;
//}Node;
//
////循环链表
//Node* initNode()
//{
//	Node* p = (Node*)malloc(sizeof(Node));
//
//	p->next = p;
//	p->parent = p;
//	p->data = 0;
//
//	return p;
//}
//
////尾插法
//void push(Node* node, int data)
//{
//	Node* p = initNode();
//	p->parent = node->parent;
//	node->parent->next = p;
//	p->next = node;
//	node->parent = p;
//	if (node->next == NULL)
//	{
//		node->next = p;
//	}
//	p->data = data;
//
//	node->data++;
//}
//
////头插法
//void in(Node* node, int data)
//{
//	Node* p = initNode();
//	p->data = data;
//	p->next = node->next;
//	p->parent = node;
//	node->next = p;
//	p->next->parent = p;
//
//	node->data++;
//}
//
////出队(栈)
//int pop(Node* node)
//{
//	if (node->data > 0)
//	{
//		Node* p = node->next;
//		int num = p->data;
//		node->next = p->next;
//		p->next->parent = node;
//		free(p);
//		node->data--;
//
//		return num;
//	}
//	else
//		return -1;
//}
//
////定义图
//typedef struct graph {
//	char* vexs;//节点的一维数组表示
//	int** arcs;//边的二维数组表示
//	int vexNum;//节点的数量
//	int arcNum;//边的总数量
//}Graph;
//
////初始化图
//Graph* initGraph(int vexNum, char* a)
//{
//	Graph* G = (Graph*)malloc(sizeof(Graph));
//	G->vexs = (char*)malloc(sizeof(char) * vexNum);
//	G->arcs = (int**)malloc(sizeof(int*) * vexNum);
//	int i = 0;
//	for (i = 0; i < vexNum; i++)
//	{
//		G->vexs[i] = a[i];//节点传入
//		int j = 0;
//		for (j = 0; j < vexNum; j++)
//		{
//			G->arcs[i] = (int*)malloc(sizeof(int) * vexNum);
//			G->arcs[i][j] = 0;//初始化边的数据
//		}
//	}
//	G->vexNum = vexNum;
//	G->arcNum = 0;
//
//	return G;
//}
//
////给图赋值(连线)
//void CreatGraph(Graph* G, int* arcs)
//{
//	int i = 0;
//	for (i = 0; i < G->vexNum; i++)
//	{
//		int j = 0;
//		for (j = 0; j < G->vexNum; j++)
//		{
//			G->arcs[i][j] = arcs[i * G->vexNum + j];
//			if (G->arcs[i][j] == 1);
//			{
//				G->arcNum++;
//			}
//		}
//	}
//
//	G->arcNum = G->arcNum / 2;
//}
//
////遍历图DFS
//void DFS(Graph* G,int topNum)//topNum是指起始位置的编号
//{
//	printf("DFS遍历: ");
//
//	int i = 0, j = 0,totalvisited = 0;
//	//记录节点是否被访问的数据结构
//	int* havedone = (int*)malloc(sizeof(int) * G->vexNum);
//	for (i = 0; i < G->vexNum; i++)
//	{
//		havedone[i] = 0;
//	}
//
//	Node* num = initNode();
//
//	//先访问第一个节点
//	printf("%c ", G->vexs[topNum]);
//	havedone[topNum] = 1;
//	totalvisited++;
//
//	for (i = topNum; totalvisited < G->vexNum ; i++)
//	{
//		//printf("\n%d\n",i);
//
//		if (i >= G->vexNum)
//		{
//			i = 0;
//		}
//		for (j = 0; j < G->vexNum; j++)
//		{
//			if (G->arcs[i][j] == 1 && havedone[j] != 1)
//			{
//				printf("%c ", G->vexs[j]);
//				havedone[j] = 1;
//				totalvisited++;
//				in(num, i);
//				i = j - 1;//因为在一次循环的末尾，i会自增，所以要比 j 小一
//
//				break;
//			}
//			if (j == G->vexNum - 1 && G->arcs[i][j] == 0)//一个路径到底，就回到上次分叉的地方
//			{
//				i = pop(num) - 1;//回到上一层，直到有分岔路口为止
//			}
//		}
//	}
//
//	free(havedone);
//}
//
////遍历图BFS
//void BFS(Graph* G, int topNum)
//{
//	printf("BFS遍历: ");
//
//	Node* num = initNode();
//
//	int* havedone = (int*)malloc(sizeof(int) * G->vexNum);
//	int i = 0, j = 0, totalvisited = 0;
//	for (i = 0; i < G->vexNum; i++)
//	{
//		havedone[i] = 0;
//	}
//
//	//主要逻辑(类似于二叉树的层次遍历)
//	printf("%c ", G->vexs[topNum]);
//	havedone[topNum] = 1;
//	totalvisited++;
//
//	for (i = topNum; totalvisited < G->vexNum; i++)
//	{
//		for (j = 0; j < G->vexNum; j++)
//		{
//			if (G->arcs[i][j] == 1 && havedone[j] != 1)
//			{
//				printf("%c ", G->vexs[j]);
//				havedone[j] = 1;
//				totalvisited++;
//
//				push(num, j);
//			}
//		}
//		if (num->data > 0)
//		{
//			i = pop(num) - 1;
//		}
//	}
//	//主要逻辑结束
//
//	free(havedone);
//}
//
//int main(void)
//{
//	int vexNum = 10;
//	char a[] = "ABCDEFGHIJ";
//	int arcs[] = {
//		0,1,1,0,1,0,1,1,0,0,
//		1,0,0,1,0,0,0,0,0,0,
//		1,0,0,0,0,1,0,0,0,1,
//		0,1,0,0,1,0,0,0,1,0,
//		1,0,0,1,0,0,0,0,0,1,
//		0,0,1,0,0,0,1,0,0,0,
//		1,0,0,0,0,1,0,0,0,0,
//		1,0,0,0,0,0,0,0,0,0,
//		0,0,0,1,0,0,0,0,0,0,
//		0,0,1,0,1,0,0,0,0,0
//	};
//
//	int topNum = 0;
//
//	Graph* G = initGraph(vexNum, a);
//	CreatGraph(G, arcs);
//	DFS(G, topNum);
//	printf("\n");
//	BFS(G, topNum);
//
//	return 0;
//}