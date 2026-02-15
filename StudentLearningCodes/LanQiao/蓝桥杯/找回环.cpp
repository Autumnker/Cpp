/////*找回环*/
////#include<iostream>
////#include<algorithm>
////#include<vector>
////#include<queue>
////#include<set>
////#include<stack>
////using namespace std;
////
////int N;//电脑数量 
////stack<int> s;
////
//////输出地图 
////void show(int** map)
////{
////	for (int i = 0; i < N; i++)
////	{
////		for (int j = 0; j < N; j++)
////		{
////			cout << map[i][j] << " ";
////		}
////		cout << endl;
////	}
////}
////
//////初始化地图 
////int** initmap(int N)
////{
////	int** map = new int* [N];
////	for (int i = 0; i < N; i++)
////	{
////		map[i] = new int[N];
////		for (int j = 0; j < N; j++)
////		{
////			map[i][j] = 0;
////		}
////	}
////
////	return map;
////}
////
//////DFS
/////*
////1.先遍历，如果有值就进栈，否则退出
////2.当回到某一个节点时才算闭环
////*/
////
////void DFS(int** map, stack<int>& s, int* a, int i0, int j0, int* m)
////{
////	if (a[i0] == 1)
////	{
////		*m = i0;
////		return;
////	}
////
////	for (int i = i0; i < N; i++)
////	{
////		for (int j = j0; j < N; j++)
////		{
////			if (map[i][j] == 1)
////			{
////				if (a[i] == 0)
////				{
////					s.push(i);
////					a[i] = 1;
////				}
////
////				DFS(map, s, a, j, 0,m);
////			}
////			if (j == N - 1 && map[i][j] == 0)
////			{
////				return;
////			}
////		}
////	}
////}
////
////int main(void)
////{
////	//录入数据
////	cin >> N;
////	int** map = initmap(N);
////	int* a = new int[N];
////	int x, y;
////	for (int i = 0; i < N; i++)
////	{
////		cin >> x >> y;
////		map[x - 1][y - 1] = 1;
////		a[i] = 0;
////	}
////
////	show(map);
////
////	//DFS
////	int m = 0;
////	DFS(map, s, a, 0, 0,&m);
////	cout << "m= " << m << endl;
////	set<int>st;
////	while (s.top() != m)
////	{
////		st.insert(s.top() + 1);
////		s.pop();
////	}
////	st.insert(s.top() + 1);
////	s.pop();
////
////
////	for (auto i : st)
////	{
////		cout << i << " ";
////	}
////
////	return 0;
////}
//
///*换个思路*/
//
/////*找回环*/
////#include<iostream>
////#include<algorithm>
////#include<vector>
////#include<queue>
////using namespace std;
////
////int N;//电脑数量 
//// 
//// //输出地图 
//// void show(int** map)
//// {
//// 	for(int i = 0; i < N; i++)
//// 	{
//// 		for(int j = 0; j < N; j++)
//// 		{
//// 			cout << map[i][j] << " " ;
////		}
////		cout << endl;
////	}
//// }
//// 
//////初始化地图 
////int** initmap(int N)
////{
////	int** map = new int*[N];
////	for(int i = 0; i < N; i++)
////	{
////		map[i] = new int[N];
////		for(int j = 0; j < N; j++)
////		{
////			map[i][j] = 0;
////		}
////	}
////	
////	return map;
////} 
////
////
////void DFS(int** map, queue<int>& q, int* a, int i0, int j0)
////{
////	if(a[i0] == 1)
////	{
////		return;
////	}
////	
////	for(int i = i0; i < N; i++)
////	{
////		for(int j = j0; j < N; j++)
////		{
////			if(map[i][j] == 1)
////			{
////				if(a[i] == 0)
////				{
////					q.push(i);
////					a[i] = 1;
////				}
////				
////				DFS(map,q,a,j,0);
////			}
////			
////			if(j == N - 1 && map[i][j] == 0)
////			{
////				return;
////			}
////		}
////	}
////}
////
////int main(void)
////{
////	//录入数据
////	cin >> N;
////	int** map = initmap(N);
////	int x,y;
////	for(int i = 0; i < N; i++)
////	{
////		cin >> x >> y;
////		map[x-1][y-1] = 1;
////	}
////	
////	//show(map);
////	
////	//DFS
////	int a[N] = {0};
////	queue<int> q;
////	DFS(map,q,a,0,0);
////	
////	//输出结果 
////	sort(q.front(),q.back());
////	
////	while(!q.empty())
////	{
////		cout << q.front() << " ";
////		q.pop();
////	}
////	
////	return 0;
////} 
//
//
//
///*换个思路*/
//#include<iostream>
//#include<algorithm>
//#include<vector>
//#include<set>
//using namespace std;
//
//struct node {
//	int data;
//	struct node* next;
//};
//
//node* initnode()
//{
//	node* p = new node;
//	p->data = 0;
//	p->next = NULL;
//
//	return p;
//}
//
//int main(void)
//{
//	int N;
//	cin >> N;
//	int* a = new int[N];
//	node* ptr = new node[N];
//	for (int i = 0; i < N; i++)
//	{
//		a[i] = 0;
//		ptr[i].data = 0;
//		ptr[i].next = NULL;
//	}
//
//	int x, y;
//
//	node* head = NULL;
//
//	for (int i = 0; i < N; i++)
//	{
//		cin >> x >> y;
//		x = x - 1;
//		y = y - 1;
//
//		ptr[x].data = x;
//		a[x] += 1;
//		ptr[y].data = y;
//		a[y] += 1;
//		ptr[x].next = &ptr[y];
//
//		if (a[x] >= 2)
//		{
//			head = &ptr[x];
//		}
//		else if (a[y] >= 2)
//		{
//			head = &ptr[y];
//		}
//	}
//
//	vector<int> v;
//	node* now = head->next;
//	
//	do {
//		v.push_back(now->data + 1);
//		now = now->next;
//	} while (now != head->next);
//
//
//	set<int>st;
//	for (int i : v)
//	{
//		st.insert(i);
//	}
//	for (int i : st)
//	{
//		cout << i << " ";
//	}
//
//	return 0;
//}