#include"Controller.h"

//默认构造方法
Controller::Controller()
{
	/*---游戏状态信息---*/
	mouseMsg = {0,0,0,0,0,0,0,0,0};										//鼠标消息
	x = -1, y = -1;														//坐标表示
	LbuttonNotDown = true;												//鼠标左键未被按下

	haveWinner = false;													//产生本局胜利者
	haveFinalWinner = false;											//产生最终胜利者

	initAll = false;													//初始化一切

	/*---游戏状态信息---*/
	state = "Normal";													//游戏状态
	turnNumber = 1;														//游戏轮数
	controlTurn = false;												//是否进入控制骰子状态
	setOver = false;													//棋子摆放是否完毕

	/*---骰子信息---*/
	diceClickAble = false;												//骰子可以被点击
	diceClicked = false;												//骰子被点击
	diceIndex = -1;														//骰子点数索引
	over_MiniIndex = -1;												//悬浮状态的鼠标下的小骰子点数
	dice_Choiced_Index = -1;											//鼠标点击后选中的小骰子点数


	/*---玩家信息---*/
	turnIsRed = true;													//红色玩家回合
	redTime = 240;														//红色玩家时间
	blueTime = 240;														//蓝色玩家时间
	redScore = 0;														//红色玩家分数
	blueScore = 0;														//蓝色玩家分数
	chess_Choiced_Index = -1;											//玩家选中的棋子索引

	/*---按钮信息---*/
	PVP = true;															//PVP模式
	HumanIsRed = true;													//人类是红色
	EVE = false;														//EVE模式
	controlHumanDice = false;											//控制玩家骰子
	controlAIDice = false;												//控制人类骰子
}


//获取鼠标消息
void Controller::GetMouseMsg_control(Board& board)
{
	//非阻塞鼠标消息获取
	PeekMouseMsg(&(this->mouseMsg));

	this->x = this->mouseMsg.x / board.CELL_SIZE;
	this->y = this->mouseMsg.y / board.CELL_SIZE;

	//当鼠标左键抬起时，鼠标左键未被按下这个变量才会变为真
	if (this->mouseMsg.uMsg == WM_LBUTTONUP)
	{
		this->LbuttonNotDown = true;
	}
}

//判断鼠标点击是否在某一区域内
bool Controller::MouseClickInArea(int area[2][2])const
{
	if (this->mouseMsg.x > area[0][0] && this->mouseMsg.y > area[0][1] &&
		this->mouseMsg.x < area[1][0] && this->mouseMsg.y < area[1][1])
		return true;
	else
		return false;
}

//获取并改变骰子状态
void Controller::GetDiceState(Dice &dice)
{
	dice.Over_MiniDice(this->mouseMsg.x, this->mouseMsg.y, this->controlTurn);
	dice.Choiced_Index(this->mouseMsg.uMsg == WM_LBUTTONDOWN, this->mouseMsg.x, this->mouseMsg.y);
	dice.IfDiceClicked(this->mouseMsg.uMsg, this->mouseMsg.x, this->mouseMsg.y);

	this->diceClickAble = dice.diceClickAble;
	this->diceClicked = dice.diceClicked;
	this->diceIndex = dice.diceIndex;
	this->over_MiniIndex = dice.over_MiniIndex;
	this->dice_Choiced_Index = dice.dice_Choiced_Index;
}

//获取并改变玩家状态
void Controller::GetPlayerState(Player& redPlayer, Player& bluePlayer, Dice& dice)
{
	this->turnIsRed = redPlayer.turn;
	this->redTime = redPlayer.time;
	this->blueTime = bluePlayer.time;
	this->redScore = redPlayer.score;
	this->blueScore = bluePlayer.score;
	
	if (redPlayer.turn == true)
	{
		this->chess_Choiced_Index = redPlayer.chess_Choiced_Index;
	}
	else
	{
		this->chess_Choiced_Index = bluePlayer.chess_Choiced_Index;
	}

	//利用等号运算符重载实现类的属性传递
	for (int i = 0; i < 6;i++)
	{
		this->redChess[i] = redPlayer.chess[i];
		this->blueChess[i] = bluePlayer.chess[i];
	}

	//本局产生胜利者
	if (redPlayer.winner || bluePlayer.winner)
	{
		this->haveWinner = true;
	}

}

