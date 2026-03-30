#include"UI.h"


Board::Board()
{
	//创键窗口
	this->hwnd = initgraph(CELL_SIZE * CELL_NUM + INTERFACE_WIDE + CELL_SIZE / 2, CELL_SIZE * CELL_NUM, 1);

	//初始化棋子
	for (int i = 0; i < 6; i++)
	{
		this->redChess[i].color = true;
		this->redChess[i].index = i;
		this->redChess[i].initChess();

		this->blueChess[i].color = false;
		this->blueChess[i].index = i;
		this->blueChess[i].initChess();
	}


	//初始化游戏消息
	this->Gamemsg = {
		{0,0,0,0,0,0,0,0,0},														//鼠标消息
		"Normal",1,false,false,														//游戏状态
		false,false,-1,-1,-1,														//骰子信息
		false,240,240,0,0,-1,														//玩家信息
		true,true,false,false,false													//按钮信息
	};

	return;
}


//创键背景
void Board::Creat_Background()
{
	////画方框
	//this->hwnd = initgraph(CELL_SIZE * CELL_NUM + INTERFACE_WIDE + CELL_SIZE / 2, CELL_SIZE * CELL_NUM, 1);

	//获取窗口句柄
	hwnd = GetForegroundWindow();
	SetWindowText(hwnd, "爱恩斯坦棋");
}

//绘制UI
void Board::Draw_UI()
{
	//设置棋盘背景颜色
	setbkcolor(this->BK_color);

	//双缓冲绘图
	BeginBatchDraw();
	cleardevice();

	// 刷新输出缓冲区
	FLUSHOUTPUT;

	//绘制黑白方块
	this->Draw_Block();

	/*********在此添加代码**********/

	this->Draw_TurnNum(this->Gamemsg.turnNumber);
	this->Draw_Time(this->Gamemsg.redTime, this->Gamemsg.blueTime);
	this->Draw_Score(this->Gamemsg.redScore, this->Gamemsg.blueScore);
	this->Draw_Turn(this->Gamemsg.turnIsRed, this->Gamemsg.controlTurn);
	this->Draw_HumanIsRed(this->Gamemsg.HumanIsRed);
	this->Draw_PVP(this->Gamemsg.PVP);
	this->Draw_RollDice(this->Gamemsg.diceClickAble, this->Gamemsg.diceClicked, this->Gamemsg.turnIsRed);
	this->Draw_DiceNumber(this->Gamemsg.setOVer, this->Gamemsg.turnIsRed, this->Gamemsg.diceIndex);
	this->Draw_SmallDice(this->Gamemsg.controlTurn, this->Gamemsg.over_MiniIndex, this->Gamemsg.dice_Choiced_Index);
	this->Draw_EVE(this->Gamemsg.EVE);
	this->Draw_ControlHumanDice(this->Gamemsg.controlHumanDice);
	this->Draw_ControlAIDice(this->Gamemsg.controlAIDice);
	this->Draw_MoveableChess(this->Gamemsg.turnIsRed, this->redChess, this->blueChess);
	this->Deaw_MovaablrDirection(this->Gamemsg.turnIsRed, this->redChess, this->blueChess, this->Gamemsg.chess_Choiced_Index);
	this->Draw_Chess();
	this->Draw_Info(this->Gamemsg.state, this->Gamemsg.turnIsRed, this->Gamemsg.dice_Choiced_Index);


	/******************************/


	Sleep(1);
	FlushBatchDraw();//双缓冲绘图
}

//关闭UI
void Board::Close_UI()const
{
	EndBatchDraw();//双缓冲绘图
	closegraph();
}



/*-----------------------------对象绘制-----------------------------*/

//绘制文字
void Board::Set_Text(int x, int y, int bkColor, int textColor, int info_x, int info_y, int info)const
{
	TCHAR INFO[4];
	sprintf_s(INFO, "%d", info);

	settextstyle(info_x, info_y, _T("宋体"));
	setbkcolor(bkColor);
	settextcolor(textColor);
	outtextxy(x, y, INFO);
}

