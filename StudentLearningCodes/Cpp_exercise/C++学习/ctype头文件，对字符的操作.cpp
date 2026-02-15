//#define _CRT_SECURE_NO_WARNINGS 1
//
///*cctype头文件，对字符进行操作和判断
//1.isalpha() 是字母吗？
//2.islower() 小写字母？
//3.isupper() 大写字母？
//4.isalnum() 字母或数字？
//5.isspace() 是 空格、\t 、\r 、\n 吗？ 
//6.tolower() 转化为小写字母
//7.toupper() 转化为大写字母
//*/
//#include<iostream>
//#include<cctype>
//
//using namespace std;
//
//int main(void)
//{
//	char n = 'a', N = 'A';
//
//	cout << "isalpha():" << isalpha(n) << " " << isalpha(N) << endl;//返回值为bool
//	cout << "islower():" << islower(n) << " " << islower(N) << endl;//返回值为bool
//	cout << "isupper():" << isupper(n) << " " << isupper(N) << endl;//返回值为bool
//	cout << "isalnum():" << isalnum(n) << " " << isalnum(N) << endl;//返回值为bool
//	cout << "isspace():" << isspace(n) << " " << isspace(N) << endl;//返回值为bool
//	cout << "tolower():" << (char)tolower(n) << " " << (char)tolower(N) << endl;//是对字符的具体操作，返回值为 ASCⅡ码
//	cout << "toupper():" << (char)toupper(n) << " " << (char)toupper(N) << endl;//是对字符的具体操作，返回值为 ASCⅡ码
//
//	return 0;
//}