//获取并改变按钮状态
void Controller::GetButtonState()
{
	if (this->mouseMsg.uMsg == WM_LBUTTONDOWN && this->LbuttonNotDown)
	{
		this->LbuttonNotDown = false;																//鼠标左键未被按下的变量值变为假

		if (this->MouseClickInArea(this->PVPArea))													//双人被点击
			this->PVP = true;
		else if (this->MouseClickInArea(this->PVEArea))												//人机被点击
			this->PVP = false;
		else if (this->MouseClickInArea(this->HumanIsRed_Area))										//红色被点击
			this->HumanIsRed = true;
		else if (this->MouseClickInArea(this->HumanIsBlue_Area))									//蓝色被点击
			this->HumanIsRed = false;
		else if (this->MouseClickInArea(this->ControlHumanDice_Area) && this->EVE == false)			//控制人类点数被点击
			this->controlHumanDice = !this->controlHumanDice;
		else if (this->MouseClickInArea(this->ControlComputerDice_Area) && this->EVE == false)		//控制电脑点数被点击
			this->controlAIDice = !this->controlAIDice;
		else if (this->MouseClickInArea(this->EVEArea))												//EVE被点击
		{
			this->EVE = !this->EVE;
			this->controlHumanDice = false;
			this->controlAIDice = false;
		}
		else
		{
			return;
		}
	}

	//EVE模式与其两个控制骰子点数的功能互斥
	if (this->EVE)
	{
		this->controlHumanDice = false;
		this->controlAIDice = false;
	}
}

//判断是否轮到骰子被控制的一方掷骰子
bool Controller::IfControlTurn(Player& redPlayer, Player& bluePlayer)const
{
	if (this->controlHumanDice && this->controlAIDice)								//如果双方的骰子都会被控制，则直接进入控制骰子状态
	{
		return true;
	}
	else if (this->controlHumanDice)												//如果控制人类点数
	{
		if (redPlayer.isAI == false && this->turnIsRed || bluePlayer.isAI == false && this->turnIsRed == false)
			return true;
		else
			return false;
	}
	else if (this->controlAIDice)													//如果控制机器点数
	{
		if (redPlayer.isAI && this->turnIsRed || bluePlayer.isAI && this->turnIsRed == false)
			return true;
	}
	else
		return false;
}

//获取对弈阶段的游戏状态
void Controller::GetFightGameState(Player& redPlayer, Player& bluePlayer)
{
	if (this->setOver)
	{
		//是否处于 EVE 状态
		if (this->EVE)
			this->state = "EVE";

		//是否处于控制骰子状态
		if (this->IfControlTurn(redPlayer, bluePlayer))
		{
			this->state = "controlDice";
			this->controlTurn = true;
		}
		else
		{
			this->state = "Normal";
			this->controlTurn = false;
		}
	}
}

//将信息同步给UI
void Controller::SyncToUI(Board& board)
{
	/*---游戏状态信息---*/
	board.Gamemsg.mouseMsg = this->mouseMsg;										//鼠标消息

	/*---游戏状态信息---*/
	board.Gamemsg.state = this->state;												//游戏状态
	board.Gamemsg.turnNumber = this->turnNumber;									//游戏轮数
	board.Gamemsg.controlTurn = this->controlTurn;									//是否进入控制骰子状态
	board.Gamemsg.setOVer = this->setOver;											//棋子摆放是否完毕

	/*---骰子信息---*/
	board.Gamemsg.diceClickAble = this->diceClickAble;								//骰子可以被点击
	board.Gamemsg.diceClicked = this->diceClicked;									//骰子被点击
	board.Gamemsg.diceIndex = this->diceIndex;										//骰子点数索引
	board.Gamemsg.over_MiniIndex = this->over_MiniIndex;							//悬浮状态的鼠标下的小骰子点数
	board.Gamemsg.dice_Choiced_Index = this->dice_Choiced_Index;								//鼠标点击后选中的小骰子点数

	/*---玩家信息---*/
	board.Gamemsg.turnIsRed = this->turnIsRed;										//红色玩家回合
	board.Gamemsg.redTime = this->redTime;											//红色玩家时间
	board.Gamemsg.blueTime = this->blueTime;										//蓝色玩家时间
	board.Gamemsg.redScore = this->redScore;										//红色玩家分数
	board.Gamemsg.blueScore = this->blueScore;										//蓝色玩家分数
	board.Gamemsg.chess_Choiced_Index = this->chess_Choiced_Index;					//玩家选中的棋子索引
	

	for (int i = 0; i < 6; i++)
	{
		board.redChess[i] = this->redChess[i];										//红方棋子信息
		board.blueChess[i] = this->blueChess[i];									//蓝方棋子信息
	}

	/*---按钮信息---*/
	board.Gamemsg.PVP = this->PVP;													//PVP模式
	board.Gamemsg.HumanIsRed = this->HumanIsRed;									//人类是红色
	board.Gamemsg.EVE = this->EVE;													//EVE模式
	board.Gamemsg.controlHumanDice = this->controlHumanDice;						//控制玩家骰子
	board.Gamemsg.controlAIDice = this->controlAIDice;								//控制人类骰子
}