//绘制提示信息
void Board::Set_INFO(int x, int y, int bkColor, int textColor, int info_x, int info_y, std::string info, 
					 int x2, int y2, int info_x2, int info_y2, std::string info2)const
{
	// 显示提示信息
	fillrectangle(750, 405, 950, 465);
	setfillcolor(bkColor);
	settextcolor(textColor);
	settextstyle(info_x, info_y, _T("宋体"));
	outtextxy(x, y, _T(info.c_str()));
	settextstyle(info_x2, info_y2, _T("宋体"));
	outtextxy(x2, y2, _T(info2.c_str()));
}

//绘制黑白方块
void Board::Draw_Block()const
{
	for (int row = 0; row < this->CELL_NUM; row++)
	{
		for (int col = 0; col < this->CELL_NUM; col++)
		{
			if(this->BK_Cellcolor[row][col] == 1)
				putimage(row * CELL_SIZE, col * CELL_SIZE, &black_cell);
			else
				putimage(row * CELL_SIZE, col * CELL_SIZE, &white_cell);
		}
	}
}

//绘制棋子
void Board::Draw_Chess()const
{
	for (int i = 0; i < 6; i++)
	{
		if (redChess[i].out == false)
		{
			putimage(this->redChess[i].x * this->CELL_SIZE + 5, redChess[i].y * this->CELL_SIZE + 5, &red[i]);
		}
		if (blueChess[i].out == false)
		{
			putimage(this->blueChess[i].x * this->CELL_SIZE + 5, blueChess[i].y * this->CELL_SIZE + 5, &blue[i]);
		}
	}
}

//绘制投骰子点数
void Board::Draw_DiceNumber(bool setOver, bool isRedTurn, int diceIndex)const
{
	if (setOver)
	{
		if (isRedTurn)
		{
			if (diceIndex == -1)
				putimage(755, 655, &reddice0);
			else
				putimage(755, 655, &reddice[diceIndex]);
		}
		else
		{
			if (diceIndex == -1)
				putimage(755, 655, &bluedice0);
			else
				putimage(755, 655, &bluedice[diceIndex]);
		}
	}
	else
	{
		putimage(755, 655, &dice0);
	}
}

//绘制轮数
void Board::Draw_TurnNum(int turnNumber)const
{
	//显示板第几轮
	putimage(750, 5, &number);

	// 显示第几轮
	this->Set_Text(840, 25, WHITE, BLACK, 50, 0, turnNumber);

}

//绘制双方时间
void Board::Draw_Time(int redTime, int blueTime)const
{
	//倒计时板显示
	putimage(750, 105, &timer);

	//红方时间
	this->Set_Text(885, 115, WHITE, RED, 25, 0, redTime);

	//蓝方时间
	this->Set_Text(885, 155, WHITE, BLUE, 25, 0, blueTime);
}

//绘制双方得分
void Board::Draw_Score(int redScore, int blueScore)const
{
	//红色得分板显示
	putimage(750, 205, &red_score);
	this->Set_Text(890, 220, WHITE, RED, 60, 0, redScore);

	//蓝色得分板显示
	putimage(750, 305, &blue_score);
	this->Set_Text(890, 320, WHITE, BLUE, 60, 0, blueScore);

}

