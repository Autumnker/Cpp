#pragma once
#include"Dice.h"
#include"Chess.h"

class Player;	//玩家类的向前声明


//AI用于提供自动摆放棋子\自动选择棋子\自动选择移动方向\自动落子等方法
class AI {
public:

	/*---------------属性---------------*/

	//棋盘
	Chess chessBoard[5][5];

	//颜色棋盘(玩家颜色只用了布尔值来表示无法表示无棋子的状态，所以单独表示)
	int colorBoard[5][5];

	//棋盘节点定义
	struct BoardNode
	{
		Chess* rootChess = nullptr;					//指向最初的棋子
		BoardNode* parentPtr = nullptr;				//指向父节点

		int Chess_Moveable_index = -1;				//可移动的棋子的数量编号

		int(*winNumber)[3];							//胜利次数
		int(*loseNumber)[3];						//失败次数

		Chess chessBoard[5][5];						//棋盘状态
	};

	//某颗棋子某个方向胜利次数
	int winNumber[2][3] = { {0,0,0}, {0,0,0} };

	//某颗棋子某个方向失败次数
	int loseNumber[2][3] = { {0,0,0}, {0,0,0} };

	/*---------------方法---------------*/

	//析构函数,回收创键节点时占用的空间
	~AI();

	/*-----被外界调用的方法-----*/

	//自动摆放棋子
	void AutoSetChess(Player& myPlayer);

	//自动选择棋子及其移动方向
	void AutoChoiceChess_DirectionAndMoving(Player& myPlayer, Player& otherPlayer, Dice& dice);



	/*-----内部使用的方法-----*/

	//获取棋局状态
	void GetChessBoard(Player& Myplayer, Player& otherPlayer);

	//棋节点创键
	BoardNode* initBoardNode(Chess* rootChess_, int chess_MoveAble_Index_, int(*winNumber_)[3], int(*loseNumber_)[3], Chess chessBoard_[5][5]);

	//创键一个棋子的一个方向的子节点
	void CreateNewNode(BoardNode* boardNode, Chess chess, int direction = 0);

	//创键N个棋子的3N个方向的节点
	void CreateAllNodes(Chess chess, int index);

};