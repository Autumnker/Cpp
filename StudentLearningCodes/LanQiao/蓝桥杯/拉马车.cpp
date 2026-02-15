////#define _CRT_SECURE_NO_WARNINGS 1
////
///*
//数据：
//A的牌顺序
//B的牌顺序
//
//流程：
//1.A、B轮流打牌，将自己的牌插入队列
//2.当某个人已经打出的牌是 K，就把队列中最后一个 K 以及之后的所有牌 逆序插入自己手中牌的队尾
//3.当某个人没有牌可打且无法赢回牌时，输出赢家的牌序。
//4.如果游戏无法结束，则输出 -1
//
//解题思路：
//1.创建三个队列 A B C
//2.当插入字母为 K 时，将 C 中两 K 之间的牌逆序插入牌手队尾
//3.当A、B其中一个为空且无法赢牌时，输出另一个的牌序
//4. 如果游戏无法结束则输出 -1
//*/
//
//#include <iostream>
//#include<queue>
//#include<vector>
//#include<stack>
//#include<unordered_map>
//using namespace std;
//
//queue<char> A, B;
//queue<char>& a = A;
//queue<char>& b = B;
//stack<char> C;
//int Ben[128] = { 0 };
//
////将玩家的牌插入牌堆（如果赢了，则返回false继续出，没赢则返回true）
//bool cha_ru(queue<char>& E)
//{
//	if (!E.empty())
//	{
//		char now = E.front();
//		E.pop();
//		C.push(now);
//		Ben[now]++;
//
//			//当玩家赢牌，收牌
//			if (Ben[now] >= 2)
//			{
//				if (!C.empty() && C.top() == now)
//				{
//					E.push(C.top());
//					C.pop();
//					Ben[now]--;
//				}
//
//				while (!C.empty() && C.top() != now)
//				{
//					E.push(C.top());
//					Ben[C.top()]--;
//					C.pop();
//				}
//
//				if (!C.empty() && 
//					C.top() == now)
//				{
//					E.push(C.top());
//					C.pop();
//					Ben[now]--;
//				}
//
//				return false;
//			}
//		else
//		{
//			//cout << "\n\n" << now << "不在Ben中" << endl;
//			return true;
//		}
//
//		return true;
//	}
//	else
//	{
//		return true;
//	}
//}
//
////两玩家的对决
//void play(queue<char>& a, queue<char>& b)
//{
//	int flag = 0;//flag  = 0 是A的回合，flag = 1 是 B 的回合。
//	int times = 0;
//
//	while (!a.empty() && !b.empty() && times <= 100000)
//	{
//		//cout << "\n\na.size()= " << a.size() << "\nb.size()= " << b.size() << endl;
//		
//		if ( !b.empty() && flag == 0)
//		{
//			if (cha_ru(a))
//			{
//				flag = 1;
//			}
//		}
//		
//		if (!a.empty() && flag == 1)//如果a成功插入，turn置为true，开始对b进行插入操作
//		{
//			if (cha_ru(b))
//			{
//				flag = 0;
//			}
//		}
//
//		times++;
//	}
//
//	//输出赢家的牌序
//	if (times < 100001)
//	{
//		if (a.empty())
//		{
//			//cout << "a.empty" << endl;
//			while (!b.empty())
//			{
//				cout << b.front();
//				b.pop();
//			}
//		}
//		else if (b.empty())
//		{
//			//cout << "b.empty" << endl;
//			while (!a.empty())
//			{
//				cout << a.front();
//				a.pop();
//			}
//		}
//		else
//			cout << -1;
//	}
//	else
//	{
//		cout << -1;
//	}
//
//	//cout << "\n\nBen的值:" << endl;
//	//for (auto i : Ben)
//	//{
//	//	cout << i.first << ':' << i.second << " ";
//	//}
//	//cout << "\n\n";
//
//}
//
//int main()
//{
//	//获取字符串
//	string ab, ba;
//	cin >> ab >> ba;
//	int i = 0;
//	for (char i : ab)
//	{
//		A.push(i);
//	}
//	for (char j : ba)
//	{
//		B.push(j);
//	}
//
//	play(A, B);
//
//	/*cout << " \nC:";
//	while (!C.empty())
//	{
//		cout << C.top() << " ";
//		C.pop();
//	}
//
//	cout << " \nA:";
//	while (!A.empty())
//	{
//		cout << A.front() << " ";
//		A.pop();
//	}
//
//	cout << " \nB:";
//	while (!B.empty())
//	{
//		cout << B.front() << " ";
//		B.pop();
//	}*/
//
//	return 0;
//}
//
/////*别人的代码*/
////#include<iostream>
////#include<string>
////#include<stack>
////using namespace std;
////bool a[128]; // a[i]表示牌堆中是否存在i这张牌 
////int main() {
////    string A, B;
////    cin >> A >> B;
////    stack<char> S; // 用栈作为牌堆
////    S.push(A[0]); a[A[0] - 0] = 1; A.erase(0, 1); // A先出牌
////    bool flag = 1; // flag控制到谁出牌 
////    int times = 0; // times表示出牌次数，超过10000认为会无限循环 
////    while (A.length() && B.length() && times < 10000) {
////        //cout<<A<<","<<B<<endl; 
////        string* sp = flag ? &B : &A; // flag为1时B出牌，将string指针指向B，方便实现B的出牌和收牌 
////        char tmp = (*sp)[0];
////        S.push(tmp); sp->erase(0, 1); // 玩家出牌 
////        if (a[tmp - 0] == 0) { a[tmp - 0] = 1; flag = !flag; } // 牌堆中没有当前出的牌，牌权更换 
////        else { // 若包含当前字符，收回一部分牌 
////            *sp += S.top(); S.pop(); // 收回刚出的那张牌，位于栈顶 
////            while (S.top() != tmp) { *sp += S.top(); a[S.top() - 0] = 0; S.pop(); } //一直收牌到与所出牌相同的另一张牌处 
////            *sp += S.top(); a[S.top() - 0] = 0; S.pop();
////        }
////        times++;
////    }
////    if (times >= 10000) return -1;
////    if (A.length()) cout << A; else cout << B;
////    return 0;
////}
//
////#include <iostream>
////#include <queue>
////#include <stack>
////#include <unordered_map>
////using namespace std;
////
////queue<char> A, B;
////stack<char> C;
////unordered_map<char, int> Ben;
////
////bool cha_ru(queue<char>& E) {
////    if (!E.empty()) {
////        char now = E.front();
////        E.pop();
////        if (Ben.find(now) != Ben.end()) {
////            C.push(now);
////            Ben[now]++;
////            if (Ben[now] > 1) {
////                if (!C.empty() && C.top() != now) {
////                    E.push(C.top());
////                    C.pop();
////                    Ben[now]--;
////                }
////                while (!C.empty() && C.top() != now) {
////                    E.push(C.top());
////                    C.pop();
////                }
////                if (!C.empty() && C.top() == now) {
////                    E.push(C.top());
////                    C.pop();
////                    Ben[now]--;
////                }
////                return false;
////            }
////        }
////        else {
////            return true;
////        }
////        return true;
////    }
////    else {
////        return true;
////    }
////}
////
////void play(queue<char>& a, queue<char>& b) {
////    int flag = 0;
////    int times = 0;
////    while (!a.empty() && !b.empty() && times <= 10000) {
////        if (!b.empty() && flag == 0) {
////            if (cha_ru(a)) {
////                flag = 1;
////            }
////        }
////        if (!a.empty() && flag == 1) {
////            if (cha_ru(b)) {
////                flag = 0;
////            }
////        }
////        times++;
////    }
////    if (times < 10000) {
////        if (a.empty()) {
////            while (!b.empty()) {
////                cout << b.front();
////                b.pop();
////            }
////        }
////        else if (b.empty()) {
////            while (!a.empty()) {
////                cout << a.front();
////                a.pop();
////            }
////        }
////        else {
////            cout << -1;
////        }
////    }
////    else {
////        cout << -1;
////    }
////}
////
////int main() {
////    Ben['2'] = 0; Ben['3'] = 0; Ben['4'] = 0; Ben['5'] = 0; Ben['6'] = 0; Ben['7'] = 0; Ben['8'] = 0; Ben['9'] = 0;
////    Ben['10'] = 0; Ben['J'] = 0; Ben['Q'] = 0; Ben['K'] = 0; Ben['A'] = 0;
////
////    string ab, ba;
////    cin >> ab >> ba;
////    for (char i : ab) {
////        A.push(i);
////    }
////    for (char j : ba) {
////        B.push(j);
////    }
////
////    play(A, B);
////
////    return 0;
////}
//
/////*别人的代码*/
////#include<iostream>
////#include<queue>
////#include<stack>
////#include<cstring>
////using namespace std;
////queue<char>a, b;
////stack<char>s;
////string s1, s2;
////int check[100];
////int order = 1;
////int main()
////{
////    cin >> s1 >> s2;
////    for (int i = 0; i < s1.length(); i++)
////    {
////        a.push(s1[i]);
////    }
////    for (int i = 0; i < s2.length(); i++)
////    {
////        b.push(s2[i]);
////    }
////    while (!a.empty() && !b.empty())
////    {
////
////        if (order == 1)
////        {
////            s.push(a.front());
////            a.pop();
////            if (check[s.top()])
////            {
////                order = 1;
////                char tmp = s.top();
////                a.push(s.top());
////                s.pop();
////                while (s.top() != tmp)
////                {
////                    a.push(s.top());
////                    check[s.top()] = 0;
////                    s.pop();
////                }
////                a.push(tmp);
////                check[tmp] = 0;
////                s.pop();
////            }
////            else
////            {
////                order = 2;
////                check[s.top()] = 1;
////            }
////        }
////        else
////        {
////            s.push(b.front());
////            b.pop();
////            if (check[s.top()])
////            {
////                order = 2;
////                char tmp = s.top();
////                b.push(tmp);
////                s.pop();
////                while (s.top() != tmp)
////                {
////                    b.push(s.top());
////                    check[s.top()] = 0;
////                    s.pop();
////                }
////                b.push(tmp);
////                check[tmp] = 0;
////                s.pop();
////
////            }
////            else
////            {
////                order = 1;
////                check[s.top()] = 1;
////            }
////        }
////    }
////    while (!a.empty())
////    {
////        cout << a.front();
////        a.pop();
////    }
////    while (!b.empty())
////    {
////        cout << b.front();
////        b.pop();
////    }
////    return 0;
////}