//绘制提示信息
void Board::Draw_Info(std::string state, bool turnIsRedint, int diceChoiced_Index)const
{
	if (state == "EVE")															//EVE模式
	{
		this->Set_INFO(750, 410, WHITE, RED, 25, 0, "已开启自博弈模式",
			760, 440, 25, 0, "再次点击以暂停");
	}
	//else if (state == "warring")												//提示信息
	//{
	//	this->Set_INFO(755, 420, WHITE, BLACK, 32, 0, "超出点击范围");
	//}
	else if(state == "controlDice")												//控制骰子
	{
		if (diceChoiced_Index >= 0)
		{
			//将索引转换为编号再转换为字符串
			std::string choiced_Num = std::to_string(diceChoiced_Index + 1);

			// 显示提示信息
			this->Set_INFO(755, 410, WHITE, BLACK, 25, 14, "您选择的是：" + choiced_Num, 755, 440, 14, 0, "点击投掷骰子确认或者重新选择");
		}
		else
		{
			this->Set_INFO(760, 420, WHITE, BLACK, 25, 0, "请选择骰子点数");
		}
	}
	else if (state == "winner_is_red")
	{
		putimage(0, 75, &red_win);
	}
	else if (state == "winner_is_blue")
	{
		putimage(0, 75, &blue_win);
	}
	else
	{
		putimage(750, 405, &notice);
	}
}

//绘制玩家回合
void Board::Draw_Turn(bool turnIsRed, bool controlTurn)const
{
	if (controlTurn == false)
	{
		if (turnIsRed)												//红方回合
		{
			putimage(754, 613, &red_turn);
		}
		else														//蓝方回合
		{
			putimage(754, 613, &blue_turn);
		}
	}
}

//绘制小骰子
void Board::Draw_SmallDice(bool controlTurn, int over_MiniIndex, int dice_Choiced_Index)const
{

	//有点数被选中后就不显示悬浮效果
#if 0

	if (controlTurn)
	{
		if (dice_Choiced_Index < 0)																	//当没有选中点数时
		{
			if (over_MiniIndex >= 0)																//当鼠标悬浮在某一个点数上时
				putimage(755 + over_MiniIndex * 33, 613, &mini_choiced[over_MiniIndex]);			//将该点数变为黄色

			for (int i = 0; i < 6; i++)
			{
				if (i == over_MiniIndex)
					continue;
				putimage(755 + i * 33, 613, &mini[i]);												//其它点数为白色
			}
		}
		else																						//当选中某一点数时
		{
			putimage(755 + dice_Choiced_Index * 33, 613, &mini_choiced[dice_Choiced_Index]);		//将该点数变为黄色

			for (int i = 0; i < 6; i++)
			{
				if (i == dice_Choiced_Index)
					continue;
				putimage(755 + i * 33, 613, &mini[i]);									//其它点数为白色
			}
		}

	}
	else
	{
		return;
	}

#endif


	//有点数被选中后依然显示悬浮效果
#if 1

	if (controlTurn)
	{
		for (int i = 0; i < 6; i++)
		{
			if (dice_Choiced_Index == i)
			{
				putimage(755 + dice_Choiced_Index * 33, 613, &mini_choiced[dice_Choiced_Index]);
			}
			else if (over_MiniIndex == i)
			{
				putimage(755 + over_MiniIndex * 33, 613, &mini_choiced[over_MiniIndex]);
			}
			else
			{
				putimage(755 + i * 33, 613, &mini[i]);
			}
		}
	}
	else
	{
		return;
	}

#endif

}

//绘制可移动棋子背景
void Board::Draw_MoveableChess(bool turnIsRed, Chess redChess[6], Chess blueChess[6])
{
	if (turnIsRed)
	{
		for (int i = 0; i < 6; i++)
		{
			if (redChess[i].moveAble)
			{
				putimage(redChess[i].x * this->CELL_SIZE, redChess[i].y * this->CELL_SIZE, &choiced);
			}
		}
	}
	else
	{
		for (int i = 0; i < 6; i++)
		{
			if (blueChess[i].moveAble)
			{
				putimage(blueChess[i].x * this->CELL_SIZE, blueChess[i].y * this->CELL_SIZE, &choiced);
			}
		}
	}
}

