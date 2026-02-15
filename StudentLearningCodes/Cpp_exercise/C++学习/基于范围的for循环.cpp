//#define _CRT_SECURE_NO_WARNINGS 1
//
///*基于范围的for循环
//1.传值
//2.传地址
//3.auto 配合容器进行循环
//*/
//#include<iostream>
//#include<vector>
//#include<algorithm>
//
//using namespace std;
//
//template<typename T>
//
////访问元素( 0 为传值方式， 1 为传引用方式)
//void show(T &a, int flag, int num)
//{
//	cout << endl;
//
//	if(flag == 0)
//	{
//		cout << "传值," << "(num = " << num << "): ";
//
//		for (int i : a)//这里的 i 是传值, i : a，类似于Java中的范围循环，其中 i = a[i]； 
//		{
//			cout << i + num << " ";
//			i += num;
//			//i++;
//		}
//	}
//	else if(flag == 1)
//	{
//		cout << "传引用," << "(num = " << num << "): ";
//
//		for (int& i : a)//注意这里的 i 前面有 &，即为传引用
//		{
//			cout << i + num << " ";
//			i += num;
//			//i++;不需要自增，i 会自动遍历 a 中的所有元素
//		}
//	}
//	cout << endl;
//}
//
//int main(void)
//{
//	int a[] = { 1,2,3,4,5,6 };
//	//【1】传值
//	show(a, 0, 1);
//	show(a, 1, 0);
//	show(a, 0, 0);
//
//	//【1】传地址
//	show(a, 1, 1);
//	show(a, 0,0);
//	//【1】auto配合容器进行循环
//	vector<int> b(10,-1);
//	show(b, 0, 0);
//	show(b, 1, 0);
//	show(b, 1, 1);
//	show(b, 0, 0);
//	show(b, 1, 1);
//	show(b, 0, 0);
//
//	return 0;
//}