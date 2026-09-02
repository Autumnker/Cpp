#ifndef __MAZE_TYPES_HPP__
#define __MAZE_TYPES_HPP__

/**
 * 核心领域层 —— 共享类型（最底层的"通用词汇"）。
 * 定义跨模块通用的值类型与容器别名；不依赖任何迷宫领域类型与外部库。
 * 任何模块只要用到这些类型即可 include 本文件，无需拖入整个 maze.hpp。
 *
 * 注意：这是依赖方向的最底层，人人都可以依赖它，但它不依赖任何人。
 */

#include <vector>

namespace maze {

/* ------------------------- 格子坐标 ------------------------- */

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

/* ------------------------- 像素/屏幕坐标 ------------------------- */

struct Point
{
    int x = 0;
    int y = 0;
    Point() = default;
    Point(int x_, int y_) : x(x_), y(y_) {}
};

/* ------------------------- 颜色（与渲染后端无关） ------------------------- */

struct RGBA
{
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    unsigned char a = 255;
};

/* ------------------------- 容器别名 ------------------------- */

using pos_vec_t = std::vector<Position>;

}

#endif // __MAZE_TYPES_HPP__
