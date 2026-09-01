#ifndef __GAME_STATE_HPP__
#define __GAME_STATE_HPP__

/**
 * 游戏状态：持有迷宫、解路径、玩家位置、机器人模式。
 * 移动规则（越界/撞墙判定）在此实现，属于纯逻辑，不依赖任何外部库。
 */

#include "maze.hpp"

namespace maze {

class GameState
{
public:
    GameState() = default;

    // 载入新一局（新迷宫 + 解路径），玩家回到起点
    void startNewGame(const Maze& maze, const pos_vec_t& solution);

    const Maze& maze() const { return maze_; }
    const pos_vec_t& solution() const { return solution_; }
    const Position& player() const { return player_; }

    // 尝试向指定方向移动一格；成功返回 true
    bool tryMovePlayer(Cell::Direction d);

    bool isWin() const { return player_ == maze_.target(); }

    bool robotMode() const { return robotMode_; }
    void setRobotMode(bool on) { robotMode_ = on; }

private:
    Maze maze_;
    pos_vec_t solution_;
    Position player_;
    bool robotMode_ = false;
};

}

#endif // __GAME_STATE_HPP__
