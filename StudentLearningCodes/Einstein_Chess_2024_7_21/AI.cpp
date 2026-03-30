#include"AI.h"
#include"Player.h"
#include<stack>
#include<vector>


//棋盘节点创键
AI::BoardNode* AI::initBoardNode(Chess* rootChess_, int chess_MoveAble_Index_, int(*winNumber_)[3], int(*loseNumber_)[3], Chess chessBoard_[5][5])
{
	BoardNode* newNode = new BoardNode;		//创键新节点

	newNode->rootChess = rootChess_;		//赋值根节点
	newNode->parentPtr = newNode;			//父节点传递自己的

	newNode->Chess_Moveable_index = chess_MoveAble_Index_;

	//复制上一局移动过后的棋盘状态
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			newNode->chessBoard[i][j] = chessBoard_[i][j];
		}
	}

	newNode->winNumber = winNumber_;
	newNode->loseNumber = loseNumber_;

	return newNode;
}


/*-----被外界调用的方法-----*/

//析构函数,回收创键节点时占用的空间
AI::~AI()
{

}

//自动摆放棋子
void AI::AutoSetChess(Player& player)
{
	if (player.color == player.Red)
	{
		player.chess[0].x = 0; player.chess[0].y = 0; player.chess[0].out = false;
		player.chess[1].x = 1; player.chess[1].y = 0; player.chess[1].out = false;
		player.chess[2].x = 1; player.chess[2].y = 1; player.chess[2].out = false;
		player.chess[3].x = 0; player.chess[3].y = 1; player.chess[3].out = false;
		player.chess[4].x = 0; player.chess[4].y = 2; player.chess[4].out = false;
		player.chess[5].x = 2; player.chess[5].y = 0; player.chess[5].out = false;
		
		player.setOver = true;
	}
	else
	{
		player.chess[0].x = 4; player.chess[0].y = 4; player.chess[0].out = false;
		player.chess[1].x = 3; player.chess[1].y = 4; player.chess[1].out = false;
		player.chess[2].x = 3; player.chess[2].y = 3; player.chess[2].out = false;
		player.chess[3].x = 4; player.chess[3].y = 3; player.chess[3].out = false;
		player.chess[4].x = 4; player.chess[4].y = 2; player.chess[4].out = false;
		player.chess[5].x = 2; player.chess[5].y = 4; player.chess[5].out = false;
		
		player.setOver = true;
	}
}

//自动选择棋子及其移动方向 ( 它不需要主动交换回合 )
void AI::AutoChoiceChess_DirectionAndMoving(Player& myPlayer, Player& otherPlayer, Dice& dice)
{
	this->GetChessBoard(myPlayer, otherPlayer);										//获取棋盘状态

#if 1
	//找到被标记为可移动的棋子并将
	int totalMoveableNum = 0;
	std::vector<int>moveAbleChessIndex;
	for (int i = 0; i < 6; i++)
	{
		if (myPlayer.chess[i].moveAble)
		{
			this->CreateAllNodes(myPlayer.chess[i], totalMoveableNum);
			moveAbleChessIndex.push_back(i);
			totalMoveableNum++;
		}
	}

	//根据自身颜色确定移动哪一个，以及向哪一个方向移动
	int chessIndex = 0, direction = 0;
	int max = 0;
	for (int i = 0; i < totalMoveableNum; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (this->winNumber[i][j] > max)
			{
				max = this->winNumber[i][j];
				chessIndex = moveAbleChessIndex[i];
				direction = j;
			}
		}
	}

	//选中并移动棋子
	myPlayer.ChoiceChess(myPlayer.chess[chessIndex].x, myPlayer.chess[chessIndex].y);
	Dice dice_temporary;
	myPlayer.chess[chessIndex].FeasibleZone();
	int x = myPlayer.chess[chessIndex].feasibleZone[direction][0];
	int y = myPlayer.chess[chessIndex].feasibleZone[direction][1];

	myPlayer.MoveChess(x, y, otherPlayer, dice_temporary);
#endif

#if 0
	/*测试*/

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5;j++)
		{
			printf("%4d", this->colorBoard[i][j] + 1);
		}
		printf("\n");
	}
	printf("\n\n\n\n\n");


	for (int i = 0; i < 6; i++)
	{
		if (myPlayer.chess[i].moveAble)
		{
			myPlayer.ChoiceChess(myPlayer.chess[i].x, myPlayer.chess[i].y);

			myPlayer.chess[i].FeasibleZone();

			for (int j = 0; j < 3; j++)
			{
				if (myPlayer.chess[i].feasibleZone[j][0] != -1)
				{
					int x = myPlayer.chess[i].feasibleZone[j][0];
					int y = myPlayer.chess[i].feasibleZone[j][1];

					myPlayer.chess[i].initFeasibleZone();

					//printf("\ncolor = %d	chess = %d	(%d,%d)\n\n", myPlayer.color, i + 1, x, y);

					myPlayer.MoveChess(x, y, otherPlayer, dice);

					myPlayer.chess[i].choiced = false;
					myPlayer.chess_Choiced_Index = -1;

					break;
				}
			}

			break;
		}
	}
#endif

}



/*-----内部使用的方法-----*/

