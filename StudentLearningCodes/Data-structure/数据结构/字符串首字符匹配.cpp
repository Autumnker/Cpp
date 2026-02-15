//#define _CRT_SECURE_NO_WARNINGS 1
//
///*自创：字符串首字符匹配*/
//#include<iostream>
//
//using namespace std;
//
////将要用到的变量
//string input, check;
//int in_len, che_len;
//
//int main(void)
//{
//	//获取待匹配字段
//	cin >> input;
//	in_len = input.length();
//	//获取用于匹配的字段
//	cin >> check;
//	che_len = check.length();
//	//输出匹配信息(从待匹配字段的第几位到第几位)
//	int i = 0, j = 0;
//	bool win = false;
//	char top = check[0];//储存首字符
//	int times = 0;
//
//	for (i = 0; i <= in_len - che_len; i++)
//	{
//		int before = i;
//		times++;
//
//		if (input[i] == top)//关键一步
//		{
//			for (j = 1; j < che_len; j++)
//			{
//				times++;
//
//				if (input[i + 1] == check[j])
//				{
//					i++;
//					if (j == che_len - 1)
//					{
//						cout << "success " << "begin: " << before + 1 << " end: " << i << endl;
//						win = true;
//					}
//				}
//				else
//				{
//					i = before;
//					break;
//				}
//			}
//		}
//	}
//
//	if (win == false)
//	{
//		cout << "false" << endl;
//	}
//
//	cout << "times: " << times << endl;
//
//	return 0;
//}