//#define _CRT_SECURE_NO_WARNINGS 1
//
///*在线性表中使用二分查找*/
//#include<iostream>
//using namespace std;
//
////定义线性表
//struct List {
//	int* a;
//	int length;
//	int num;
//};
//
////线性表的初始化函数
//List* initList(int length)
//{
//	List* list = new List;
//	list->length = length;
//	list->num = 0;
//	list->a = new int[length];
//
//	return list;
//}
//
////线性表赋值函数
//void fu_zhi(List* list, int data)
//{
//	if (list->num == list->length)
//	{
//		cout << "\n线性表已满\n" << endl;
//		return;
//	}
//	else
//	{
//		list->a[list->num] = data;
//		list->num = list->num + 1;
//	}
//}
//
////线性表打印函数
//void PrintList(List* list)
//{
//	if (list->num <= 0)
//	{
//		cout << "\n线性表已空\n" << endl;
//		return;
//	}
//	else
//	{
//		int i = 0;
//		for (i = 0; i < list->num; i++)
//		{
//			cout << list->a[i] << " ";
//		}
//	}
//}
//
////线性表元素排序(从小到大)
//void pai_xu(List* list)
//{
//	int i = 0, j = 0;
//	for (i = 0; i < list->length; i++)
//	{
//		for (j = i; j < list->length; j++)
//		{
//			if (list->a[i] > list->a[j])
//			{
//				int temp = list->a[i];
//				list->a[i] = list->a[j];
//				list->a[j] = temp;
//			}
//		}
//	}
//}
//
////二分查找，找下标(迭代实现)
//int bin_serach(List* list, int data)
//{
//	int end = list->num - 1, start = 0;
//	int mid = 0;
//	bool have_found = false;
//
//	while (mid != end)
//	{
//		mid = (end + start) / 2;
//		//cout << "mid = " << mid << " ";
//
//		if (list->a[mid] == data)
//		{
//			have_found = true;
//			return mid;
//		}
//
//		if (list->a[mid] > data)
//		{
//			end = mid - 1;
//		}
//		else if (list->a[mid] < data)
//		{
//			start = mid + 1;
//		}
//
//	}
//
//	if (have_found == false)
//	{
//		return -1;
//	}
//}
//
////二分查找，找下标(递归实现)
//void bin_digui(List* list, int data,int start,int end, int* index)
//{
//	int mid = (start + end) / 2;
//	//cout << mid << " ";
//
//	if (list->a[mid] == data)
//	{
//		//cout << "\n相等了\n";
//		*index = mid;
//		return;
//	}
//	else if (list->a[mid] > data)
//	{
//		end = mid - 1;
//	}
//	else if (list->a[mid] < data)
//	{
//		start = mid + 1;
//	}
//
//	if (start >= end)
//	{
//		mid = (start + end) / 2;
//		//cout << mid << " ";
//		//cout << "\nmid >= end 满足" << endl;
//		if (list->a[mid] == data)
//		{
//			*index = mid;
//			return;
//		}
//		else
//		{
//			return;
//		}
//	}
//
//	bin_digui(list, data, start, end, index);
//}
//
//int main(void)
//{
//	const int LENGTH = 100;
//
//	List* list = initList(LENGTH);
//
//	int i = 0;
//	for (i = 0; i < LENGTH; i++)
//	{
//		fu_zhi(list, i + 1);
//	}
//
//	//PrintList(list);
//	pai_xu(list);
//	//PrintList(list);
//
//
//	for (i = 0; i < LENGTH; i++)
//	{
//		cout << "\n迭代实现，" << i + 1 << "的索引是：" << bin_serach(list, i + 1) << endl;
//
//		int start = 0, end = list->num - 1, index = -1;
//
//		bin_digui(list, i + 1, start, end, &index);
//		if (index == -1)
//		{
//			cout << "\n数据不存在于单链表中" << endl;
//		}
//		else
//
//		{
//			cout << "\n递归实现，" << i + 1 << "的索引： " << index << endl;
//		}
//	}
//
//	return 0;
//}