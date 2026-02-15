//#define _CRT_SECURE_NO_WARNINGS 1
//
/////*二叉树的创建与遍历【失败案例】*/
////#include<stdio.h>
////#include<stdlib.h>
////
//////定义节点
////typedef struct Treenode {
////	int data;
////	struct Treenode* father;
////	struct Treenode* left;
////	struct Treenode* right;
////}Node;
////
//////创建并初始化节点
////Node* chuangjian()
////{
////	Node* a = (Node*)malloc(sizeof(Node));
////	a->data = 0;
////	a->father = NULL;
////	a->left = NULL;
////	a->right = NULL;
////
////	return a;
////}
////
//////节点赋值
////void fuzhi(Node* node, int* a, int i)
////{
////	if (node->left && i <= sizeof(a)/sizeof(int))
////	{
////		node->data = a[i];
////		i++;
////		fuzhi(node->left, a, i);
////
////		if (node->right && i <= sizeof(a) / sizeof(int))
////		{
////			node->data = a[i];
////			i++;
////			fuzhi(node->right, a, i);
////		}
////		else if(i <= sizeof(a) / sizeof(int))
////		{
////			//创建一个右节点
////			Node* r = chuangjian();
////			r->father = node;
////			fuzhi(node->right, a, i);
////		}
////	}
////	else if (i <= sizeof(a) / sizeof(int))
////	{
////		//创建一个左节点
////		Node* l = chuangjian();
////		l->father = node;
////		fuzhi(node->left, a, i);
////	}
////}
////
//////节点的遍历及每一条链的可视化输出
////void shuchu(Node* node)
////{
////	Node* p = node;
////	if (p == NULL)
////	{
////		return;
////	}
////	else
////	{
////		printf("%d \n", p->data);
////		shuchu(p->left);
////		shuchu(p->right);
////	}
////
////}
////
////int main(void)
////{
////	int i = 0; int n = 0;
////
////	Node* tree = chuangjian();
////	Node* p = tree;
////	scanf("%d", &n);
////	int* a = (int*)malloc(sizeof(int) * n);
////	int* b = a;
////	for (i = 0; i < n; i++)
////	{
////		scanf("%d", b);
////		b++;
////	}
////	b = a;
////
////	//fuzhi(p, a, 1);
////	//shuchu(tree);
////	
////	return 0;
////}
//
/////*固定深度的二叉树的创建与遍历【还是比较简单的，只要搞清楚递归的运作情况】*/
////#include<stdio.h>
////#include<stdlib.h>
////
////typedef struct node {
////	int data;
////	struct node* L;
////	struct node* R;
////	struct node* root;
////}Node;
////
////Node* chushihua()
////{
////	Node* a = new Node;
////	a->data = 0;
////	a->L = NULL;
////	a->R = NULL;
////	a->root = a;
////	return a;
////}
////
//////创建二叉树
////void creat(Node* root,int n)
////{
////	Node* p = root;
////
////	if (n == 0)
////	{
////		return;
////	}
////	else
////	{
////		int lnum = 0;
////		scanf("%d", &lnum);
////		Node* left = chushihua();
////		left->data = lnum;
////		left->root = root->root;
////		left->root->data++;
////		root->L = left;
////
////		creat(left, n - 1);
////
////		int rnum = 0;
////		scanf("%d", &rnum);
////		Node* right = chushihua();
////		right->data = rnum;
////		right->root = root->root;
////		right->root->data++;
////		root->R = right;
////
////		creat(right, n-1);
////	}
////}
////
//////*序遍历
////void print_tree(Node* root)
////{
////	if(root)
////	{
////		printf("%d ", root->data);
////		print_tree(root->L);
////		print_tree(root->R);
////	}
////}
////
////int main(void)
////{
////	Node* root = chushihua();
////	int n = 0;
////	printf("请输入二叉树的深度\n(根节点不包括在内，根节点用于存储除根节点以外的节点数量)\n");
////	scanf("%d", &n);
////	creat(root, n);
////	print_tree(root);
////	printf("\n\n共有%d个节点\n", root->data + 1);
////
////	return 0;
////}
//
/////*二叉树的递归创建、遍历方法*/
////#include<stdio.h>
////#include<stdlib.h>
////
//////定义树的节点
////typedef struct tree {
////	int data;
////	struct tree* L;
////	struct tree* R;
////}Node;
////
//////初始化节点
////Node* creat()
////{
////	Node* p = (Node*)malloc(sizeof(Node));
////	p->data = 0;
////	p->L = NULL;
////	p->R = NULL;
////
////	return p;
////}
////
//////树的递归创建(先序创建)
////void creattree(Node* T)
////{
////	int c = NULL;
////	scanf("%d", &c);
////	if (c == 0)
////	{
////		return;
////	}
////	else
////	{
////		T->data = c;
////		T->L = creat();
////		creattree(T->L);
////		T->R = creat();
////		creattree(T->R);
////	}
////}
////
//////树的递归遍历（后续遍历）
////void show(Node* p)
////{
////	if (p == NULL)
////	{
////		return;
////	}
////	else
////	{
////		show(p->L);
////		show(p->R);
////		if (p->data != 0)
////		{
////			printf("%d ", p->data);
////		}
////	}
////}
////
////int main(void)
////{
////	Node* p = creat();
////	creattree(p);
////	show(p);
////
////	return 0;
////}
//
/////*二叉树的层次遍历*/
////#include<stdio.h>
////#include<stdlib.h>
////
//////定义树的节点
////typedef struct tree {
////	char data;
////	struct tree* L;
////	struct tree* R;
////}TNode;
//////初始化树的节点
////TNode* initTNode()
////{
////	TNode* p = (TNode*)malloc(sizeof(TNode));
////	p->data = ' ';
////	p->L = NULL;
////	p->R = NULL;
////
////	return p;
////}
////
//////创建树(前序)
////void Tcreat(TNode** T)
////{
////	char a;
////	scanf(" %c", &a);
////	if (a == '*')
////	{
////		(*T) = NULL;
////		return;
////	}
////	else
////	{
////		(*T)->data = a;
////
////		(*T)->L = initTNode();
////		Tcreat(&((*T)->L));
////
////		(*T)->R = initTNode();
////		Tcreat(&((*T)->R));
////	}
////}
////
//////遍历树(前序遍历)
////void Tprint(TNode* T)
////{
////	if (T)
////	{
////		if (T->data)
////		{
////			printf("%c ", T->data);
////			Tprint(T->L);
////			Tprint(T->R);
////		}
////	}
////	else
////	{
////		return;
////	}
////}
////
//////定义队列节点
////typedef struct node {
////	int total;
////	TNode* data;
////	struct node* pre;
////	struct node* next;
////}Node;
////
//////初始化节点
////Node* initNode()
////{
////	Node* p = (Node*)malloc(sizeof(Node));
////	p->total = 0;
////	p->data = NULL;
////	p->next = p;
////	p->pre = p;
////
////	return p;
////}
////
//////创建队列，尾插法
////void creatNode(Node** node,TNode* data)
////{
////	if (data)
////	{
////		if ((*node)->total == 0)
////		{
////			Node* nw = initNode();
////			nw->data = data;
////			nw->pre = (*node);
////			nw->next = (*node);
////			(*node)->pre = nw;
////			(*node)->next = nw;
////			(*node)->total++;
////
////		}
////		else
////		{
////			Node* cur = initNode();
////			cur->data = data;
////			cur->next = (*node);
////			cur->pre = (*node)->pre;
////			(*node)->pre->next = cur;
////			(*node)->pre = cur;
////			(*node)->total++;
////		}
////	}
////	else
////	{
////		return;
////	}
////}
////
//////弹出队首元素
////TNode* pop(Node* node)
////{
////	Node* now = node->next;
////	TNode* p = now->data;
////	now->next->pre = now->pre;
////	now->pre->next = now->next;
////	node->total--;
////	free(now);
////
////	return p;
////}
////
//////层次遍历树【逻辑有误】
////void cen_ci(TNode* T,Node** node)
////{
////	if (T)
////	{
////		if ((*node)->total == 0)
////		{
////			creatNode(node, T);//放进去
////			printf("%c ", pop(*node)->data);//弹出来
////			creatNode(node, T->L);//放左边
////			creatNode(node, T->R);//放右边
////		}
////		else
////		{
////			printf("%c ", pop(*node)->data);//弹出来
////			creatNode(node, T->L);//放左边
////			creatNode(node, T->R);//放右边
////		}
////		cen_ci(T->L, node);
////		cen_ci(T->R, node);
////	}
////	else
////	{
////		return;
////	}
////}
////
//////层次遍历树2.0
////void cen(Node** node)
////{
////	if ((*node)->total > 0)
////	{
////		TNode* p = pop(*node);
////		creatNode(node, p->L);
////		creatNode(node, p->R);
////		printf("%c ", p->data);
////		cen(node);
////	}
////	else
////	{
////		return;
////	}
////}
////
////int main(void)
////{
////	TNode* T = initTNode();
////	Tcreat(&T);
////
////	printf("\n正序遍历：\n");
////	Tprint(T);
////
////	printf("\n层次遍历：\n");
////	Node* node = initNode();
////	creatNode(&node, T);
////	cen(&node);
////
////	return 0;
////}
//
//
///*二叉树的非递归创建、遍历方法*/
//#include<stdio.h>
//#include<stdlib.h>
//
///*创建二叉树*/
//typedef struct t {
//	char data;
//	int was;
//	struct t* L;
//	struct t* R;
//}TNode;
//
//TNode* initTNode()
//{
//	TNode* p = (TNode*)malloc(sizeof(TNode));
//	p->data = ' ';
//	p->was = 0;
//	p->L = NULL;
//	p->R = NULL;
//
//	return p;
//}
//
//void Tcreat(TNode* T)
//{
//	char a;
//	scanf("%c", &a);
//	if (a == '*')
//	{
//		T->was = 1;
//		T = NULL;
//		free(T);
//		return;
//	}
//	else
//	{
//		T->data = a;
//		T->L = initTNode();
//		Tcreat(T->L);
//		T->R = initTNode();
//		Tcreat(T->R);
//	}
//}
//
//void Tprint(TNode* T)
//{
//	if (T)
//	{
//		if (T->data != ' ')
//		{
//			printf("%c ", T->data);
//			Tprint(T->L);
//			Tprint(T->R);
//		}
//	}
//}
//
///*前序遍历和中序遍历*/
////1.根节点入栈
////2.循环，直到左支为空
////3.出栈，访问节点，入栈右支
//
////创建栈
//typedef struct node {
//	int total;
//	TNode* data;
//	struct node* next;
//}Node;
//
//Node* initNode()
//{
//	Node* p = (Node*)malloc(sizeof(Node));
//	p->total = 0;
//	p->data = NULL;
//	p->next = NULL;
//
//	return p;
//}
//
////头插法
//void push(Node* node, TNode* data)
//{
//	if (data != NULL && data->data != ' ')
//	{
//		Node* x = initNode();
//		x->data = data;
//		x->next = node->next;
//		node->next = x;
//		node->total++;
//	}
//	else
//	{
//		return;
//	}
//}
//
////弹出栈顶
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
//TNode* ppp(Node* node)
//
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
////清除访问次数
//void clear(TNode* T)
//{
//	if (T->data != ' ')
//	{
//		T->was = 0;//清除访问次数
//		clear(T->L);
//		clear(T->R);
//	}
//}
//
////判断节点有无右支
//int haveR(TNode* p)
//{
//	if (p->R->was ==  0)
//	{
//		return 1;
//	}
//	else
//	{
//		return 0;
//	}
//}
//
////根节点入栈
//void pushroot(Node* node, TNode* root, int mode)
//{
//	while (root)
//	{
//		push(node, root);
//		if (mode == 1 && root->data != ' ')
//		{
//			printf("%c ", root->data);
//			root->was++;//访问次数+1
//		}
//		root = root->L;
//	}
//}
//
///*一、先序遍历（迭代方式）*/
//void xianxu(TNode* T)
//{
//	printf("\n先序遍历：\n");
//
//	//左根入栈并访问
//	Node* node = initNode();
//	TNode* cur = T;
//	pushroot(node, cur, 1);
//
//	TNode* now = ppp(node);
//	while (node->total > 0)
//	{
//		//有右支先访问右支
//		if (haveR(now))
//		{
//			pushroot(node, now->R, 1);
//		}
//		else//没有右支则弹出
//		{
//			pop(node);
//		}
//		now = ppp(node);//复制栈首元素
//	}
//
//	//清除访问次数
//	clear(T);
//}
//
///*二、中序遍历（迭代方式）*/
//void zhongxu(TNode* T)
//{
//	printf("\n中序遍历：\n");
//
//	Node* node = initNode();
//	//根节点入栈
//	TNode* cur = T;
//	pushroot(node, cur, 2);
//
//	while (node->total > 0)
//	{
//		//弹出栈顶并访问
//		TNode* now = pop(node);
//		if (now->data != ' ')
//		{
//			printf("%c ", now->data);
//			now->was++;
//		}
//
//		//判断栈顶弹出元素是否含有右支
//		if (haveR(now))
//		{
//			pushroot(node, now->R, 2);//将弹出元素的右节点作为根节点入栈
//		}
//	}
//
//	//清除访问次数
//	clear(T);
//}
//
///*三、后序遍历（迭代方式）*/
////1.出栈的时候被访问
//void houxu(TNode* T)
//{
//	printf("\n后序遍历：\n");
//
//	Node* node = initNode();
//	TNode* cur = T;
//	//左根节点入栈
//	pushroot(node, cur, 2);
//	TNode* now = ppp(node);
//	
//	while (node->total > 0)
//	{
//		//出栈前判断是否有右支
//		if (haveR(now))//有右支先右支入栈，再pushroot
//		{
//			pushroot(node, now->R, 2);
//			now = ppp(node);
//		}
//		//无右支则直接出栈（出栈时访问）
//		else
//		{
//			now = pop(node);//弹出
//			printf("%c ", now->data);
//			now->was++;
//		}
//		now = ppp(node);
//	}
//
//	//清除访问次数
//	clear(T);
//}
//
//int main(void)
//{
//	TNode* T = initTNode();
//	Tcreat(T);
//	printf("递归遍历：\n");
//	Tprint(T);
//	printf("\n\n迭代遍历：\n");
//
//	/*一、先序遍历（迭代方式）*/
//	xianxu(T);
//	/*二、中序遍历（迭代方式）*/
//	zhongxu(T);
//	/*三、后序遍历（迭代方式）*/
//	houxu(T);
//
//	return 0;
//}
//
///*
//总结：
//一、层次遍历：
//1.先将根节点入栈，在栈顶弹出之后紧接着将该顶的左右节点用尾插法插入队列。
//
//二、非递归方法遍历二叉树
//1.前序遍历：先让左边的根节点入栈，【在入栈的同时访问节点】，当栈顶元素没有右子树时，弹出栈顶；
//如果它有右子树，先将右子树插入栈中（用先插入左根节点的方式），当栈顶元素没有右子树时，弹出栈顶。
//
//2.中序遍历：先让左边的根节点入栈，当栈顶元素没有右子树时，【弹出栈顶并访问弹出节点】，如果后一栈顶节点有右子树，
//则【先弹出栈顶，再将右子树插入栈中】（用先插入左根节点的方式）。
//
//3.后序遍历：先让左边的根节点入栈，当栈顶元素没有右子树时，【弹出栈顶并访问该节点】；
//当它有右子树时，先插入右子树（用先插入左根节点的方式），当它没有右子树时，弹出栈顶并访问该节点。
//*/