#include"Player.h"

bool Player::turn;

//带参构造函数
Player::Player(bool color_, int totalChessNumber_, bool turn_, int time_, int score_)
{
	this->color = color_;

	for (int i = 0; i < 6; i++)
		this->chess[i] = Chess(this->color, i, -1, -1, true, false, false);
	
	this->totalChessNumber = totalChessNumber_;
	this->setOver = false;
	this->chess_Choiced_Index = -1;
	this->turn = turn_;
	this->isAI = false;
	this->time = time_;
	this->winner = false;
	this->score = score_;
}

//棋子是否在可放置区域
bool Player::InSetArea(bool color, int x, int y)
{
	switch (color)
	{
	case this->Red:
		for (int i = 0;i < 6; i++)
		{
			if (x == this->redArea[i][0] && y == this->redArea[i][1])
			{
				return true;
			}
		}

		break;

	case this->Blue:
		for (int i = 0;i < 6; i++)
		{
			if (x == this->blueArea[i][0] && y == this->blueArea[i][1])
			{
				return true;
			}
		}

		break;
	}
	return false;
}

//判断目标位置是否有友方单位
bool Player::HaveFriedflyUnit(int x, int y)
{
	for (int i = 0; i < 6; i++)
	{
		if (this->chess[i].out == false)
		{
			if (this->chess[i].x == x && this->chess[i].y == y)
				return true;
		}
	}
	return false;
}

//摆放棋子
void Player::SetChess(int x, int y, Dice& dice)
{
	if (this->turn == this->color)
	{
		if (this->color == this->Red && this->setOver == false)
		{
			if (this->InSetArea(this->color, x, y) && this->HaveFriedflyUnit(x, y) == false)
			{
				this->chess[this->totalChessNumber].x = x;
				this->chess[this->totalChessNumber].y = y;
				this->chess[this->totalChessNumber].out = false;
				this->totalChessNumber++;

				if (this->totalChessNumber == 6)
				{
					this->setOver = true;
					this->turn = !this->turn;
					dice.diceClickAble = true;
				}
				else
				{
					dice.diceClickAble = false;
				}

				return;
			}
		}
		else if (this->color == this->Blue && this->setOver == false)
		{
			if (this->InSetArea(this->color, x, y) && this->HaveFriedflyUnit(x, y) == false)
			{
				this->chess[this->totalChessNumber].x = x;
				this->chess[this->totalChessNumber].y = y;
				this->chess[this->totalChessNumber].out = false;
				this->totalChessNumber++;

				if (this->totalChessNumber == 6)
				{
					this->setOver = true;
					this->turn = !this->turn;
					dice.diceClickAble = true;
				}
				else
				{
					dice.diceClickAble = false;
				}

				return;
			}
		}
	}
	return;
}

//交换回合
void Player::ChangeTurn(Player& redPlayer, Player& bluePlayer, Dice& dice)
{
	//更改玩家信息
	redPlayer.chess_Choiced_Index = -1;		//玩家选中的棋子编号
	bluePlayer.chess_Choiced_Index = -1;

	//双方棋子的可移动情况和被选中情况
	for (int i = 0; i < 6; i++)
	{
		if (redPlayer.chess[i].out == false)
		{
			redPlayer.chess[i].moveAble = false;
			redPlayer.chess[i].choiced = false;
		}

		if (bluePlayer.chess[i].out == false)
		{
			bluePlayer.chess[i].moveAble = false;
			bluePlayer.chess[i].choiced = false;
		}
	}

	//骰子初始化
	dice.InitDice();

	//进入对战过程后，每次交换回合时，骰子可以被点击一次
	if (redPlayer.setOver && bluePlayer.setOver)
	{
		dice.diceClickAble = true;

		//判断本局是否产生胜利者
		this->WinOnce(redPlayer, bluePlayer);

	}
	this->turn = !this->turn;				//交换回合
}

//投掷骰子
void Player::RollDice(Dice& dice)
{
	if (this->isAI)
	{
		dice.diceClicked = true;;
		dice.RollDice();
		dice.diceClicked = false;
	}
	else
	{
		dice.RollDice();
	}
}

//计算可移动棋子的编号
void Player::ConputMoveAbleChessIndex(int diceNumber)
{
	if (diceNumber == -1)
		return;

	bool finded = false;

	for (int i = 0; i <= 5; i++)
	{
		int lower = diceNumber - i;
		int upper = diceNumber + i;

		if (lower >= 0 && this->chess[lower].out == false)
		{
			this->chess[lower].moveAble = true;
			finded = true;
		}
		if (upper <= 5 && this->chess[upper].out == false)
		{
			this->chess[upper].moveAble = true;
			finded = true;
		}

		if (finded)
			return;
	}
}

