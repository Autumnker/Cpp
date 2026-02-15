#define _CRT_SECURE_NO_WARNINGS 1

///*字符金字塔*/
//#include<stdio.h>
//
//int main(void)
//{
//	int n = 0;
//	scanf("%d", &n);
//
//	char a = 'A';
//	int max = 0;
//	int kongge = 0;
//
//	int i = 0, j = 0;
//	for (i = 1; i <= n; i++)
//	{
//		max = n + i - 1;
//		kongge = n - i;
//		for (j = 0; j < max; j++)
//		{
//			if (j < kongge)
//			{
//				printf(" ");
//			}
//			else
//			{
//				printf("%c", a);
//			}
//		}
//		printf("\n");
//		a = a + 1;
//	}
//
//	return 0;
//}

///*十六进制数转八进制数*/
//#include<stdio.h>
//#include<stdlib.h>
//#include<string.h>
//#include<math.h>
//
//////创建二进制数的节点
////typedef struct er {
////	int data;
////	struct er* next;
////}Node;
//
//////初始化节点
////Node* chushihua()
////{
////	Node* a;
////	a->data = 0;
////	a->next = NULL;
////	return a;
////}
//
//////创建链表
////Node* jiedian(Node* node, int num)
////{
////	Node* p = chushihua();
////	p->data = num;
////	p->next = node->next;
////	node->next = p;
////	return p;
////}
//
//////将十六进制转换为二进制
////void shiliu_er(char* shiliu,Node* er)
////{
////	switch (*shiliu)
////	{
////	case '0':
////		jiedian(jiedian(jiedian(jiedian(er, 0), 0), 0), 0);
////		return;
////	case '1':
////		jiedian(jiedian(jiedian(jiedian(er, 0), 0), 0), 1);
////		return;
////	case '2':
////		jiedian(jiedian(jiedian(jiedian(er, 0), 0), 1), 0);
////		return;
////	case '3':
////		jiedian(jiedian(jiedian(jiedian(er, 0), 0), 1), 1);
////		return;
////	case '4':
////		jiedian(jiedian(jiedian(jiedian(er, 0), 1), 0), 0);
////		return;
////	case '5':
////		jiedian(jiedian(jiedian(jiedian(er, 0), 1), 0), 1);
////		return;
////	case '6':
////		jiedian(jiedian(jiedian(jiedian(er, 0), 1), 1), 0);
////		return;
////	case '7':
////		jiedian(jiedian(jiedian(jiedian(er, 0), 1), 1), 1);
////		return;
////	case '8':
////		jiedian(jiedian(jiedian(jiedian(er, 1), 0), 0), 0);
////		return;
////	case '9':
////		jiedian(jiedian(jiedian(jiedian(er, 1), 0), 0), 1);
////		return;
////	case 'A':
////		jiedian(jiedian(jiedian(jiedian(er, 1), 0), 1), 0);
////		return;
////	case 'B':
////		jiedian(jiedian(jiedian(jiedian(er, 1), 0), 1), 1);
////		return;
////	case 'C':
////		jiedian(jiedian(jiedian(jiedian(er, 1), 1), 0), 0);
////		return;
////	case 'D':
////		jiedian(jiedian(jiedian(jiedian(er, 1), 1), 0), 1);
////		return;
////	case 'E':
////		jiedian(jiedian(jiedian(jiedian(er, 1), 1), 1), 0);
////		return;
////	case 'F':
////		jiedian(jiedian(jiedian(jiedian(er, 1), 1), 1), 1);
////		return;
////	}
////}
//
////十六进制到二进制的映射
//void shiliu_to_er(char* a, int* b)
//{
//	int i = 0, j = 0;
//	for (i = 0; i < 100001; i++)
//	{
//		if (a[i] != '\0')
//		{
//			j = 4 * i;
//			switch (a[i])
//			{
//			case '0':
//				b[j] = 0;
//				b[j + 1] = 0;
//				b[j + 2] = 0;
//				b[j + 3] = 0;
//				break;
//			case '1':
//				b[j] = 0;
//				b[j + 1] = 0;
//				b[j + 2] = 0;
//				b[j + 3] = 1;
//				break;
//			case '2':
//				b[j] = 0;
//				b[j + 1] = 0;
//				b[j + 2] = 1;
//				b[j + 3] = 0;
//				break;
//			case '3':
//				b[j] = 0;
//				b[j + 1] = 0;
//				b[j + 2] = 1;
//				b[j + 3] = 1;
//				break;
//			case '4':
//				b[j] = 0;
//				b[j + 1] = 1;
//				b[j + 2] = 0;
//				b[j + 3] = 0;
//				break;
//			case '5':
//				b[j] = 0;
//				b[j + 1] = 1;
//				b[j + 2] = 0;
//				b[j + 3] = 1;
//				break;
//			case '6':
//				b[j] = 0;
//				b[j + 1] = 1;
//				b[j + 2] = 1;
//				b[j + 3] = 0;
//				break;
//			case '7':
//				b[j] = 0;
//				b[j + 1] = 1;
//				b[j + 2] = 1;
//				b[j + 3] = 1;
//				break;
//			case '8':
//				b[j] = 1;
//				b[j + 1] = 0;
//				b[j + 2] = 0;
//				b[j + 3] = 0;
//				break;
//			case '9':
//				b[j] = 1;
//				b[j + 1] = 0;
//				b[j + 2] = 0;
//				b[j + 3] = 1;
//				break;
//			case 'A':
//				b[j] = 1;
//				b[j + 1] = 0;
//				b[j + 2] = 1;
//				b[j + 3] = 0;
//				break;
//			case 'B':
//				b[j] = 1;
//				b[j + 1] = 0;
//				b[j + 2] = 1;
//				b[j + 3] = 1;
//				break;
//			case 'C':
//				b[j] = 1;
//				b[j + 1] = 1;
//				b[j + 2] = 0;
//				b[j + 3] = 0;
//				break;
//			case 'D':
//				b[j] = 1;
//				b[j + 1] = 1;
//				b[j + 2] = 0;
//				b[j + 3] = 1;
//				break;
//			case 'E':
//				b[j] = 1;
//				b[j + 1] = 1;
//				b[j + 2] = 1;
//				b[j + 3] = 0;
//				break;
//			case 'F':
//				b[j] = 1;
//				b[j + 1] = 1;
//				b[j + 2] = 1;
//				b[j + 3] = 1;
//				break;
//
//			default:
//				return;
//			}
//		}
//		else
//		{
//			return;
//		}
//	}
//}
//
////将二进制转换为八进制
//void er_ba(int* er, int* ba)
//{
//	int i = 0, j = 0;
//	int n = 0, total = 0;
//	for (i = 0; i < 400004; i++)
//	{
//		if (er[i] == 2)
//		{
//			n = i - 1;
//			break;
//		}
//	}
//
//	//if (n % 3 == 0)
//	//{
//	//	m = n / 3;
//	//}
//	//else
//	//{
//	//	m = n / 3 + 1;
//	//}
//
//	int m = 0;
//	int times = 0;
//
//	for (i = n; i >= 0; i--)
//	{
//		total += er[i] * pow(2, (times % 3));
//		times++;
//		if (times % 3 == 0)
//		{
//			ba[m] = total;
//			total = 0;
//			m++;
//		}
//		else if (times == n + 1)
//		{
//			ba[m] = total;
//			total = 0;
//			m++;
//		}
//	}
//}
//
////逆序输出八进制数
//void print(int* ba)
//{
//	int i = 0;
//	int n = 0;
//	for (i = 0; i < 300003; i++)
//	{
//		if (ba[i] == 8)
//		{
//			n = i - 1;
//			break;
//		}
//	}
//
//	int flag = 0;
//
//	for (i = n; i >= 0; i--)
//	{
//		if (ba[i] != 0)
//		{
//			flag = 1;
//		}
//		if (flag == 1)
//		{
//			printf("%d", ba[i]);
//		}
//	}
//
//	printf("\n");
//}
//
////输出二进制数
//void print_er(int* er)
//{
//	int i = 0;
//	for (i = 0; i < 400004; i++)
//	{
//		if (er[i] == 2)
//		{
//			break;
//		}
//		else
//		{
//			printf("%d", er[i]);
//		}
//	}
//	printf("\n");
//}
//
////输出十六进制数
//void print_shiliu(char* shiliu)
//{
//	int i = 0;
//	for (i = 0; i < 100001; i++)
//	{
//		if (shiliu[i] == '\0')
//		{
//			break;
//		}
//		else
//		{
//			printf("%c", shiliu[i]);
//		}
//	}
//	printf("\n");
//}
//
//int main(void)
//{
//	FILE* file = fopen("res/input1.txt", "r");
//	if (file == NULL)
//	{
//		printf("打开文件失败");
//		return 1;
//	}
//
//	int i = 0, j = 0;
//	int n = 0;
//	//获取十六进制数的个数
//	scanf("%d", &n);
//
//	char** shiliu = (char**)malloc(sizeof(char*) * n);
//	int** er = (int**)malloc(sizeof(int*) * n);
//	int** ba = (int**)malloc(sizeof(int*) * n);
//	
//	for (i = 0; i < n; i++)
//	{
//		shiliu[i] = (char*)malloc(sizeof(char) * 100001);
//		er[i] = (int*)malloc(sizeof(int) * 400004);
//		ba[i] = (int*)malloc(sizeof(int) * 300003);
//	}
//
//	//初始化
//	for (i = 0; i < n; i++)
//	{
//
//		for (j = 0; j < 400004; j++)
//		{
//			if (j < 300003)
//			{
//				if (j < 100001)
//				{
//					shiliu[i][j] = '\0';
//				}
//				ba[i][j] = 8;
//			}
//			er[i][j] = 2;
//		}
//	}
//
//	for (i = 0; i < n; i++)
//	{
//		scanf("%s", shiliu[i]);
//		//fscanf(file, "%s", shiliu[i]);
//	}
//
//	////记得保存二进制的头节点和尾节点
//
//	////十六转二进制
//	for (i = 0; i < n; i++)
//	{
//		shiliu_to_er(shiliu[i], er[i]);
//	}
//	//print_er(er[0]);
//
//	////二转八进制
//	for (i = 0; i < n; i++)
//	{
//		er_ba(er[i], ba[i]);
//	}
//
//	////输出八进制
//	for (i = 0; i < n; i++)
//	{
//		print(ba[i]);
//		//print_er(er[i]);
//	}
//
//	//for (i = 0; i < 10; i++)
//	//{
//	//	printf("%d", ba[0][i]);
//	//}
//
//	for (i = 0; i < n; i++)
//	{
//		free(shiliu[i]);
//		free(er[i]);
//		free(ba[i]);
//	}
//	free(shiliu);
//	free(er);
//	free(ba);
//
//	fclose(file);
//
//	return 0;
//}
