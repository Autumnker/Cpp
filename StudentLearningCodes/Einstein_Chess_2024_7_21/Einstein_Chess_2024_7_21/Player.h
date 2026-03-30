#pragma once
#include<string>
#include"Chess.h"
#include"Dice.h"
#include"AI.h"


class Player {
public:
	/*---------------属性---------------*/
	bool color;																				//颜色
	Chess chess[6];																			//棋子							
	int totalChessNumber;																	//剩余棋子总数

	bool setOver;																			//己方棋子放置完毕
	int chess_Choiced_Index;																//选中的棋子索引
	static bool turn;																		//玩家回合
	bool isAI;																				//玩家是机器人
	int time;																				//玩家时间
	bool winner;																			//本轮赢家
	int score;																				//玩家分数

	AI ai;																					//玩家自带AI


	//枚举变量
	enum COLOR { Blue, Red };																//颜色枚举
	enum Direction { Rx = 1, Ry, Rxy, Bx, By, Bxy };										//移动方向枚举

	//摆放棋子的可行域
	int redArea[6][2] = { {0,0}, {0,1}, {0,2}, {1,0}, {1,1}, {2,0} };
	int blueArea[6][2] = { {4,4}, {4,3}, {4,2}, {3,4}, {3,3}, {2,4} };


	/*---------------方法---------------*/

	//带参构造函数
	Player(bool color_, int totalChessNumber_, bool turn_, int time_, int score_);

	//摆放棋子
	void SetChess(int x, int y, Dice& dice);

	//棋子是否在可放置区域
	bool InSetArea(bool color, int x, int y);

	//判断目标位置是否有友方单位
	bool HaveFriedflyUnit(int x, int y);

	//交换回合
	void ChangeTurn(Player& redPlayer, Player& bluePlayer, Dice& dice);

	//投掷骰子
	void RollDice(Dice& dice);

	//计算可移动棋子的编号
	void ConputMoveAbleChessIndex(int diceNumber);

	//选择将要移动的棋子
	void ChoiceChess(int x, int y);

	//确定移动方向
	int ChoiceDirection(int chessIndex, int x, int y);

	//判断移动方向是否合法
	bool RightDirection(int chessIndex, int direction);

	//移动棋子
	void MoveChess(int x, int y, Player& otherPlayer, Dice& dice);

	//对弈方法
	void Fight(Player& otherPlayer, Dice& dice, bool controlAIDice, bool controlHumanDice, int x_CELL_SIZE, int y_CELL_SIZE, bool LbuttonDown);

	//计算己方剩余棋子数量
	void ConputTotalChessNumber();

	//一局胜利判断
	void WinOnce(Player& redPlayer, Player& bluePlayer);

	//初始化方法
	void initPlayer();


	/*-----------------------因AI而被放弃的方法---------------------------*/
	//自动摆放棋子
	//void AutoSetChess(Player& redPlayer, Player& bluePlayer, Dice& dice);
};