//获取棋局状态
void AI::GetChessBoard(Player& myPlayer, Player& otherPlayer)
{
	//初始化棋盘
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			this->chessBoard[i][j].initChess();
			this->chessBoard[i][j].initFeasibleZone();

			this->colorBoard[i][j] = -1;
		}
	}

	for (int i = 0; i < 6; i++)
	{
		if (myPlayer.chess[i].out == false)
		{
			int x = myPlayer.chess[i].x;
			int y = myPlayer.chess[i].y;

			this->chessBoard[y][x] = myPlayer.chess[i];
			this->colorBoard[y][x] = myPlayer.color;
		}

		if (otherPlayer.chess[i].out == false)
		{
			int x = otherPlayer.chess[i].x;
			int y = otherPlayer.chess[i].y;

			this->chessBoard[y][x] = otherPlayer.chess[i];
			this->colorBoard[y][x] = otherPlayer.color;
		}
	}
}

#if 0
// [递归实现] 创键一个棋子的一个方向的子节点 (其中 direction 必须是 Index 型的方向)
void AI::CreateNewNode(BoardNode* boardNode, Chess chess, int direction)
{
	printf("%d\n", this->winNumber[0][0]);

#if 0
	//判断当前方向是否可以移动
	if (chess.color == chess.Red)
	{
		if (direction == chess.Rx && chess.x == 4)
			return;
		else if (direction == chess.Rxy && (chess.x == 4 || chess.y == 4))
			return;
		else if (direction == chess.Ry && chess.y == 4)
			return;
	}
	else
	{
		if (direction == chess.Bx && chess.x == 0)
			return;
		else if (direction == chess.Bxy && (chess.x == 0 || chess.y == 0))
			return;
		else if (direction == chess.By && chess.y == 0)
			return;
	}
#endif

	//判断当前方向是否可以移动
	chess.FeasibleZone();					//计算其可行域
	
	//创键新节点并复制旧节点状态
	BoardNode* newNode = initBoardNode(boardNode->rootChess, boardNode->Chess_Moveable_index, boardNode->winNumber, boardNode->loseNumber, boardNode->chessBoard);

	//改变新节点状态
	int pre_x = chess.x;
	int pre_y = chess.y;
	int aim_x = chess.feasibleZone[direction][0];
	int aim_y = chess.feasibleZone[direction][1];
	newNode->chessBoard[aim_x][aim_y].initChess();								//除掉目标位置的棋子 (无论有没有都适用)
	chess.x = aim_x;
	chess.y = aim_y;
	newNode->chessBoard[aim_x][aim_y] = chess;
	newNode->chessBoard[pre_x][pre_y].initChess();

	//判断是否达到终局状态 (如果对方棋子未归零，则压入栈中)
	std::stack<Chess>enemyChessStack;
	int otherPlayerChessNumber = 0;
	for (int i = 0; i < 5;i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (newNode->chessBoard[i][j].out == false)
			{
				if (newNode->chessBoard[i][j].color != chess.color)					//对方颜色节点
				{
					otherPlayerChessNumber++;
					enemyChessStack.push(newNode->chessBoard[i][j]);
				}
			}
		}
	}

	//胜利了
	if (chess.color == newNode->rootChess->color)									//原始方回合
	{
		if (otherPlayerChessNumber == 0)
		{
			newNode->winNumber[newNode->Chess_Moveable_index][direction]++;
			return;
		}

		if (newNode->rootChess->color == chess.Red && aim_x == 4 && aim_y == 4 ||
			newNode->rootChess->color == chess.Blue && aim_x == 0 && aim_y == 0)	//攻击原始方的对手的角点
		{
			newNode->winNumber[newNode->Chess_Moveable_index][direction]++;
			return;
		}
	}
	else																			//原始方的对手的回合
	{
		if (otherPlayerChessNumber == 0)
		{
			newNode->loseNumber[newNode->Chess_Moveable_index][direction]++;
			return;
		}

		if (newNode->rootChess->color == chess.Red && aim_x == 0 && aim_y == 0 ||
			newNode->rootChess->color == chess.Blue && aim_x == 4 && aim_y == 4)	//原始方的角点被攻击
		{
			newNode->loseNumber[newNode->Chess_Moveable_index][direction]++;
			return;
		}
	}

	//创键新的节点
	while (enemyChessStack.empty() == false)
	{
		this->CreateNewNode(newNode, enemyChessStack.top(), 0);
		this->CreateNewNode(newNode, enemyChessStack.top(), 1);
		this->CreateNewNode(newNode, enemyChessStack.top(), 2);
		enemyChessStack.pop();
	}

}
#endif

// [迭代实现] 创键一个棋子的一个方向的子节点 (其中 direction 必须是 Index 型的方向)
void AI::CreateNewNode(BoardNode* boardNode, Chess chess, int direction)
{
	printf("AI.cpp - 312 - 行 - 需要处理\n");
}

//创键N个棋子的3N个方向的节点 (index 是可移动棋子数量的索引)
void AI::CreateAllNodes(Chess chess, int index)
{
	BoardNode* rootNode[3];
	for (int i = 0; i < 3; i++)
	{
		rootNode[i] = initBoardNode(&chess, index, this->winNumber, this->loseNumber, this->chessBoard);
		this->CreateNewNode(rootNode[i], chess, i);
	}
}