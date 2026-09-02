#include "robot.hpp"

#include <queue>
#include <vector>

namespace maze {
namespace {

// 广度优先搜索：计算从 from 走向 to 的第一步方向（避开墙），不可达返回 UN_KNOWN
Cell::Direction bfs_next_step(const Maze& maze, const Position& from, const Position& to)
{
    if (from == to || !maze.isInside(from) || !maze.isInside(to))
    {
        return Cell::Direction::UN_KNOWN;
    }

    const int rows = maze.rows();
    const int cols = maze.cols();
    const auto index = [cols](const Position& p) { return p.row * cols + p.col; };

    std::vector<int> prev(rows * cols, -1);          // 记录每个格子是从哪个格子走来的（线性索引）
    std::vector<bool> visited(rows * cols, false);
    std::queue<Position> queue;

    visited[index(from)] = true;
    queue.push(from);

    while (!queue.empty())
    {
        Position cur = queue.front();
        queue.pop();
        if (cur == to) { break; }

        const Cell::Direction dirs[4] = {
            Cell::Direction::UP, Cell::Direction::RIGHT,
            Cell::Direction::DOWN, Cell::Direction::LEFT
        };
        for (Cell::Direction d : dirs)
        {
            Position next = cur;
            switch (d)
            {
            case Cell::Direction::UP:    next.row -= 1; break;
            case Cell::Direction::RIGHT: next.col += 1; break;
            case Cell::Direction::DOWN:  next.row += 1; break;
            case Cell::Direction::LEFT:  next.col -= 1; break;
            default: break;
            }

            if (!maze.isInside(next) || maze.hasWall(cur, d)) { continue; }
            if (visited[index(next)]) { continue; }

            visited[index(next)] = true;
            prev[index(next)] = index(cur);
            queue.push(next);
        }
    }

    if (!visited[index(to)]) { return Cell::Direction::UN_KNOWN; }

    // 从终点沿 prev 回溯，找到从起点出发的第一步方向
    Position step = to;
    while (step != from)
    {
        int parentIndex = prev[index(step)];
        if (parentIndex < 0) { return Cell::Direction::UN_KNOWN; }
        Position parent{ parentIndex / cols, parentIndex % cols };
        if (parent == from) { return direction_between(from, step); }
        step = parent;
    }
    return Cell::Direction::UN_KNOWN;
}

} // namespace

Cell::Direction RobotAI::suggest(const GameState& state) const
{
    const pos_vec_t& solution = state.solution();
    const Position& player = state.player();

    // 优先沿解路径走（玩家在解路径上时开销最小）
    for (size_t i = 0; i + 1 < solution.size(); ++i)
    {
        if (solution[i] == player)
        {
            return direction_between(player, solution[i + 1]);
        }
    }

    // 玩家不在解路径上（例如手动乱走后开启机器人）：用 BFS 向终点寻路
    return bfs_next_step(state.maze(), player, state.maze().target());
}

} // namespace maze
