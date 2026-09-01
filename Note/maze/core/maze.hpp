#ifndef __MAZE_HPP__
#define __MAZE_HPP__

/**
 * 核心领域层：纯 C++ 基础类型，不依赖任何外部库（raylib 等）。
 *  - Position  : 格子坐标
 *  - Point     : 像素/屏幕坐标
 *  - RGBA      : 与渲染后端无关的颜色
 *  - Cell      : 单个迷宫格子（四面墙 + 访问标记）
 *  - Maze      : 只读迷宫（格子 + 起点 + 终点）
 */

#include <vector>
#include <cstdlib>

namespace maze {

/* ------------------------- 基础类型 ------------------------- */

struct Position
{
    int row = 0;
    int col = 0;

    Position() = default;
    Position(int row_, int col_) : row(row_), col(col_) {}

    bool operator==(const Position& other) const
    {
        return row == other.row && col == other.col;
    }
    bool operator!=(const Position& other) const
    {
        return !(*this == other);
    }
};

// 像素/屏幕坐标
struct Point
{
    int x = 0;
    int y = 0;
    Point() = default;
    Point(int x_, int y_) : x(x_), y(y_) {}
};

// 颜色（不依赖 raylib）
struct RGBA
{
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    unsigned char a = 255;
};

struct Cell
{
    // UN_KNOWN 与 DIRECTION_SIZE 同为 4：既表示"无效方向"，又作为墙数组长度
    enum Direction { UP = 0, RIGHT, DOWN, LEFT, DIRECTION_SIZE, UN_KNOWN = DIRECTION_SIZE };

    bool wall[DIRECTION_SIZE];   // false=无墙；true=有墙
    bool visited;

    Cell()
    {
        for (int i = 0; i < DIRECTION_SIZE; ++i) { wall[i] = true; }
        visited = false;
    }
};

/* ------------------------- 类型别名 ------------------------- */

using map_t = std::vector<std::vector<Cell>>;
using pos_vec_t = std::vector<Position>;

/* ------------------------- 迷宫对象 ------------------------- */

class Maze
{
public:
    Maze() = default;
    Maze(map_t cells, Position start, Position target)
        : cells_(std::move(cells)), start_(start), target_(target) {}

    bool isInside(const Position& p) const
    {
        return p.row >= 0 && p.row < rows() && p.col >= 0 && p.col < cols();
    }

    // 某格子某方向是否有墙；非法输入一律视为有墙
    bool hasWall(const Position& p, Cell::Direction d) const
    {
        if (!isInside(p) || d < Cell::Direction::UP || d >= Cell::Direction::UN_KNOWN)
        {
            return true;
        }
        return cells_[p.row][p.col].wall[d];
    }

    int rows() const { return static_cast<int>(cells_.size()); }
    int cols() const { return cells_.empty() ? 0 : static_cast<int>(cells_[0].size()); }
    const map_t& cells() const { return cells_; }
    const Position& start() const { return start_; }
    const Position& target() const { return target_; }

private:
    map_t cells_;
    Position start_;
    Position target_;
};

/* ------------------------- 方向工具 ------------------------- */

// 反方向
inline Cell::Direction flip_direction(Cell::Direction d)
{
    switch (d)
    {
    case Cell::Direction::UP:    return Cell::Direction::DOWN;
    case Cell::Direction::RIGHT: return Cell::Direction::LEFT;
    case Cell::Direction::DOWN:  return Cell::Direction::UP;
    case Cell::Direction::LEFT:  return Cell::Direction::RIGHT;
    default:                     return Cell::Direction::UN_KNOWN;
    }
}

// 从 from 移动到 to 所需的方向；不相邻则返回 UN_KNOWN
inline Cell::Direction direction_between(const Position& from, const Position& to)
{
    if (to.row == from.row - 1 && to.col == from.col) { return Cell::Direction::UP; }
    if (to.row == from.row && to.col == from.col + 1) { return Cell::Direction::RIGHT; }
    if (to.row == from.row + 1 && to.col == from.col) { return Cell::Direction::DOWN; }
    if (to.row == from.row && to.col == from.col - 1) { return Cell::Direction::LEFT; }
    return Cell::Direction::UN_KNOWN;
}

// 朝目标格子移动一步的主导方向（用于鼠标拖拽，目标可不相邻）
inline Cell::Direction direction_toward(const Position& from, const Position& to)
{
    int dr = to.row - from.row;
    int dc = to.col - from.col;
    if (dr == 0 && dc == 0) { return Cell::Direction::UN_KNOWN; }
    if (std::abs(dr) >= std::abs(dc)) { return dr < 0 ? Cell::Direction::UP : Cell::Direction::DOWN; }
    return dc > 0 ? Cell::Direction::RIGHT : Cell::Direction::LEFT;
}

/* ------------------------- 默认值 ------------------------- */

constexpr int CELL_NUM_EACH_ROW_DEFAULT = 32;   // 每行格子数
constexpr int CELL_NUM_EACH_COL_DEFAULT = 20;   // 每列格子数

}

#endif // __MAZE_HPP__
