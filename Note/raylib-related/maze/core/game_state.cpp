#include "game_state.hpp"

namespace maze {

void GameState::startNewGame(const Maze& maze, const pos_vec_t& solution)
{
    maze_ = maze;
    solution_ = solution;
    player_ = maze_.start();
    robotMode_ = false;
}

bool GameState::tryMovePlayer(Cell::Direction d)
{
    if (d < Cell::Direction::UP || d >= Cell::Direction::UN_KNOWN) { return false; }

    Position next = player_;
    switch (d)
    {
    case Cell::Direction::UP:    next.row -= 1; break;
    case Cell::Direction::RIGHT: next.col += 1; break;
    case Cell::Direction::DOWN:  next.row += 1; break;
    case Cell::Direction::LEFT:  next.col -= 1; break;
    default:                     return false;
    }

    // 越界或撞墙则拒绝
    if (!maze_.isInside(next) || maze_.hasWall(player_, d)) { return false; }

    player_ = next;
    return true;
}

} // namespace maze
