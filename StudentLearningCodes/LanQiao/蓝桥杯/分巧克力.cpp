///*分巧克力
//
//*数据：
//
//N = 巧克力数量
//K = 小朋友数量
//Wi\Hi = 每块巧克力的边长、高
//
//*要求：
//1.切出的巧克力是正方形
//2.巧克力边长是整数且是完整的 -> 小巧克力的边长比最小的大巧克力的边长小或者相等
//
//*求：
//最大变长
//
//*/
//
///*
//
//求解思路：
//
//1.大巧克力总面积 Ni / 小朋友数量 K = 小巧克力最大面积 S_max
//2.用 S_max 求出小巧克力最大边长 L_max
//3.大巧克力中边长最短的 W_min 是否大于 L_max ？ 最终边长 = W_min : L
//
//4.判断依据 ： 巧克力被正常（没有小数边，全是完整边）分完，且数量大于小朋友数量
//
//*/
//
///*
//关键是分大巧克力的函数
//*/
//
//#include<iostream>
//#include<cmath>
//using namespace std;
//
////创建巧克力类，获得小巧克力变长后返回能切出的最大小巧克力数量
//class Chocolate {
//public:
//	int H;
//	int W;
//	int area;
//public:
//	//获取边长 
//	void get_HW(int Hin, int Win)
//	{
//		H = Hin;
//		W = Win;
//		area = Hin * Win;
//	}
//	//返回最大小巧克力数量
//	int max_num(int L_max)
//	{
//		int num = 0;
//
//		if (L_max > H || L_max > W)
//		{
//			return 0;
//		}
//		else
//		{
//			num = (H / L_max) * (W / L_max);//这是精髓所在 
//		}
//
//		return num;
//	}
//};
//
////初始化巧克力对象
//void initChocolate(Chocolate* a, int N)
//{
//	for (int i = 0; i < N; i++)
//	{
//		a[i].H = 0;
//		a[i].W = 0;
//		a[i].area = 0;
//	}
//}
//
////将所有大巧克力能切出的小巧克力的数量加起来并返回出来
//int Total_num(Chocolate a[], int N, int L_max)
//{
//	int total = 0;
//	for (int i = 0; i < N; i++)
//	{
//		total += a[i].max_num(L_max);
//	}
//
//	return total;
//}
//
////判断 Total_num 是否大于等于小朋友数量
//bool judge(Chocolate a[], int N, int L_max, int K)
//{
//	if (Total_num(a, N,L_max) >= K)
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
//
////返回小巧克力最大边长(二分查找)
//int Max(Chocolate a[], int N, int K, int Large)
//{
//	int L = 1;
//	int R = Large;
//	int mid = 0;
//	int max = 0;
//
//	while (L <= R)
//	{
//		mid = (L + R) / 2;
//
//		if (judge(a, N, mid, K))
//		{
//			max = mid;
//			L = mid + 1;
//		}
//		else
//		{
//			R = mid - 1;
//		}
//	}
//
//	return max;
//}
//int main(void)
//{
//
//	//获取 巧克力和小朋友的数量
//	int N, K;
//	cin >> N >> K;
//	Chocolate* a = new Chocolate[N];
//	initChocolate(a, N);
//
//	//获取每一块巧克力的边长和高度
//	int max_area = 0;
//
//	for (int i = 0; i < N; i++)
//	{
//		int h, w;
//		cin >> h >> w;
//		a[i].get_HW(h, w);
//
//		max_area += a[i].area;
//	}
//	int Large = sqrt(max_area);
//	//计算小巧克力的最大边长
//	int fin = Max(a, N, K, Large);
//
//	cout << fin;
//
//	return 0;
//}
//
//

///*别人的代码*/
////#include <iostream>
////using namespace std;
////const int def = 100005;
////int h[def] = { 0 };
////int w[def] = { 0 };
////int n, k;
//////求巧克力的块数--除法
////bool check(int x)//边长为x的正方形
////{
////    int sum = 0;
////    for (int i = 0; i < n; i++)
////    {
////        sum += (h[i] / x) * (w[i] / x);
////    }
////    if (sum >= k)return true;
////    else
////        return false;
////}
////int main()
////{
////    // 请在此输入您的代码
////
////    cin >> n >> k;
////    for (int i = 0; i < n; i++)
////    {
////        cin >> h[i] >> w[i];
////    }
////    //二分法判断边长
////    int l = 1;
////    int r = 100000;
////    int max = 0;
////    while (l <= r)//注意判断条件包括=
////    {
////        int mid = (l + r) / 2;
////        if (check(mid))//mid边长是满足的
////        {
////            max = mid;
////            l = mid + 1;//要求最大边长，还需要继续判断
////        }
////        else
////            r = mid - 1;//说明不满足，需要缩减边长
////    }
////    cout << max;
////    return 0;
////}