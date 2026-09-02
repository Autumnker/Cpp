#include "generate_map.hpp"
#include "raylib.h"
#include "logger.hpp"
#include <random>

const int start_row_ = 0;
const int start_col_ = 0;
const int target_row_ = CELL_NUM_PRE_COLUMN - 1;
const int target_col_ = CELL_NUM_PRE_ROW - 1;
bool map_generate_done = false;
std::vector<std::vector<Cell>> cell_map_{ CELL_NUM_PRE_COLUMN, std::vector<Cell>{CELL_NUM_PRE_ROW, Cell()} };
std::vector<int> visited_path_;

// 向指定方向移动
bool move_to_direction(int& row, int& col, int direction)
{
    if (direction == 0 && row - 1 >= 0)
    {
        row--;
        return true;
    }
    else if (direction == 1 && col + 1 < CELL_NUM_PRE_ROW)
    {
        col++;
        return true;
    }
    else if (direction == 2 && row + 1 < CELL_NUM_PRE_COLUMN)
    {
        row++;
        return true;
    }
    else if (direction == 3 && col - 1 >= 0)
    {
        col--;
        return true;
    }

    return false;
}
// 检查是否还有未被访问的格子
bool is_visited_all()
{
    for (const auto& row : cell_map_)
    {
        for (const auto& cell : row)
        {
            if (!cell.visited)
            {
                return false;
            }
        }
    }
    return true;
}
// 移动方向反转
int flip_direction(int direction)
{
    switch (direction)
    {
    case 0: return 2;
    case 1: return 3;
    case 2: return 0;
    case 3: return 1;
    }
    return -1;
}
// 延时函数
bool is_timeout(float timeout)
{
    static float elapsed_time = 0.0f;
    if (elapsed_time < timeout)
    {
        elapsed_time += GetFrameTime();
        return false;
    }
    elapsed_time = 0.0f;
    return true;
}
// 随机数生成
std::random_device rd;
std::mt19937 gen(rd());
int generate_random_number(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

// 地图生成算法
void generate_map(int& curr_row, int& curr_col)
{
    if (map_generate_done) { return; }  // 必须放最上面避免遍历地图的资源开销
    if (is_visited_all()) { map_generate_done = true; }

    // 延迟
    if (!is_timeout(1.0f / GENERATE_SPEED)) { return; }

    // 收集可移动方向
    std::vector<int> moveable_direction;
    if (curr_row - 1 >= 0 && !cell_map_[curr_row - 1][curr_col].visited) { moveable_direction.push_back(0); }
    if (curr_col + 1 < CELL_NUM_PRE_ROW && !cell_map_[curr_row][curr_col + 1].visited) { moveable_direction.push_back(1); }
    if (curr_row + 1 < CELL_NUM_PRE_COLUMN && !cell_map_[curr_row + 1][curr_col].visited) { moveable_direction.push_back(2); }
    if (curr_col - 1 >= 0 && !cell_map_[curr_row][curr_col - 1].visited) { moveable_direction.push_back(3); }

    // 确定是否回退
    if (moveable_direction.empty())
    {
        if (visited_path_.empty())
        {
            Logger::instance().fail(__FILE__, __FUNCTION__, __LINE__, "回溯路径为空");
            return;
        }
        // 获得回退方向
        int direction = flip_direction(visited_path_[visited_path_.size() - 1]);
        if (direction == -1)
        {
            Logger::instance().fail(__FILE__, __FUNCTION__, __LINE__, "回退方向错误");
            return;
        }
        // 回退
        if (move_to_direction(curr_row, curr_col, direction))
        {
            visited_path_.pop_back();
        }
        // 本帧已执行回退动作，等待下一帧再重新判断可移动方向
        return;
    }

    // 确定移动方向
    int index = generate_random_number(0, moveable_direction.size() - 1);
    int direction = moveable_direction[index];

    // 移动
    cell_map_[curr_row][curr_col].wall[direction] = false;      // 打破本侧墙壁
    if (move_to_direction(curr_row, curr_col, direction))
    {
        // 打破对侧墙壁
        cell_map_[curr_row][curr_col].wall[flip_direction(direction)] = false;
        cell_map_[curr_row][curr_col].visited = true;
        visited_path_.push_back(direction);
    }
    else
    {
        cell_map_[curr_row][curr_col].wall[direction] = true;   // 恢复本侧墙壁
        std::string msg = "移动失败, direction=" + std::to_string(direction);
        Logger::instance().fail(__FILE__, __FUNCTION__, __LINE__, msg.c_str());
    }
}
