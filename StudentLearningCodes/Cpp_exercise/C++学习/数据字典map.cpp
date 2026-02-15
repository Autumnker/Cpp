//#define _CRT_SECURE_NO_WARNINGS 1
//
///*map具有按照ASC码排序的特性，也可以自定义排序规则
//1.创建键值对
//2.添加
//3.遍历
//4.访问
//5.获取长度
//6.删除
//7.自定义排序函数（排序方式）
//*/
//
//#include<iostream>
//#include<map>
//#include<algorithm>
//
//using namespace std;
//
//////根据整数，由大到小排序
////bool cmp(int x, int y)
////{
////	if (x > y)
////	{
////		return true;
////	}
////	else
////		return false;
////}
//
//int main(void)
//{
//	//【1】创建键值对
//	map <string, int> maper;
//	//【2】添加
//	maper["one"] = 1;
//	maper["two"] = 2;
//	maper["three"] = 3;
//	maper["four"] = 4;
//	maper["five"] = 5;
//	//【3】遍历
//	for (auto ptr = maper.begin(); ptr != maper.end(); ptr++)
//	{
//		cout << ptr -> first << " " << ptr -> second << endl;
//	}
//
//	//【4】访问
//	cout << maper["three"] << endl;
//
//	//【5】获取长度
//	cout << maper.size() << endl;
//
//	//【6】删除键值对{查找：maper.find()其中[括号中为键，返回值为指针] / 删除：maper.erase()括号中是指针 }
//	maper.erase(maper.find("three"));
//	cout << maper.size() << endl;
//
//	//【7】自定义排序函数
//	//sort(&(maper.begin()->second), &(maper.end()->second), cmp);//字符串从大到小排序
//
//	//cout << "\n\n\n" << endl;
//	//for (auto ptr = maper.begin(); ptr != maper.end(); ptr++)
//	//{
//	//	cout << ptr->first << " " << ptr->second << endl;
//	//}
//
//	return 0;
//}
//
//
/////*非排序字典 unordered_map*/
////#include<unordered_map>
////#include<iostream>
////using namespace std;
////
////int main(void)
////{
////	unordered_map <string,int> un_map;
////
////	un_map["abc"] = 123;
////	un_map["adef"] = 456;
////	un_map["aghi"] = 789;
////	un_map["jakl"] = 13;
////
////	for (auto p = un_map.begin(); p != un_map.end(); p++)
////	{
////		cout << p->first << " " << p->second << endl;
////	}
////
////	return 0;
////}