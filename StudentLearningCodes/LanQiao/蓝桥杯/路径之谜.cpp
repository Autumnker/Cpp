#define _CRT_SECURE_NO_WARNINGS 1

/*路径之谜*/

//1.【数据结构】：二维数组、两个一维数组、玩家类

//2.【算法】DFS

#include<iostream>
#include<stack>
using namespace std;
int N;

int* bei = new int[N];
int* xi = new int[N];

//坐标
struct point {
	int x;
	int y;
};

//储存路径坐标的栈
stack<point> st;

//玩家类
class player {
public:
	point now;//当前坐标
	
	//构造方法
	player()
	{
		now.x = 0;
		now.y = 0;
	}

	//移动方法,上下左右
	bool up(point now, int N, int* bei, int* xi)
	{
		if (now.x > 0 && xi[now.x - 1] > 0 && bei[now.y] > 0)
		{
			now.x--;
			xi[now.x]--;
			bei[now.y]--;

			return true;
		}
		else
			return false;
	}
	bool down(point now, int N, int* bei, int* xi)
	{
		if (now.x < N - 1 && xi[now.x + 1] > 0 && bei[now.y] > 0)
		{
			now.x++;
			xi[now.x]--;
			bei[now.x]--;

			return true;
		}
		else
			return false;
	}
	bool left(point now, int N, int* bei, int* xi)
	{
		if (now.y > 0 && xi[now.x] > 0 && bei[now.y - 1] > 0)
		{
			now.y--;
			xi[now.x]--;
			bei[now.y]--;

			return true;
		}
		else
			return false;
	}
	bool right(point now, int N, int* bei, int* xi)
	{
		if (now.y < N - 1 && xi[now.x] > 0 && bei[now.y + 1] > 0)
		{
			now.y++;
			xi[now.x]--;
			bei[now.y]--;

			return true;
		}
		else
			return false;
	}

};

//DFS
void DFS(player py, stack<point> st, int** map, int* xi, int* bei)
{
	
}

int mian(void)
{
	//获取输入并创建数据结构

	//DFS寻找路径

	//输出栈中的路径

	return 0;
}