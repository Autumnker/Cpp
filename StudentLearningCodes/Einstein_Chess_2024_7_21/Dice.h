#pragma once

class Dice {

	//鼠标消息
	unsigned int mouseMove = 512;
	unsigned int mouseLeftDown = 513;
	unsigned int mouseLeftUp = 514;
	unsigned int mouseRightDown = 516;
	unsigned int mouseRightUp = 517;

public:
	/*---------------属性---------------*/

	//骰子可以被点击
	bool diceClickAble;

	//骰子被点击
	bool diceClicked;

	//骰子点数的索引
	int diceIndex;

	//悬浮鼠标下的小骰子的索引
	int over_MiniIndex;

	//被选中的小骰子点数的索引
	int dice_Choiced_Index;

	//大骰子的坐标区域
	int bigDiceArea[2][2] = { {855,655} ,{945,745} };

	//小骰子的坐标区域
	int smallDiceArea[6][2][2] = {
		{{755,613} ,{785,663}},
		{{788,613} ,{818,663}},
		{{821,613} ,{851,663}},
		{{854,613} ,{884,663}},
		{{887,613} ,{917,663}},
		{{920,613} ,{950,663}}
	};

	/*---------------方法---------------*/

	//无参构造函数
	Dice() :diceIndex(-1), diceClickAble(false), diceClicked(false), over_MiniIndex(-1), dice_Choiced_Index(-1) {};

	//初始化骰子
	void InitDice();

	//掷骰子
	void RollDice();

	//检测骰子是否被点击
	void IfDiceClicked(unsigned int mouseMsg, int x, int y);

	//判断鼠标是否悬停在小骰子区域的函数(返回 -1 则表示不在区域里；若在，则返回索引)
	int IfInSmallDiceArea(int x, int y);

	//鼠标悬浮效果
	void Over_MiniDice(int x, int y, bool controlTurn);

	//选择小骰子
	void Choiced_Index(bool LbuttonDown, int x, int y);

	//将小骰子点数同步到大骰子,并掷出
	void SyncToBigDice();
};