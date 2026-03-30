#include<stdio.h>
#include"UI.h"
#include"Controller.h"

int main(void)
{
	//枚举变量
	enum COLOR { Blue, Red };														//颜色枚举


	//游戏开关
	bool running = true;


	//创键对象
	Board board;																	//创键UI对象
	Controller controller;															//创键游戏控制器对象
	Player redPlayer(Red, 0, Red, 240, 0), bluePlayer(Blue, 0, Red, 240, 0);		//创键玩家对象
	Dice dice;																		//创键骰子对象

	//创键游戏背景
	board.Creat_Background();

	//加载贴图
	board.Load_pic();

	//开始游戏
	while (running)
	{
		//游戏控制器
		controller.GetMouseMsg_control(board);										//获取鼠标消息
		controller.GetButtonState();												//改变按钮状态
		controller.ChangeState(redPlayer, bluePlayer, dice);						//改变游戏状态
		controller.GetDiceState(dice);												//获取骰子信息
		controller.GetPlayerState(redPlayer, bluePlayer, dice);						//获取玩家信息
		
		
		controller.NewFight(redPlayer, bluePlayer, dice);							//对弈

		controller.GetFightGameState(redPlayer, bluePlayer);						//获取对战信息

		controller.SyncToUI(board);													//将信息同步给UI
		
		board.Draw_UI();															//绘制UI

		controller.initAllObject(redPlayer, bluePlayer, board, running);			//胜利判断与初始化
	}

	//游戏结束,关闭UI
	board.Close_UI();

	return 0;
}