//选择将要移动的棋子
void Player::ChoiceChess(int x, int y)
{
	//将被点击的棋子设置为被选中状态
	for (int i = 0; i < 6; i++)
	{
		if (this->chess[i].out == false && this->chess[i].moveAble == true)
		{
			if (x == this->chess[i].x && y == this->chess[i].y)
			{
				this->chess[i].choiced = true;
				this->chess_Choiced_Index = i;
			}
		}
	}

	//其它棋子的被选中状态设置为 false
	for (int i = 0; i < 6; i++)
	{
		if (this->chess[i].out == false && i != this->chess_Choiced_Index)
		{
			this->chess[i].choiced = false;
		}
	}

	return;
}

//确定移动方向
int Player::ChoiceDirection(int chessIndex, int x, int y)
{
	int direction = -1;

	if (this->color == this->Red)
	{
		if (this->chess[chessIndex].x == x - 1 && this->chess[chessIndex].y == y)
		{
			direction = this->Rx;
		}
		else if (this->chess[chessIndex].x == x - 1 && this->chess[chessIndex].y == y - 1)
		{
			direction = this->Rxy;
		}
		else if (this->chess[chessIndex].x == x && this->chess[chessIndex].y == y - 1)
		{
			direction = Ry;
		}

		return direction;
	}
	else
	{
		if (this->chess[chessIndex].x == x + 1 && this->chess[chessIndex].y == y)
		{
			direction = this->Bx;
		}
		else if (this->chess[chessIndex].x == x + 1 && this->chess[chessIndex].y == y + 1)
		{
			direction = this->Bxy;
		}
		else if (this->chess[chessIndex].x == x && this->chess[chessIndex].y == y + 1)
		{
			direction = By;
		}

		return direction;
	}
}

//判断移动方向是否合法
bool Player::RightDirection(int chessIndex, int direction)
{
	if (this->color == this->Red)
	{
		if (direction == this->Rx)
		{
			if (this->chess[chessIndex].x + 1 > 4)
				return false;
			else
				return true;
		}
		else if (direction == this->Rxy)
		{
			if (this->chess[chessIndex].x + 1 > 4 || this->chess[chessIndex].y + 1 > 4)
				return false;
			else
				return true;
		}
		else if (direction == this->Ry)
		{
			if (this->chess[chessIndex].y + 1 > 4)
				return false;
			else
				return true;
		}
		else
			return false;
	}
	else
	{
		if (direction == this->Bx)
		{
			if (this->chess[chessIndex].x - 1 < 0)
				return false;
			else
				return true;
		}
		else if (direction == this->Bxy)
		{
			if (this->chess[chessIndex].x - 1 < 0 || this->chess[chessIndex].y - 1 < 0)
				return false;
			else
				return true;
		}
		else if (direction == this->By)
		{
			if (this->chess[chessIndex].y - 1 < 0)
				return false;
			else
				return true;
		}
		else
			return false;
	}
}

//移动棋子
void Player::MoveChess(int x, int y, Player& otherPlayer, Dice& dice)
{
	int direction = ChoiceDirection(this->chess_Choiced_Index, x, y);

	if (RightDirection(this->chess_Choiced_Index, direction))
	{
		for (int i = 0; i < 6; i++)
		{
			if (otherPlayer.chess[i].out == false && otherPlayer.chess[i].x == x && otherPlayer.chess[i].y == y)
			{
				otherPlayer.chess[i].out = true;
				otherPlayer.chess[i].x = -1;
				otherPlayer.chess[i].y = -1;
			}
			else if (this->chess[i].out == false && this->chess[i].x == x && this->chess[i].y == y)
			{
				this->chess[i].out = true;
				this->chess[i].x = -1;
				this->chess[i].y = -1;
			}
		}

		this->chess[this->chess_Choiced_Index].x = x;
		this->chess[this->chess_Choiced_Index].y = y;
		this->chess[this->chess_Choiced_Index].moveAble = false;
		this->chess[this->chess_Choiced_Index].choiced = false;

		if (this->color == this->Red)
			this->ChangeTurn(*this, otherPlayer, dice);
		else
			this->ChangeTurn(otherPlayer, *this, dice);
	}
}

//对弈方法
void Player::Fight(Player& otherPlayer, Dice& dice, bool controlAIDice, bool controlHumanDice, int x_CELL_SIZE, int y_CELL_SIZE, bool LbuttonDown)
{
	if (this->turn == this->color)
	{
		if (this->isAI)
		{
			//LbuttonDown = true;				//当轮到 AI 的回合，鼠标左键点击始终为真
			if (dice.diceClickAble)
			{
				//掷骰子
				if (controlAIDice)
				{
					//控制掷骰子办法
					dice.SyncToBigDice();
				}
				else
				{
					//手动投掷骰子
					this->RollDice(dice);
				}

				//计算可移动棋子索引
				this->ConputMoveAbleChessIndex(dice.diceIndex);
			}

			//选择可将要移动的棋子及其移动方向并移动棋子
			this->ai.AutoChoiceChess_DirectionAndMoving(*this, otherPlayer, dice);
		}
		else
		{
			if (controlHumanDice)
			{
				//控制投掷骰子方法
				dice.SyncToBigDice();
			}
			else
			{
				//手动投掷骰子
				if (dice.diceClicked)
				{
					this->RollDice(dice);
				}
			}
			this->ConputMoveAbleChessIndex(dice.diceIndex);
			if (LbuttonDown)
			{
				if (dice.diceIndex != -1)
				{
					this->ChoiceChess(x_CELL_SIZE, y_CELL_SIZE);
				}

				if (this->chess_Choiced_Index != -1)
				{
					this->MoveChess(x_CELL_SIZE, y_CELL_SIZE, otherPlayer, dice);
				}
			}
		}

	}
}

