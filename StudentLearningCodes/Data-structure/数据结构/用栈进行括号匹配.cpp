//#define _CRT_SECURE_NO_WARNINGS 1
//
///*用链栈进行括号匹配*/
//#include<iostream>
//#include<cstdlib>
//
////定义链栈节点
//typedef struct node {
//	int total;
//	char data;
//	struct node* next;
//}Node;
//
////初始化节点
//Node* initnode()
//{
//	Node* a = (Node*)malloc(sizeof(Node));
//	a->total = 0;
//	a->data = NULL;
//	a->next = NULL;
//
//	return a;
//}
//
////创建链栈（头插法，负责左括号）
//void creatL(Node* node,char data)
//{
//	Node* p = node;
//	Node* str = initnode();
//	str->data = data;
//	str->next = p->next;
//	p->next = str;
//	p->total++;
//}
////创建链栈（尾插法，负责右括号）
//void creatR(Node* node, char data)
//{
//	Node* p = initnode();
//	Node* cur = node;
//	int n = cur->total;
//	//将cur指向栈尾
//	while (n > 0)
//	{
//		cur = cur->next;
//		n--;
//	}
//	p->data = data;
//	p->next = cur->next;
//	cur->next = p;
//	node->total++;
//}
//
////判断栈空
//int isEmpty(Node* node)
//{
//	if (node->total == 0 || node->next == NULL)
//	{
//		return 1;
//	}
//	else
//		return 0;
//}
//
////打印链表元素
//void show(Node* node)
//{
//	int n = node->total;
//	Node* p = node->next;
//	while (n > 0)
//	{
//		printf("%c ", p->data);
//		p = p->next;
//		n--;
//	}
//}
//
////弹出栈顶元素
//void pop(Node* a)
//{
//	char data = NULL;
//
//	if (isEmpty(a))
//	{
//		return;
//	}
//	else
//	{
//		Node* p = a->next;
//		data = p->data;
//		a->next = p->next;
//		free(p);
//		a->total--;
//
//		return;
//	}
//}
//
////抛出栈顶元素（元素不出栈）
//char top(Node* a)
//{
//	if (isEmpty(a))
//	{
//		return NULL;
//	}
//	else
//	{
//		char data = a->next->data;
//		return data;
//	}
//}
//
//int main(void)
//{
//	//创建两个链表，分别存储左括号和右括号
//	Node* left = initnode();
//	Node* right = initnode();
//	Node* all = initnode();
//
//	//向两个链表中存入括号
//	char in = 'a';
//	int flag = 1;
//	int num = 0;
//
//	printf("\n请输入括号，“()”“[]”“{}”皆可，于字母“o”结束输入\n");
//	while (flag)
//	{
//		scanf("%c", &in);
//		if (in == 'o')
//		{
//			flag = 0;
//		}
//
//		if (flag)
//		{
//			if (in == '(' || in == '[' || in == '{')
//			{
//				creatL(left, in);
//			}
//			else if (in == ')' || in == ']' || in == '}')
//			{
//				creatR(right, in);//用于表示右括号的排列方式
//			}
//			creatR(all, in);
//
//			////打印链表元素
//			//printf("\n这是左括号，共有 %d 个元素\n", left->total);
//			//show(left);
//			//printf("\n这是右括号，共有 %d 个元素\n", left->total);
//			//show(right);
//			//printf("\n这是所有括号，共有 %d 个元素\n", all->total);
//			//show(all);
//			//printf("\n");
//
//			//遍历所有括号，遇到右括号时，弹出左括号栈顶元素并与之比对
//			char left_top = top(left);
//			char all_top = top(all);
//			while (all->total)
//			{
//				if (!isEmpty(left) && !isEmpty(all))
//				{
//					if (all_top == ')' && left_top != '(' ||
//						all_top == ']' && left_top != '[' ||
//						all_top == '}' && left_top != '{')
//					{
//						num = 1;//printf("\n匹配失败，括号类型不匹配\n");
//						break;
//					}
//					else if (all_top == '(' || all_top == '[' || all_top == '{')
//					{
//						all_top = top(all);
//						pop(all);//弹出左括号
//					}
//					else if (all_top == ')' && left_top == '(' ||
//						all_top == ']' && left_top == '[' ||
//						all_top == '}' && left_top == '{')
//					{
//						all_top = top(all);
//						pop(all);
//						left_top = top(left);
//						pop(left);
//					}
//				}
//				else if (isEmpty(left) && !isEmpty(all))
//				{
//					num = 2; //printf("\n匹配失败，括号未成对出现(缺少左括号)\n");
//					break;
//				}
//			}
//
//			if (isEmpty(left) && isEmpty(all))
//			{
//				num = 4; //printf("\n匹配成功\n");
//			}
//			else if (!isEmpty(left) && isEmpty(all))
//			{
//				num = 3; //printf("\n匹配失败，括号未成对出现(缺少右括号)\n");
//			}
//		}
//	}
//
//	switch(num)
//	{
//	case 1:
//		printf("\n括号类型不匹配\n");
//		break;
//	case 2:
//		printf("\n缺少左括号\n");
//		break;
//	case 3:
//		printf("\n缺少右括号\n");
//		break;
//	case 4:
//		printf("\n匹配成功\n");
//		break;
//	}
//
//	return 0;
//}