#define _CRT_SECURE_NO_WARNINGS 1

///*指针数组与指向指针的指针的关系*/
//#include<stdio.h>
//#include<string.h>
//int main(void)
//{
//	//str[]数组中储存的是字符串首地址，str是指向首地址的指针,所以它是指向指针的指针
//	const char* str[] = {
//	"hello ",
//	"my name ",
//	"is lihua ",
//	};
//
//	const char** p = str;
//
//	for (int i = 0; i < 3; i++)
//	{
//		printf("%s", *(p + i));
//	}
//
//	return 0;
//}

///*指针函数和函数指针*/
//#include<stdio.h>
////指针函数（返回值是指针的函数）
//int* fun1(int a)
//{
//	a = 10;
//	return &a;
//}
////函数指针（指向函数入口（首地址）的指针）
//int fun2(int b)
//{
//	return b;
//}
//
//int main(void)
//{
//	int a = 0, b = 1;
//	int *p1 = fun1(a);
//	int (*p2)(int) = NULL;//注意，(*p2)(这里面的参数列表与所指函数的参数列表要一致)
//	p2 = fun2;
//	p2(b);
//
//	printf("p1 = %p,*p1 = %d\n",p1,*p1);
//	printf("p2 = %p,*p2 = %d\n", p2(b), p2(b));
//
//	return 0;
//}