//计算己方剩余棋子数量
void Player::ConputTotalChessNumber()
{
	int num = 0;

	for (int i = 0; i < 6; i++)
	{
		if (this->chess[i].out == false)
			num++;
	}

	this->totalChessNumber = num;
}

//一局胜利判断(在交换回合之前检测)
void Player::WinOnce(Player& redPlayer, Player& bluePlayer)
{
	//1.到达对方角点
	if (this->turn == Red)
	{
		for (int i = 0; i < 6;i++)
		{
			if (redPlayer.chess[i].x == 4 && redPlayer.chess[i].y == 4)
			{
				redPlayer.winner = true;
				redPlayer.score++;

				printf("redwin     1\n");
				return;
			}
		}
	}
	else if(this->turn == Blue)
	{
		for (int i = 0; i < 6;i++)
		{
			if (bluePlayer.chess[i].x == 0 && bluePlayer.chess[i].y == 0)
			{
				bluePlayer.winner = true;
				bluePlayer.score++;

				printf("bluewin     1\n");

				return;
			}
		}
	}
	

	//2.对方棋子数量为0 (在摆放棋子完毕的情况下)
	if (redPlayer.setOver && bluePlayer.setOver)
	{
		if (this->turn == Red)
		{
			bluePlayer.ConputTotalChessNumber();
			if (bluePlayer.totalChessNumber == 0)
			{
				redPlayer.winner = true;
				redPlayer.score++;

				printf("redwin     2\n");

				return;
			}
		}
		else if (this->turn == Blue)
		{
			redPlayer.ConputTotalChessNumber();
			if (redPlayer.totalChessNumber == 0)
			{
				bluePlayer.winner = true;
				bluePlayer.score++;
				
				printf("bluewin     2\n");

				return;
			}
		}
	}


	//3.对方时间用尽
	if (this->turn == Red)
	{
		if (bluePlayer.time <= 0)
		{
			redPlayer.winner = true;
			redPlayer.score++;

			printf("redwin     3\n");

			return;
		}
	}
	else if (this->turn == Blue)
	{
		if(redPlayer.time <= 0)
		{
			bluePlayer.winner = true;
			bluePlayer.score++;

			printf("bluewin     3\n");
			return;
		}
	}

}

//初始化方法
void Player::initPlayer()
{
	for (int i = 0; i < 6; i++)														//初始化棋子
		this->chess[i].initChess();

	this->totalChessNumber = 0;														//初始化棋子总数
	this->setOver = false;															//未摆放完毕
	this->chess_Choiced_Index = -1;													//未选中棋子
	this->time = 240;																//时间满格
	this->winner = false;															//不再是赢家

	//只有颜色{color}、回合{turn}、分数{score}需要保留
}



/*-----------------------因AI而被放弃的方法---------------------------*/
#if 0
//自动摆放棋子
void Player::AutoSetChess(Player& redPlayer, Player& bluePlayer, Dice& dice)
{
	if (this->turn == this->color)
	{
		if (this->color == this->Red)
		{
			this->chess[0].x = 0; this->chess[0].y = 0; this->chess[0].out = false;
			this->chess[1].x = 1; this->chess[1].y = 0; this->chess[1].out = false;
			this->chess[2].x = 1; this->chess[2].y = 1; this->chess[2].out = false;
			this->chess[3].x = 0; this->chess[3].y = 1; this->chess[3].out = false;
			this->chess[4].x = 0; this->chess[4].y = 2; this->chess[4].out = false;
			this->chess[5].x = 2; this->chess[5].y = 0; this->chess[5].out = false;

			this->setOver = true;
			this->ChangeTurn(redPlayer, bluePlayer, dice);
		}
		else
		{
			this->chess[0].x = 4; this->chess[0].y = 4; this->chess[0].out = false;
			this->chess[1].x = 3; this->chess[1].y = 4; this->chess[1].out = false;
			this->chess[2].x = 3; this->chess[2].y = 3; this->chess[2].out = false;
			this->chess[3].x = 4; this->chess[3].y = 3; this->chess[3].out = false;
			this->chess[4].x = 4; this->chess[4].y = 2; this->chess[4].out = false;
			this->chess[5].x = 2; this->chess[5].y = 4; this->chess[5].out = false;

			this->setOver = true;
			this->ChangeTurn(redPlayer, bluePlayer, dice);
		}
	}

	this->setOver = true;
}

#endif