//状态转换
void Controller::ChangeState(Player& redPlayer, Player& bluePlayer, Dice& dice)
{
	this->ChangeToRobot(redPlayer, bluePlayer);										//改变机器人属性

	//if (this->PVP)
	//	return;
	//else
	//{
	//	if (this->setOver)
	//	{
	//		if (this->state == "controlDice")
	//		{

	//		}
	//	}
	//}
}

//单局胜利判断 (同时改变 controller 里面的 haveWinner)
bool Controller::HaveWinner(Player& redPlayer, Player& bluePlayer)
{
	//运行检测函数
	if (this->turnIsRed)
	{
		redPlayer.WinOnce(redPlayer, bluePlayer);

		if (redPlayer.winner)
		{
			this->haveWinner = true;
			return true;
		}
	}
	else
	{
		bluePlayer.WinOnce(redPlayer, bluePlayer);

		if (bluePlayer.winner)
		{
			this->haveWinner = true;
			return true;
		}
	}

	return false;
}

//开始新的一局，初始化信息 (产生胜利者时才会被执行)
void Controller::initGame(Player& redPlayer, Player& bluePlayer)
{

	if (this->HaveWinner(redPlayer, bluePlayer))
	{
		//初始化先手后手
		// 1、4、5、局红方优先//2、3、6、7局蓝方优先
		int turns = redPlayer.score + bluePlayer.score;
		if (turns == 1 || turns == 4 || turns == 5)
		{
			redPlayer.turn = redPlayer.Red;
		}
		else
		{
			redPlayer.turn = redPlayer.Blue;
		}

		redPlayer.initPlayer();
		bluePlayer.initPlayer();
		this->initController(redPlayer, bluePlayer);
	}
}

//进入选择界面
void Controller::ChoiceToExit_Replay(Player& redPlayer, Player& bluePlayer, Board& board, bool& running)
{
	//暂停在选择界面
	if (redPlayer.score >= 4)
	{
		this->state = "winner_is_red";
		board.Gamemsg.state = this->state;
	}
	else if (bluePlayer.score >= 4)
	{
		this->state = "winner_is_blue";
		board.Gamemsg.state = this->state;
	}

	//进行选择
	if (this->mouseMsg.uMsg == this->LbuttonNotDown)
	{
		running = false;															//单机左键退出游戏
	}
	else if (this->mouseMsg.uMsg == this->mouseRightDown)							//单击右键重开
	{
		redPlayer.score = 0;
		bluePlayer.score = 0;
		this->turnNumber = 0;
		this->haveFinalWinner = false;
		this->state = "Normal";
	}
}

//最终胜利判断 (同时改变 controller 里面的 haveFinalWinner)
bool Controller::HaveFinalWinner(Player& redPlayer, Player& bluePlayer, Board& board)
{
	if (redPlayer.score >= 4)
	{
		this->haveFinalWinner = true;
		this->state = "winner_is_red";
		board.Gamemsg.state = this->state;										//将胜利者信息及时同步到UI

		return true;
	}
	else if (bluePlayer.score >= 4)
	{
		this->haveFinalWinner = true;
		this->state = "winner_is_blue";
		board.Gamemsg.state = this->state;										//将胜利者信息及时同步到UI

		return true;
	}
	else
	{
		return false;
	}
}

//初始化一切(最终胜利者产生后，进行选择)(只有产生最终胜利者后才被执行)
void Controller::initAllObject(Player& redPlayer, Player& bluePlayer, Board& board, bool& running)
{
	//初始化一局游戏 (只有产生胜利者时才被执行)
	this->initGame(redPlayer, bluePlayer);

	//产生最终胜利者后，显示胜利界面时进行选择
	if (this->HaveFinalWinner(redPlayer, bluePlayer, board))
	{
		this->ChoiceToExit_Replay(redPlayer, bluePlayer, board, running);
	}
	else
	{
		return;
	}
}