//绘制棋子的可移动方向
void Board::Deaw_MovaablrDirection(bool turnIsRed, Chess redChess[6], Chess blueChess[6], int chess_Choiced_Index)
{
	if (chess_Choiced_Index == -1)
		return;
#if 0
	if (turnIsRed)
	{
		int x = redChess[chess_Choiced_Index].x; int y = redChess[chess_Choiced_Index].y;
		if (x + 1 <= 4)
		{
			if (this->BK_Cellcolor[x + 1][y] == 0) { putimage((x + 1) * this->CELL_SIZE, y * this->CELL_SIZE, &choiced); }
			else if (this->BK_Cellcolor[x + 1][y] == 1) { putimage((x + 1) * this->CELL_SIZE, y * this->CELL_SIZE, &choiced_black); }
		}
		if (x + 1 <= 4 && y + 1 <= 4)
		{
			if (this->BK_Cellcolor[x + 1][y + 1] == 0) { putimage((x + 1) * this->CELL_SIZE, (y + 1) * this->CELL_SIZE, &choiced); }
			else if (this->BK_Cellcolor[x + 1][y + 1] == 1) { putimage((x + 1) * this->CELL_SIZE, (y + 1) * this->CELL_SIZE, &choiced_black); }
		}
		if (y + 1 <= 4)
		{
			if (this->BK_Cellcolor[x][y + 1] == 0) { putimage(x * this->CELL_SIZE, (y + 1) * this->CELL_SIZE, &choiced); }
			else if (this->BK_Cellcolor[x][y + 1] == 1) { putimage(x * this->CELL_SIZE, (y + 1) * this->CELL_SIZE, &choiced_black); }
		}
	}
	else
	{
		int x = blueChess[chess_Choiced_Index].x; int y = blueChess[chess_Choiced_Index].y;
		if (x - 1 >= 0)
		{
			if (this->BK_Cellcolor[x - 1][y] == 0) { putimage((x - 1) * this->CELL_SIZE, y * this->CELL_SIZE, &choiced); }
			else if (this->BK_Cellcolor[x - 1][y] == 1) { putimage((x - 1) * this->CELL_SIZE, y * this->CELL_SIZE, &choiced_black); }
		}
		if (x - 1 >= 0 && y - 1 >= 0)
		{
			if (this->BK_Cellcolor[x - 1][y - 1] == 0) { putimage((x - 1) * this->CELL_SIZE, (y - 1) * this->CELL_SIZE, &choiced); }
			else if (this->BK_Cellcolor[x - 1][y - 1] == 1) { putimage((x - 1) * this->CELL_SIZE, (y - 1) * this->CELL_SIZE, &choiced_black); }
		}
		if (y - 1 >= 0)
		{
			if (this->BK_Cellcolor[x][y - 1] == 0) { putimage(x * this->CELL_SIZE, (y - 1) * this->CELL_SIZE, &choiced); }
			else if (this->BK_Cellcolor[x][y - 1] == 1) { putimage(x * this->CELL_SIZE, (y - 1) * this->CELL_SIZE, &choiced_black); }
		}
	}
#endif

#if 1

	if (turnIsRed)
	{
		//选择棋子后计算其可行域
		redChess[chess_Choiced_Index].FeasibleZone();

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				if (redChess[chess_Choiced_Index].feasibleZone[i][j] != -1)
				{
					int x = redChess[chess_Choiced_Index].feasibleZone[i][0];
					int y = redChess[chess_Choiced_Index].feasibleZone[i][1];

					if (this->BK_Cellcolor[x][y] == 0)
						putimage(x * this->CELL_SIZE, y * this->CELL_SIZE, &choiced);
					else
						putimage(x * this->CELL_SIZE, y * this->CELL_SIZE, &choiced_black);
				}
			}
		}
	}
	else
	{
		//选择棋子后计算其可行域
		blueChess[chess_Choiced_Index].FeasibleZone();

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				if (blueChess[chess_Choiced_Index].feasibleZone[i][j] != -1)
				{
					int x = blueChess[chess_Choiced_Index].feasibleZone[i][0];
					int y = blueChess[chess_Choiced_Index].feasibleZone[i][1];

					if (this->BK_Cellcolor[x][y] == 0)
						putimage(x * this->CELL_SIZE, y * this->CELL_SIZE, &choiced);
					else
						putimage(x * this->CELL_SIZE, y * this->CELL_SIZE, &choiced_black);
				}
			}
		}
	}
