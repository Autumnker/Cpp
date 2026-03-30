#pragma once

class Chess {
public:
	bool color;
	int index;
	int x; int y;
	bool out;
	bool moveAble;
	bool choiced;

	//可行域
	int feasibleZone[3][2] = {
		{-1,-1},
		{-1,-1},
		{-1,-1}
	};


	//枚举变量
	enum COLOR { Blue, Red };																//颜色枚举
	enum Direction { Rx = 1, Ry, Rxy, Bx, By, Bxy };										//移动方向枚举

	//默认构造函数
	Chess();

	//带参构造函数
	Chess(bool color_, int index_, int x_, int y_, bool out_, bool moveAble_, bool choiced_);

	//初始化棋子
	void initChess();

	//计算可行域
	void FeasibleZone();

	//初始化可行域
	void initFeasibleZone();

	//等号运算符重载
	Chess& operator=(const Chess& other);

};