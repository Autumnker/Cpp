//#define _CRT_SECURE_NO_WARNINGS 1
//
//#include<iostream>
//
//using namespace std;
//
////最大公约数
//int gcd(int a, int b)
//{
//	if (a != 0 && b != 0)
//	{
//		while (b != 0)
//		{
//			int temp = b;
//			b = a % b;
//			a = temp;
//		}
//	}
//	return a;
//}
//
//int lcm(int a, int b)
//{
//	return (a * b) / gcd(a, b);
//}
//
//int main(void)
//{
//	int a, b;
//	cin >> a >> b;
//	cout << "gcd= " << gcd(a, b) << endl;
//	cout << "lc,= " << lcm(a, b) << endl;
//
//	return 0;
//}