//初始化方法
void Controller::initController(Player& redPlayer, Player& bluePlayer)
{
	/*---游戏状态信息---*/
	this->mouseMsg = { 0,0,0,0,0,0,0,0,0 };										//鼠标消息
	this->x = -1, this->y = -1;													//坐标表示
	this->LbuttonNotDown = true;												//鼠标左键未被按下

	this->haveWinner = false;													//产生本局胜利者
	//this->haveFinalWinner = false;												//产生最终胜利者

	/*---游戏状态信息---*/
	this->turnNumber = redPlayer.score + bluePlayer.score + 1;					//游戏轮数

	//this->state = "Normal";														//游戏状态
	//this->controlTurn = false;													//是否进入控制骰子状态
	this->setOver = false;														//棋子摆放是否完毕

	/*---骰子信息---*/
	this->diceClickAble = false;												//骰子可以被点击
	this->diceClicked = false;													//骰子被点击
	this->diceIndex = -1;														//骰子点数索引
	this->over_MiniIndex = -1;													//悬浮状态的鼠标下的小骰子点数
	this->dice_Choiced_Index = -1;												//鼠标点击后选中的小骰子点数


	/*---玩家信息---*/
	this->redScore = redPlayer.score;											//红色玩家分数
	this->blueScore = bluePlayer.score;											//蓝色玩家分数
	this->turnIsRed = redPlayer.turn;											//红色玩家回合

	this->redTime = 240;														//红色玩家时间
	this->blueTime = 240;														//蓝色玩家时间
	this->chess_Choiced_Index = -1;												//玩家选中的棋子索引


	///*---按钮信息{保持不变}---*/
	//this->PVP = true;															//PVP模式
	//this->HumanIsRed = true;													//人类是红色
	//this->EVE = false;														//EVE模式
	//this->controlHumanDice = false;											//控制玩家骰子
	//this->controlAIDice = false;												//控制人类骰子
}


//根据按钮改变玩家是否是机器人的方法
void Controller::ChangeToRobot(Player& redPlayer, Player& bluePlayer)
{
	if (this->EVE == false)
	{
		if (this->PVP == false)														//PVE模式
		{
			if (this->HumanIsRed)
			{
				redPlayer.isAI = false;
				bluePlayer.isAI = true;
			}
			else
			{
				redPlayer.isAI = true;
				bluePlayer.isAI = false;
			}
		}
		else																		//PVP模式
		{
			redPlayer.isAI = false;
			bluePlayer.isAI = false;
		}
		
	}
	else																			//EVE模式
	{
		redPlayer.isAI = true;
		bluePlayer.isAI = true;
	}

}


//摆放棋子(新)
void Controller::NewSetChess(Player& redPlayer, Player& bluePlayer, Dice& dice)
{
	if (this->mouseMsg.uMsg == WM_LBUTTONDOWN && this->setOver == false && this->haveFinalWinner == false)
	{
		if (this->turnIsRed)
		{
			if (redPlayer.isAI)
			{
				redPlayer.ai.AutoSetChess(redPlayer);
				redPlayer.ChangeTurn(redPlayer, bluePlayer, dice);
			}
			else
			{
				redPlayer.SetChess(this->x, this->y, dice);
			}
		}
		else
		{
			if (bluePlayer.isAI)
			{
				bluePlayer.ai.AutoSetChess(bluePlayer);
				redPlayer.ChangeTurn(redPlayer, bluePlayer, dice);
			}
			else
			{
				bluePlayer.SetChess(this->x, this->y, dice);
			}
		}

		//双方棋子都摆放完毕则全局摆放完毕
		if (redPlayer.setOver && bluePlayer.setOver)
			this->setOver = true;
	}

	return;
}


