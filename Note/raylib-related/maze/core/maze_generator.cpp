#include "maze_generator.hpp"
#include "logger.hpp"

#include <random>

namespace maze {
namespace {

// 生成随机数 [min, max]
int random_number(int min, int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

bool is_all_visited(const map_t& map)
{
    for (const auto& row : map)
    {
        for (const auto& cell : row)
        {
            if (!cell.visited) { return false; }
        }
    }
    return true;
}

// 当前格子可移动到的未访问邻居
void movable_positions(const map_t& map, const Position& current, pos_vec_t& out)
{
    int total_row = static_cast<int>(map.size());
    int total_col = static_cast<int>(map[0].size());

    if (current.row - 1 >= 0 && !map[current.row - 1][current.col].visited)
        out.push_back(Position{ current.row - 1, current.col });
    if (current.col + 1 < total_col && !map[current.row][current.col + 1].visited)
        out.push_back(Position{ current.row, current.col + 1 });
    if (current.row + 1 < total_row && !map[current.row + 1][current.col].visited)
        out.push_back(Position{ current.row + 1, current.col });
    if (current.col - 1 >= 0 && !map[current.row][current.col - 1].visited)
        out.push_back(Position{ current.row, current.col - 1 });
}

// 打通 from -> to 之间的墙
void break_wall(map_t& map, const Position& from, const Position& to)
{
    Cell::Direction d = direction_between(from, to);
    if (d == Cell::Direction::UN_KNOWN) { return; }
    map[from.row][from.col].wall[d] = false;
    map[to.row][to.col].wall[flip_direction(d)] = false;
}

} // namespace

GenerateResult RecursiveBacktrackerGenerator::generate(const MazeConfig& config) const
{
    GenerateResult result;
    if (config.rows <= 0 || config.cols <= 0)
    {
        Logger::instance().fail(__FILE__, __FUNCTION__, __LINE__, "Invalid maze config");
        return result;
    }

    // 起点/终点必须在迷宫范围内，否则后续会越界访问（未定义行为）
    if (config.start.row < 0 || config.start.row >= config.rows ||
        config.start.col < 0 || config.start.col >= config.cols ||
        config.target.row < 0 || config.target.row >= config.rows ||
        config.target.col < 0 || config.target.col >= config.cols)
    {
        Logger::instance().fail(__FILE__, __FUNCTION__, __LINE__, "Invalid start/target position");
        return result;
    }

    map_t map(config.rows, std::vector<Cell>(config.cols, Cell{}));
    pos_vec_t path;   // 当前回溯路径（起点 -> 当前格子）

    Position current = config.start;
    map[current.row][current.col].visited = true;
    path.push_back(current);

    while (!is_all_visited(map))
    {
        pos_vec_t candidates;
        movable_positions(map, current, candidates);

        if (candidates.empty())
        {
            // 回溯：弹出当前死胡同，回到其父节点
            path.pop_back();
            if (path.empty()) { break; }
            current = path.back();
            continue;
        }

        Position next = candidates[random_number(0, static_cast<int>(candidates.size()) - 1)];
        break_wall(map, current, next);
        map[next.row][next.col].visited = true;
        current = next;
        path.push_back(current);

        // 首次到达终点时记录解路径
        if (result.solution.empty() && current == config.target)
        {
            result.solution = path;
        }
    }

    result.maze = Maze(std::move(map), config.start, config.target);
    if (result.solution.empty())
    {
        Logger::instance().warn(__FILE__, __FUNCTION__, __LINE__, "No solution found");
    }
    return result;
}

} // namespace maze
