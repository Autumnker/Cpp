//#define _CRT_SECURE_NO_WARNINGS 1
//
///*栈（没有迭代器）
//1.创建栈
//2.入栈
//3.出栈
//4.访问栈顶
//5.获取栈的长度
//6.遍历
//*/
//#include<iostream>
//#include<stack>
//
//using namespace std;
//
//int main(void)
//{
//	//【1】创建栈
//	stack <int> s;
//
//	//【2】入栈
//	s.push(1);
//	s.push(2);
//	s.push(3);
//	s.push(4);
//	s.push(5);
//	
//	cout << "访问栈顶元素：" << s.top() << endl;
//
//	//【3】出栈
//	s.pop();
//	cout << "访问栈顶元素：" << s.top() << endl;
//	
//	//【4】访问栈顶
//	s.pop();
//	cout << "访问栈顶元素：" << s.top() << endl;
//	
//	//【5】获取栈的长度
//	cout << "获取栈的长度：" << s.size() << endl;
//
//	//【6】遍历
//
//	stack <int> s2;
//
//	cout << "\n遍历s" << endl;
//	while (!s.empty())
//	{
//		cout << s.top() << " ";
//		s2.push(s.top());
//		s.pop();
//	}
//	cout << endl;
//
//	cout << "\n遍历s2" << endl;
//	while (!s2.empty())
//	{
//		cout << s2.top() << " ";
//		s2.pop();
//	}
//	cout << endl;
//
//	return 0;
//}
//
/////*stack 中的 swap 方法*/
////#include <iostream>
////#include <stack>
////
//////#define DBUG
////
////using namespace std;
////
////int main() {
////    std::stack<int> s1, s2;
////
////    // 向 s1 中添加元素
////    s1.push(1);
////    s1.push(2);
////    s1.push(3);
////
////    // 向 s2 中添加元素
////    s2.push(4);
////    s2.push(5);
////
////#ifdef DBUG
////
////    // 打印原始的 s1 和 s2 内容
////    std::cout << "Before swap:" << std::endl;
////    std::cout << "s1: ";
////    while (!s1.empty()) {
////        std::cout << s1.top() << " ";
////        s1.pop();
////    }
////    std::cout << std::endl;
////
////    std::cout << "s2: ";
////    while (!s2.empty()) {
////        std::cout << s2.top() << " ";
////        s2.pop();
////    }
////    std::cout << std::endl;
////
////#endif
////
////    // 交换 s1 和 s2 的内容
////    s1.swap(s2);
////
////    // 打印交换后的 s1 和 s2 内容
////    std::cout << "After swap:" << std::endl;
////    std::cout << "s1: ";
////    while (!s1.empty()) {
////        std::cout << s1.top() << " ";
////        s1.pop();
////    }
////    std::cout << std::endl;
////
////    std::cout << "s2: ";
////    while (!s2.empty()) {
////        std::cout << s2.top() << " ";
////        s2.pop();
////    }
////    std::cout << std::endl;
////
////    return 0;
////}