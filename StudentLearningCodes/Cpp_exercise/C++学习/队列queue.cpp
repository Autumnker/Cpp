//#define _CRT_SECURE_NO_WARNINGS 1
//
///*队列queue
//1.创建
//2.入队
//3.出队
//4.访问队首
//5.访问队尾
//6.获取长度
//7.遍历
//*/
//
//#include<iostream>
//#include<queue>
//
//using namespace std;
//
//int main(void)
//{
//	//【1】创建
//	queue <int> q;
//
//	//【2】入队
//	for (int i = 0; i < 10; i++)
//	{
//		q.push(i);
//	}
//	cout << "队首元素为：" << q.front() << ';' << "队尾元素为：" << q.back() << endl;
//
//	//【3】出队
//	q.pop();
//	cout << "队首元素为：" << q.front() << ';' << "队尾元素为：" << q.back() << endl;
//	
//	//【4】访问队首
//	q.push(11);
//	cout << "队首元素为：" << q.front() << endl;
//	
//	//【5】访问队尾
//	cout << "队尾元素为：" << q.back() << endl;
//	
//	//【6】获取长度
//	cout << "队列长度为：" << q.size();
//
//	//【7】遍历
//	cout << "\n遍历：" << endl;
//	while (!q.empty())
//	{
//		cout << q.front() << " ";
//		q.pop();
//	}
//
//	return 0;
//}