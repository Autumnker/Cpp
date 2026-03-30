#pragma once
#include<graphics.h>
#include<string>
#include"Chess.h"

//游戏消息
struct GameMSG {
	/*---游戏状态信息---*/
	MOUSEMSG mouseMsg;												//鼠标消息

	/*---游戏状态信息---*/
	std::string state;												//游戏状态
	int turnNumber;													//游戏轮数
	bool controlTurn;												//是否进入控制骰子状态
	bool setOVer;													//棋子放置阶段结束

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


	/*---按钮信息---*/
	bool PVP;														//PVP模式
	bool HumanIsRed;												//人类是红色
	bool EVE;														//EVE模式
	bool controlHumanDice;											//控制玩家骰子
	bool controlAIDice;												//控制人类骰子

	Chess chess[6];
};

class Board {

private:
	/*---------------属性---------------*/

	//贴图常量
	IMAGE number, timer, blue_score, red_score, reddice[6], reddice0, bluedice[6], bluedice0, dice0,
		buttonred, buttonblue, button, red_turn, blue_turn, red_win, blue_win, notice;						//界面贴图

	IMAGE red[6], blue[6];																					//棋子贴图
	IMAGE black_cell, white_cell, choiced, choiced_black;													//方块贴图

	IMAGE pvp, pve, choiced_pvp, choiced_pve, red_one, blue_one, red_choiced, blue_choiced;					//模式与己方颜色显示贴图
	IMAGE mini[6], mini_choiced[6], hand_in_mac0, hand_in_mac1, hand_in_self0, hand_in_self1, EVE0, EVE1;	//人为选择的骰子点数

	//棋盘背景编号
	int BK_Cellcolor[5][5] = {
		{1,0,1,0,1},
		{0,1,0,1,0},
		{1,0,1,0,1},
		{0,1,0,1,0},
		{1,0,1,0,1}
	};

public:

	//游戏消息
	GameMSG Gamemsg;

	//棋子信息
	Chess redChess[6];												//红方玩家棋子数据
	Chess blueChess[6];												//蓝方玩家棋子数据

	//棋盘数据
	int CELL_SIZE = 150;
	int CELL_NUM = 5;
	int INTERFACE_WIDE = 200;
	int BK_color = LIGHTGRAY;
	int cor = 5;
	double od = 1 / 6.0;//表示概率的 6 倍，为了与棋盘坐标的价值适配
	int frequency = 1000;//搜索次数
	int back_num = 6; //可悔棋的次数


	//创建一个句柄变量,用于获取窗口句柄
	HWND hwnd;

	//按钮坐标【还需要花时间调整】
	int BoardArea[2][2] = { {0,0},{0,0} };									//棋盘区域坐标
	int PVPArea[2][2] = { {0,0},{0,0} };									//PVP按钮区域坐标
	int PVEArea[2][2] = { {0,0},{0,0} };									//PVE按钮区域坐标
	int HumanIsRed_Area[2][2] = { {0,0},{0,0} };							//红色按钮区域坐标
	int HumanIsBlue_Area[2][2] = { {0,0},{0,0} };							//蓝色按钮区域坐标
	int diceArea[2][2] = { {0,0},{0,0} };									//骰子按钮区域坐标
	int EVEArea[2][2] = { {0,0},{0,0} };									//EVE按钮区域坐标
	int ControlHumanDice_Area[2][2] = { {0,0},{0,0} };						//控制人类点数按钮坐标
	int ControlComputerDice_Area[2][2] = { {0,0},{0,0} };					//控制机器点数按钮坐标


	/*---------------方法---------------*/
	Board();

	/*-------背景框架--------*/
	//加载贴图
	void Load_pic();

	//创键背景
	void Creat_Background();

	//画方框
	void Draw_UI();

	//关闭UI
	void Close_UI()const;

	/*------对象绘制---------*/
	void Set_Text(int x, int y, int bkColor = WHITE, int textColor = BLACK, int info_x = 25, int info_y = 25, int info = 0)const;	//绘制文字
	void Set_INFO(int x = 750, int y = 410, int bkColor = WHITE, int textColor = BLACK, int info_x = 25, int info_y = 25, std::string info = "测试",
		int x2 = 750, int y2 = 410, int info_x2 = 25, int info_y2 = 25, std::string info2 = "\0")const;								//绘制提示信息
	void Draw_Block()const;																											//绘制黑白方块
	void Draw_Chess()const;																											//绘制棋子
	void Draw_DiceNumber(bool setOver = false, bool isRedTurn = true, int diceindex = -1)const;										//绘制投骰子点数
	void Draw_TurnNum(int turnNumber = 1)const;																						//绘制轮数
	void Draw_Time(int redTime = 240, int blueTime = 240)const;																		//绘制双方时间
	void Draw_Score(int redScore = 0, int blueScore = 0)const;																		//绘制双方得分
	void Draw_Info(std::string state = "Normal", bool turnIsRedint = true, int diceChoiced_Index = -1)const;						//绘制提示信息
	void Draw_Turn(bool turnIsRed = true, bool controlTurn = false)const;															//绘制回合信息
	void Draw_SmallDice(bool controlTurn = false, int over_MiniIndex = -1, int choiced_Index = -1)const;							//绘制小骰子
	void Draw_MoveableChess(bool turnIsRed, Chess redChess[6], Chess blueChess[6]);													//绘制可移动棋子背景
	void Deaw_MovaablrDirection(bool turnIsRed, Chess redChess[6], Chess blueChess[6], int choiced_Index);							//绘制棋子的可移动方向

	void Draw_PVP(bool PVP = true)const;																							//绘制红色/蓝色按钮
	void Draw_HumanIsRed(bool HumanIsRed = true)const;																				//绘制人机/双人按钮
	void Draw_RollDice(bool diceClickable, bool diceClicked = false, bool turnIsRed = true)const;									//绘制投掷骰子按钮
	void Draw_EVE(bool EVE = false)const;																							//绘制EVE按钮
	void Draw_ControlHumanDice(bool ConrtolHumanDice = false)const;																	//绘制控制玩家点数按钮
	void Draw_ControlAIDice(bool ControolAIDice = false)const;																		//绘制控制电脑点数按钮

};
