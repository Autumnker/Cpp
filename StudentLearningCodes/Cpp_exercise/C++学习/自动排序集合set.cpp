//#define _CRT_SECURE_NO_WARNINGS 1
//
///*自动排序集合 set
// 1.创建
// 2.插入
// 3.删除
// 4.查询
// 5.遍历
//*/
//
//#include<iostream>
//#include<set>
//
//using namespace std;
//
///*
//1.创建：set <int> a; a后面不能带括号，不能像 vector 那样初始化或者开辟空间【因为集合是由互异元素构成的】
//2.插入：a.inster(data); 插入元素后，set 集合 a 会自动将元素排序
//3.遍历：用迭代器
//4.查找：a.find(data); 返回值是一个指针
//5.删除：a.eraser(data);
//*/
//
//int main(void)
//{
//	//【1】创建集合
//	set <int> s;
//
//	//【2】插入元素
//	s.insert(1);
//	s.insert(2);
//	s.insert(3);
//	s.insert(3);
//
//	//【3】遍历集合
//	cout << "遍历已有元素" << endl;
//	auto pbegin = s.begin();
//	auto pend = s.end();
//	auto head = pbegin;
//	for (; pbegin != pend; pbegin++)
//	{
//		cout << *pbegin << " ";
//	}
//	cout << endl;
//	pbegin = head;
//
//	//【4】查找元素（返回指针）
//	auto ptr_2 = s.find(2);
//	cout << "通过 2 的指针访问：\n" << *ptr_2 << endl;
//
//	//【5】删除元素
//	cout << "插入元素 3 和 0 " << endl;
//	s.insert(3);//插入已有元素不会改变集合的内容
//	s.insert(0);//插入一个 0 
//
//	pbegin = s.begin();
//	pend = s.end();
//	head = pbegin;
//	for (; pbegin != pend; pbegin++)
//	{
//		cout << *pbegin << " ";
//	}
//	cout << endl;
//	pbegin = head;
//
//
//	//删除元素 0 
//	cout << "删除元素 0 " << endl;
//	s.erase(0);
//
//	pbegin = s.begin();
//	pend = s.end();
//	head = pbegin;
//	for (; pbegin != pend; pbegin++)
//	{
//		cout << *pbegin << " ";
//	}
//	cout << endl;
//	pbegin = head;
//
//	return 0;
//}

///*非排序集合 unordered_set*/
//#include<unordered_set>
//#include<set>
//#include<iostream>
//using namespace std;
//
//int main(void)
//{
//	unordered_set <int> un_set;
//
//	un_set.insert(300);
//	un_set.insert(37);
//	un_set.insert(12);
//	un_set.insert(34);
//	un_set.insert(6);
//	un_set.insert(9);
//
//	cout << "unordered_set:" << endl;
//	for (auto p = un_set.begin(); p != un_set.end(); p++)
//	{
//		cout << *p << " ";
//	}
//	cout << endl;
//
//	set <int> s;
//	s.insert(300);
//	s.insert(37);
//	s.insert(12);
//	s.insert(34);
//	s.insert(6);
//	s.insert(9);
//
//	cout << "set:" << endl;
//	for (auto ptr = s.begin(); ptr != s.end(); ptr++)
//	{
//		cout << *ptr << " ";
//	}
//	cout << endl;
//
//	return 0;
//}