#endif
}

//绘制红色/蓝色按钮
void Board::Draw_PVP(bool PVP)const
{
	if (PVP)
	{
		putimage(853, 468, &pve);
		putimage(853, 541, &choiced_pvp);
	}
	else
	{
		putimage(853, 468, &choiced_pve);
		putimage(853, 541, &pvp);
	}
}

//绘制人机/双人按钮
void Board::Draw_HumanIsRed(bool HumanIsRed)const
{
	if (HumanIsRed)
	{
		putimage(753, 468, &red_choiced);
		putimage(753, 541, &blue_one);
	}
	else
	{
		putimage(753, 468, &red_one);
		putimage(753, 541, &blue_choiced);
	}
}

//绘制投掷骰子按钮(包含被点击测试)
void Board::Draw_RollDice(bool diceClickable, bool diceClicked, bool turnIsRed)const
{
	int button_dx = 853;
	int button_dy = 655;

	if (diceClicked)
	{
		if (turnIsRed)
		{
			putimage(button_dx, button_dy, &buttonred);
		}
		else
		{
			putimage(button_dx, button_dy, &buttonblue);
		}
	}
	else
	{
		putimage(button_dx, button_dy, &button);
	}
}

//绘制EVE按钮
void Board::Draw_EVE(bool EVE)const
{
	if (EVE)
	{
		putimage(950, 0, &EVE1);
	}
	else
	{
		putimage(950, 0, &EVE0);
	}
}

//绘制控制玩家点数按钮
void Board::Draw_ControlHumanDice(bool ConrtolHumanDice)const
{
	if (ConrtolHumanDice)
	{
		putimage(950, 255, &hand_in_self1);
	}
	else
	{
		putimage(950, 255, &hand_in_self0);
	}
}

//绘制控制电脑点数按钮
void Board::Draw_ControlAIDice(bool ControolAIDice)const
{
	if (ControolAIDice)
	{
		putimage(950, 505, &hand_in_mac1);
	}
	else
	{
		putimage(950, 505, &hand_in_mac0);
	}
}

