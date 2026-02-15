#define _CRT_SECURE_NO_WARNINGS 1
/*哈希表
1.创建固顶长度的数组
2.设计哈希函数
3.创建put函数
*/
#include<iostream>
#include<vector>
#include<set>
#include<algorithm>

using namespace std;

const int LEN = 5;
int* init_a(int n)
{
	int* a = new int[n];
	int i = 0;
	for (i = 0; i < n; i++)
	{
		a[i] = 0;
	}

	return a;
}

int hax(int data)
{
	int n = data % LEN;
	return n;
}

void put(int* a, int len, int data)
{
	if (a[len - 1] != 0)
	{
		cout << "\n数组已满\n" << endl;
		return;
	}
	else
	{
		int x = hax(data);
		int count = 1;
		while (a[x] != 0)
		{
			x = hax(data + count);
			count++;
			if (count >= len)
			{
				break;
			}
		}
		if (count < len)
		{
			a[x] = data;
		}
	}
}

void show(int* a, int len)
{
	int i = 0;
	for (i = 0; i < len; i++)
	{
		cout << a[i] << " ";
	}
	cout << endl;
}

int main(void)
{
	int* a = init_a(LEN);

	put(a, LEN, 1);
	put(a, LEN, 2);
	put(a, LEN, 30);
	put(a, LEN, 4);
	put(a, LEN, 50);
	put(a, LEN, 6);
	put(a, LEN, 7);

	show(a, LEN);

	return 0;
}