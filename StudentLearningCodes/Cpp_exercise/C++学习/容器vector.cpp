//#define _CRT_SECURE_NO_WARNINGS 1
//
///*容器 vector（可变长数组）
//1.三种声明方式 vector v; vector v(10); vector v(10,1);
//2.追加元素 v.push_back(data);
//3.重新分配长度 v.resize(length);
//4.获取容器长度 v.size();
//5.迭代器() v.begin(); v.end(); 其中，“v.end();”是指末尾元素的下一个地址
//*/
//
//#include<vector>
//#include<iostream>
//
////用预处理操作符 "#" 获取容器变量的名称
//#define GET_VECTOR_NAME(name) #name
////失败，因为 name 无法动态变换
//
//using namespace std;
//
////输出容器中元素的函数
//void show(vector <int> a, string name)
//{
//	cout << name << ":";
//	for (auto p = a.begin(); p != a.end(); p++)//其中 p 是指针
//	{
//		cout << *p << " ";
//	}
//	cout << endl;
//}
//
//int main(void)
//{
//	//【1】三种声明方式及其含义
//	vector <int> a;//表示声明了一个空间大小为 0 的 int 型容器
//	vector <int> b(10);//表示声明了一个空间大小为 0 的 int 型容器,且其中每个元素都初始化为 0 
//	vector <int> c(10, 2);//表示声明了一个空间大小为 0 的 int 型容器,且其中每个元素都初始化为 2 
//
//	vector <string> name;
//	name.push_back("a");
//	name.push_back("b");
//	name.push_back("c");
//
//	//输出三个容器中的元素
//	show(a,name[0]);
//	cout << "a的长度为：" << a.size() << endl;
//
//	show(b,name[1]);
//	cout << "b的长度为：" << b.size() << endl;
//
//	show(c,name[2]);
//	cout << "c的长度为：" << c.size() << endl;
//
//	//【2】追加元素 v.push_back(data);
//	cout << "\n给各个容器追加一个元素‘123’\n" << endl;
//	a.push_back(123);
//	b.push_back(123);
//	c.push_back(123);
//	show(a,name[0]);
//	cout << "a的新长度为：" << a.size() << endl;
//
//	show(b,name[1]);
//	cout << "b的新长度为：" << b.size() << endl;
//
//	show(c,name[2]);
//	cout << "c的新长度为：" << c.size() << endl;
//
//	//【3】重新分配长度 v.resize(length);
//	cout << "\n给各个容器重新分配内存，新内存长度为15\n并且a追加的空间初始化为 3 ，b 为 5， c不进行显示的初始化\n" << endl;
//
//	a.resize(15,3);//让a中追加的内存初始化为 3 
//	show(a,name[0]);
//	cout << "a的长度为：" << a.size() << endl;
//
//	b.resize(15, 5);//确实是新追加的内存 初始化为 5 ,以前的内存不变
//	show(b,name[1]);
//	cout << "b的长度为：" << b.size() << endl;
//
//	c.resize(15);
//	show(c,name[2]);
//	cout << "c的长度为：" << c.size() << endl;
//
//	//【4】获取容器长度 v.length();
//	/*已经写过，不再赘述*/
//
//	//【5】迭代器() v.begin(); v.end(); 其中，“v.end(); ”是指末尾元素的下一个地址
//	cout << "\n迭代器已经实现过，但是还是再弄一下吧" << endl;
//	
//	auto pb = a.begin();//表示容器 a 的首个元素的地址
//	auto pe = a.end();//表示容器 a 的末尾元素的下一个地址
//	auto p = pb;//始终指向 a 的首地址
//
//	cout << "\n输出 a 中的元素\n" << endl;
//	for (; pb != pe; pb++)
//	{
//		cout << *pb << " ";
//	}
//	cout << endl;
//
//	pb = p;//指针归位
//
//	cout << "当对 a 使用 .clear() 函数之后" << endl;
//	a.clear();
//	if (a.begin() == a.end())//判断容器是否清空成功
//	{
//		cout << "\n容器已经清空，没有可输出元素\n" << endl;
//	}
//	else
//	{
//		for (; pb != pe; pb++)
//		{
//			cout << *pb << " ";
//		}
//		cout << endl;
//	}
//
//	return 0;
//}