//加载贴图
void Board::Load_pic()
{
	loadimage(&number, "res/number.png", 200, 95);
	loadimage(&timer, "res/time.png", 200, 95);
	loadimage(&red_score, "res/red_score.png", 200, 95);
	loadimage(&blue_score, "res/blue_score.png", 200, 95);
	loadimage(&buttonred, "res/buttonred.png", 95, 95);
	loadimage(&buttonblue, "res/buttonblue.png", 95, 95);
	loadimage(&button, "res/button.png", 95, 95);

	//红色骰子
	loadimage(&reddice0, "res/dice/reddice0.png", 95, 95);
	loadimage(&reddice[0], "res/dice/reddice1.png", 95, 95);
	loadimage(&reddice[1], "res/dice/reddice2.png", 95, 95);
	loadimage(&reddice[2], "res/dice/reddice3.png", 95, 95);
	loadimage(&reddice[3], "res/dice/reddice4.png", 95, 95);
	loadimage(&reddice[4], "res/dice/reddice5.png", 95, 95);
	loadimage(&reddice[5], "res/dice/reddice6.png", 95, 95);
	//蓝色骰子
	loadimage(&bluedice0, "res/dice/bluedice0.png", 95, 95);
	loadimage(&bluedice[0], "res/dice/bluedice1.png", 95, 95);
	loadimage(&bluedice[1], "res/dice/bluedice2.png", 95, 95);
	loadimage(&bluedice[2], "res/dice/bluedice3.png", 95, 95);
	loadimage(&bluedice[3], "res/dice/bluedice4.png", 95, 95);
	loadimage(&bluedice[4], "res/dice/bluedice5.png", 95, 95);
	loadimage(&bluedice[5], "res/dice/bluedice6.png", 95, 95);

	//mini骰子
	loadimage(&mini[0], "res/dice/mini_dice0.png");
	loadimage(&mini[1], "res/dice/mini_dice1.png");
	loadimage(&mini[2], "res/dice/mini_dice2.png");
	loadimage(&mini[3], "res/dice/mini_dice3.png");
	loadimage(&mini[4], "res/dice/mini_dice4.png");
	loadimage(&mini[5], "res/dice/mini_dice5.png");
	//mini选中骰子
	loadimage(&mini_choiced[0], "res/dice/mini_choiced_dice0.png");
	loadimage(&mini_choiced[1], "res/dice/mini_choiced_dice1.png");
	loadimage(&mini_choiced[2], "res/dice/mini_choiced_dice2.png");
	loadimage(&mini_choiced[3], "res/dice/mini_choiced_dice3.png");
	loadimage(&mini_choiced[4], "res/dice/mini_choiced_dice4.png");
	loadimage(&mini_choiced[5], "res/dice/mini_choiced_dice5.png");


	//初始骰子
	loadimage(&dice0, "res/dice/dice0.png", 95, 95);


	//棋子图片加载
	//红色棋子
	loadimage(&red[0], "res/chess/red1.png", 140, 140);
	loadimage(&red[1], "res/chess/red2.png", 140, 140);
	loadimage(&red[2], "res/chess/red3.png", 140, 140);
	loadimage(&red[3], "res/chess/red4.png", 140, 140);
	loadimage(&red[4], "res/chess/red5.png", 140, 140);
	loadimage(&red[5], "res/chess/red6.png", 140, 140);
	//蓝色棋子
	loadimage(&blue[0], "res/chess/blue1.png", 140, 140);
	loadimage(&blue[1], "res/chess/blue2.png", 140, 140);
	loadimage(&blue[2], "res/chess/blue3.png", 140, 140);
	loadimage(&blue[3], "res/chess/blue4.png", 140, 140);
	loadimage(&blue[4], "res/chess/blue5.png", 140, 140);
	loadimage(&blue[5], "res/chess/blue6.png", 140, 140);

	//落子棋子的颜色
	loadimage(&red_turn, "res/turn.red.png", 194, 40);
	loadimage(&blue_turn, "res/turn.blue.png", 194, 40);


	//显示胜利信息
	loadimage(&red_win, "res/win.red.png");
	loadimage(&blue_win, "res/win.blue.png");
	loadimage(&notice, "res/notice.png", 200, 60);


	//方块加载
	loadimage(&black_cell, "res/black_cell.png");
	loadimage(&white_cell, "res/white_cell.png");
	loadimage(&choiced, "res/choiced.png");
	loadimage(&choiced_black, "res/choiced_black.png");

	//控制图块
	loadimage(&pvp, "res/contral/player_pvp.png", 96, 70);
	loadimage(&pve, "res/contral/player_pve.png", 96, 70);
	loadimage(&red_one, "res/contral/player_red.png", 96, 70);
	loadimage(&blue_one, "res/contral/player_blue.png", 96, 70);
	loadimage(&red_choiced, "res/contral/player_choiced_red.png", 96, 70);
	loadimage(&blue_choiced, "res/contral/player_choiced_blue.png", 96, 70);
	loadimage(&choiced_pvp, "res/contral/player_choiced_pvp.png", 96, 70);
	loadimage(&choiced_pve, "res/contral/player_choiced_pve.png", 96, 70);

	//模式控制块加载
	loadimage(&hand_in_mac0, "res/hand_in_mac_dice0.png", 75, 245);
	loadimage(&hand_in_mac1, "res/hand_in_mac_dice1.png", 75, 245);
	loadimage(&hand_in_self0, "res/hand_in_self_dice0.png", 75, 245);
	loadimage(&hand_in_self1, "res/hand_in_self_dice1.png", 75, 245);
	loadimage(&EVE0, "res/EVE0.png");
	loadimage(&EVE1, "res/EVE1.png");

}