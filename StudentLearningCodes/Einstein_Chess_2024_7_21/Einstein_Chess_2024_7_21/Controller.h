#pragma once
#include<string>
#include<graphics.h>
#include"Dice.h"
#include"Player.h"
#include"UI.h"
#include"Chess.h"

class Controller {
private:
	//鼠标消息
	unsigned int mouseMove = 512;
	unsigned int mouseLeftDown = 513;
	unsigned int mouseLeftUp = 514;
	unsigned int mouseRightDown = 516;
	unsigned int mouseRightUp = 517;

public:

	/*---------------属性---------------*/

	/*---游戏状态信息---*/
	//按钮坐标
	int BoardArea[2][2] = { {0,0},{745,745} };									//棋盘区域坐标
	int diceArea[2][2] = { {855,660},{940,740} };								//骰子按钮区域坐标

	int PVPArea[2][2] = { {858,550},{940,600} };								//PVP按钮区域坐标
	int PVEArea[2][2] = { { 860,470 },{940,535} };								//PVE按钮区域坐标
	int HumanIsRed_Area[2][2] = { {755,470},{840,535} };						//红色按钮区域坐标
	int HumanIsBlue_Area[2][2] = { {760,550},{842,600} };						//蓝色按钮区域坐标
	int EVEArea[2][2] = { {960,10},{1010,245} };								//EVE按钮区域坐标
	int ControlHumanDice_Area[2][2] = { {950,265},{1020,490} };					//控制人类点数按钮坐标
	int ControlComputerDice_Area[2][2] = { {950,515},{1010,740} };				//控制机器点数按钮坐标

	/*---游戏状态信息---*/
	MOUSEMSG mouseMsg;												//鼠标消息
	int x, y;														//坐标表示
	bool LbuttonNotDown;											//鼠标左键未被按下

	bool haveWinner;												//产生本局胜利者
	bool haveFinalWinner;											//产生最终胜利者

	bool initAll;													//初始化一切

	/*---游戏状态信息---*/
	std::string state;												//游戏状态
	int turnNumber;													//游戏轮数
	bool controlTurn;												//是否进入控制骰子状态
	bool setOver;													//棋子摆放是否完毕

	/*---骰子信息---*/
	bool diceClickAble;												//骰子可以被点击
	bool diceClicked;												//骰子被点击
	int diceIndex;													//骰子点数索引
	int over_MiniIndex;												//悬浮状态的鼠标下的小骰子点数
	int dice_Choiced_Index;											//鼠标点击后选中的小骰子点数


	/*---玩家信息---*/
	bool turnIsRed;													//红色玩家回合
	int redTime;													//红色玩家时间
	int blueTime;													//蓝色玩家时间
	int redScore;													//红色玩家分数
	int blueScore;													//蓝色玩家分数
	int chess_Choiced_Index;										//玩家选中的棋子索引

	Chess redChess[6];												//红方棋子信息
	Chess blueChess[6];												//蓝方棋子信息


	/*---按钮信息---*/
	bool PVP;														//PVP模式
	bool HumanIsRed;												//人类是红色
	bool EVE;														//EVE模式
	bool controlHumanDice;											//控制玩家骰子
	bool controlAIDice;												//控制人类骰子

	/*---------------方法---------------*/

	//默认构造方法
	Controller();

	//获取鼠标消息
	void GetMouseMsg_control(Board& board);

	//判断鼠标点击是否在某一区域内
	bool MouseClickInArea(int area[2][2])const;

	//获取并改变骰子状态
	void GetDiceState(Dice& dice);

	//获取并改变玩家状态
	void GetPlayerState(Player& redPlayer, Player& bluePlayer, Dice& dice);

	//获取并改变按钮状态
	void GetButtonState();

	//判断是否进入控制骰子状态
	bool IfControlTurn(Player& redPlayer, Player& bluePlayer)const;

	//获取对弈阶段的游戏状态
	void GetFightGameState(Player& redPlayer, Player& bluePlayer);

	//将信息同步给UI
	void SyncToUI(Board& board);

	//状态转换
	void ChangeState(Player& redPlayer, Player& bluePlayer, Dice& dice);

	//单局胜利判断
	bool HaveWinner(Player& redPlayer, Player& bluePlayer);

	//最终胜利判断
	bool HaveFinalWinner(Player& redPlayer, Player& bluePlayer, Board& board);

	//开始新的一局，初始化信息
	void initGame(Player& redPlayer, Player& bluePlayer);

	//进入选择界面
	void ChoiceToExit_Replay(Player& redPlayer, Player& bluePlayer, Board& board, bool& running);

	//初始化一切(最终胜利者产生后，进行选择)
	void initAllObject(Player& redPlayer, Player& bluePlayer, Board& board, bool& running);

	//初始化方法
	void initController(Player& redPlayer, Player& bluePlayer);

	//根据按钮改变玩家是否是机器人的方法
	void ChangeToRobot(Player& redPlayer, Player& bluePlayer);

	//对弈
	void NewSetChess(Player& redPlayer, Player& bluePlayer, Dice& dice);
	
	//摆放棋子
	void NewFight(Player& redPlayer, Player& bluePlayer, Dice& dice);

	/*---------------------------------------可能会被弃用的方法--------------------------------------------------*/
#if 0
	//判断放置棋子阶段的游戏状态
	std::string SetChessGameState(Player& redPlayer, Player& bluePlay, Dice& dice);

	//对弈
	void Fight(Player& redPlayer, Player& bluePlayer, Dice& dice);
#endif

};