#include"Chess.h"


//默认构造函数
Chess::Chess()
{
	//颜色初始化
	this->color = true;
	//索引初始化
	this->index = -1;

	//坐标初始化
	this->x = -1;
	this->y = -1;
	//出局状态初始化
	this->out = true;
	//可移动状态初始化
	this->moveAble = false;
	//被选中状态初始化
	this->choiced = false;
}

//带参构造函数
Chess::Chess(bool color_, int index_, int x_, int y_, bool out_, bool moveAble_, bool choiced_)
{
	this->color = color_;
	this->index = index_;
	this->x = x_;
	this->y = y_;
	this->out = out_;
	this->moveAble = moveAble_;
	this->choiced = choiced_;
}

//初始化棋子
void Chess::initChess()
{
	//可行域初始化
	for (int i = 0; i < 3; i++)
	{
		this->feasibleZone[i][0] = -1;
		this->feasibleZone[i][1] = -1;
	}

	//坐标初始化
	this->y = -1; this->x = -1;
	//出局状态初始化
	this->out = true;
	//可移动状态初始化
	this->moveAble = false;
	//被选中状态初始化
	this->choiced = false;

	//只有颜色{color}、索引{index}需要保留

}

//计算可行域
void Chess::FeasibleZone()
{
	if (this->choiced == false)
		return;

	if (this->color == this->Red)
	{
		if (this->x + 1 <= 4)
		{
			this->feasibleZone[0][0] = this->x + 1;
			this->feasibleZone[0][1] = this->y;
		}
		else
		{
			this->feasibleZone[0][0] = -1;
			this->feasibleZone[0][1] = -1;
		}

		if (this->x + 1 <= 4 && this->y + 1 <= 4)
		{
			this->feasibleZone[1][0] = this->x + 1;
			this->feasibleZone[1][1] = this->y + 1;
		}
		else
		{
			this->feasibleZone[1][0] = -1;
			this->feasibleZone[1][1] = -1;
		}

		if (this->y + 1 <= 4)
		{
			this->feasibleZone[2][0] = this->x;
			this->feasibleZone[2][1] = this->y + 1;
		}
		else
		{
			this->feasibleZone[2][0] = -1;
			this->feasibleZone[2][1] = -1;
		}
	}
	else
	{
		if (this->x - 1 >= 0)
		{
			this->feasibleZone[0][0] = this->x - 1;
			this->feasibleZone[0][1] = this->y;
		}
		else
		{
			this->feasibleZone[0][0] = -1;
			this->feasibleZone[0][1] = -1;
		}

		if (this->x - 1 >= 0 && this->y - 1 >= 0)
		{
			this->feasibleZone[1][0] = this->x - 1;
			this->feasibleZone[1][1] = this->y - 1;
		}
		else
		{
			this->feasibleZone[1][0] = -1;
			this->feasibleZone[1][1] = -1;
		}

		if (this->y - 1 >= 0)
		{
			this->feasibleZone[2][0] = this->x;
			this->feasibleZone[2][1] = this->y - 1;
		}
		else
		{
			this->feasibleZone[2][0] = -1;
			this->feasibleZone[2][1] = -1;
		}
	}
}

//初始化可行域
void Chess::initFeasibleZone()
{
	for (int i = 0; i < 3; i++)
	{
		this->feasibleZone[i][0] = -1;
		this->feasibleZone[i][1] = -1;
	}
}

//等号运算符重载
Chess& Chess::operator=(const Chess& other)
{
	this->color = other.color;
	this->index = other.index;
	this->x = other.x;
	this->y = other.y;
	this->out = other.out;
	this->moveAble = other.moveAble;
	this->choiced = other.choiced;

	return *this;
}