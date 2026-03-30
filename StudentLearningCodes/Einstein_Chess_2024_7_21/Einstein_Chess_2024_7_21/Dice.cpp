#include"Dice.h"
#include<random>

//初始化骰子
void Dice::InitDice()
{
	this->diceClickAble = false;
	this->diceClicked = false;
	this->diceIndex = -1;
	this->over_MiniIndex = -1;
	this->dice_Choiced_Index = -1;
}

//掷骰子
void Dice::RollDice()
{
	// 随机数生成器
	while (this->diceIndex < 0)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		// 使用均匀分布生成0到5之间的随机整数（骰子点数的下标）
		std::uniform_int_distribution<> dis(0, 5);
		this->diceIndex = dis(gen);													//将骰子点数索引赋给骰子
	}
}

//检测骰子是否被点击
void Dice::IfDiceClicked(unsigned int uMsg, int x, int y)
{
	if (this->diceClickAble)
	{
		if (uMsg == this->mouseLeftDown)
		{
			if (x >= this->bigDiceArea[0][0] && y >= this->bigDiceArea[0][1] &&
				x <= this->bigDiceArea[1][0] && y <= this->bigDiceArea[1][1])
			{
				this->diceClicked = true;
				this->diceClickAble = false;
			}
		}
		
	}
	
	if(uMsg == this->mouseLeftUp)
	{
		this->diceClicked = false;
	}
}

//判断鼠标是否悬停在小骰子区域的函数(返回 -1 则表示不在区域里；若在，则返回索引)
int Dice::IfInSmallDiceArea(int x, int y)
{
	for (int i = 0; i < 6; i++)
	{
		if (x >= this->smallDiceArea[i][0][0] && y >= this->smallDiceArea[i][0][1] && 
			x <= this->smallDiceArea[i][1][0] && y <= this->smallDiceArea[i][1][1])
		{
			return i;
		}
	}
	return -1;
}

//鼠标悬浮效果
void Dice::Over_MiniDice(int x, int y, bool controlTurn)
{
	if (controlTurn)
	{
		this->over_MiniIndex = this->IfInSmallDiceArea(x, y);
	}
}

//选择小骰子
void Dice::Choiced_Index(bool LbuttonDown, int x, int y)
{
	if (this->over_MiniIndex != -1)
	{
		if (LbuttonDown)
		{
			int choiced = this->IfInSmallDiceArea(x, y);
			if (choiced == this->over_MiniIndex)
				this->dice_Choiced_Index = choiced;
		}
	}
}

//将小骰子点数同步到大骰子,并掷出
void Dice::SyncToBigDice()
{
	//投掷骰子按钮在选中点数前始终可以被点击
	if (this->dice_Choiced_Index == -1)
	{
		this->diceClickAble = true;
	}

	//如果骰子被点击
	if (this->diceClicked)
	{
		this->diceIndex = this->dice_Choiced_Index;
	}
}