//对弈(新)
void Controller::NewFight(Player& redPlayer, Player& bluePlayer, Dice& dice)
{
	NewSetChess(redPlayer, bluePlayer, dice);

	if (this->setOver && this->haveFinalWinner == false)
	{
		bool lbuttonDown = false;
		if (this->mouseMsg.uMsg == WM_LBUTTONDOWN)
			lbuttonDown = true;

		redPlayer.Fight(bluePlayer, dice, this->controlAIDice, this->controlHumanDice, this->x, this->y, lbuttonDown);
		bluePlayer.Fight(redPlayer, dice, this->controlAIDice, this->controlHumanDice, this->x, this->y, lbuttonDown);

		//if (this->turnIsRed)
		//{
		//	if (redPlayer.isAI)
		//	{
		//		if (dice.diceClickAble)
		//		{
		//			//掷骰子
		//			if (this->controlAIDice)
		//			{
		//				//控制掷骰子办法
		//				dice.SyncToBigDice();
		//			}
		//			else
		//			{
		//				redPlayer.RollDice(dice);
		//			}

		//			//计算可移动棋子索引
		//			redPlayer.ConputMoveAbleChessIndex(dice.diceIndex);
		//		}

		//		//选择可将要移动的棋子及其移动方向并移动棋子
		//		redPlayer.ai.AutoChoiceChess_DirectionAndMoving(redPlayer, bluePlayer, dice);
		//	}
		//	else
		//	{
		//		if (this->controlHumanDice)
		//		{
		//			//控制投掷骰子方法
		//			dice.SyncToBigDice();
		//		}
		//		else
		//		{
		//			if (dice.diceClicked)
		//			{
		//				redPlayer.RollDice(dice);
		//			}
		//		}
		//		redPlayer.ConputMoveAbleChessIndex(dice.diceIndex);
		//		redPlayer.ChoiceChess(this->x, this->y);
		//		redPlayer.MoveChess(this->x, this->y, bluePlayer, dice);
		//	}

		//}
		//else
		//{
		//	if (bluePlayer.isAI)
		//	{
		//		if (dice.diceClickAble)
		//		{
		//			if (this->controlAIDice)
		//			{
		//				//控制投掷骰子方法
		//				dice.SyncToBigDice();
		//			}
		//			else
		//			{
		//				bluePlayer.RollDice(dice);
		//			}
		//			bluePlayer.ConputMoveAbleChessIndex(dice.diceIndex);
		//		}
		//		//AI移动棋子
		//		bluePlayer.ai.AutoChoiceChess_DirectionAndMoving(bluePlayer, redPlayer, dice);
		//	}
		//	else
		//	{
		//		if (this->controlHumanDice)
		//		{
		//			//控制投掷骰子方法
		//			dice.SyncToBigDice();
		//		}
		//		else
		//		{
		//			if (dice.diceClicked)
		//			{
		//				bluePlayer.RollDice(dice);
		//			}
		//		}

		//		bluePlayer.ConputMoveAbleChessIndex(dice.diceIndex);
		//		bluePlayer.ChoiceChess(this->x, this->y);
		//		bluePlayer.MoveChess(this->x, this->y, redPlayer, dice);
		//	}
		//}
	}
}



/*---------------------------------------可能会被弃用的方法--------------------------------------------------*/
#if 0
//判断放置棋子阶段的游戏状态
std::string Controller::SetChessGameState(Player& redPlayer, Player& bluePlayer, Dice& dice)
{
	if (this->EVE == false)
	{
		if (this->setOver == false)														//摆放阶段是否完毕的判断
		{
			if (redPlayer.setOver && bluePlayer.setOver)
			{
				this->setOver = true;
			}
			else if (this->turnIsRed && redPlayer.setOver == false)
			{
				redPlayer.SetChess(this->x, this->y, dice);
			}
			else if (this->turnIsRed == false && bluePlayer.setOver == false)
			{
				bluePlayer.SetChess(this->x, this->y, dice);
			}
		}

		return "Normal";
	}
	else
	{
		if (this->setOver == false)
		{
			if (this->turnIsRed && redPlayer.setOver == false)
			{
				//redPlayer.AutoSetChess(redPlayer, bluePlayer, dice);
				redPlayer.ai.AutoSetChess(redPlayer);
				redPlayer.ChangeTurn(redPlayer, bluePlayer, dice);
			}
			else if (this->turnIsRed == false && bluePlayer.setOver == false)
			{
				//bluePlayer.AutoSetChess(redPlayer, bluePlayer, dice);
				bluePlayer.ai.AutoSetChess(bluePlayer);
				bluePlayer.ChangeTurn(redPlayer, bluePlayer, dice);
			}
		}

		return "EVE";
	}
}

//对弈
void Controller::Fight(Player& redPlayer, Player& bluePlayer, Dice& dice)
{
	if (this->setOver)
	{
		if (this->turnIsRed)
		{
			int choicedIndex = -1;
			if (this->diceClicked)
			{
				redPlayer.RollDice(dice);
				redPlayer.ConputMoveAbleChessIndex(dice.diceIndex);
			}
			redPlayer.ChoiceChess(this->x, this->y);
			redPlayer.MoveChess(this->x, this->y, bluePlayer, dice);
		}
		else
		{
			int choicedIndex = -1;
			if (this->diceClicked)
			{
				bluePlayer.RollDice(dice);
				bluePlayer.ConputMoveAbleChessIndex(dice.diceIndex);
			}
			bluePlayer.ChoiceChess(this->x, this->y);
			bluePlayer.MoveChess(this->x, this->y, redPlayer, dice);
		}
	}
}
#endif