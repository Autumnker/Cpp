#ifndef __MAZE_GENERATOR_HPP__
#define __MAZE_GENERATOR_HPP__

/**
 * 地图生成：定义生成配置、生成结果与算法接口。
 * 新增算法只需实现 IMazeGenerator。
 */

#include "maze.hpp"

namespace maze {

/* ------------------------- 生成参数与结果 ------------------------- */

struct MazeConfig
{
    int rows = CELL_NUM_EACH_COL_DEFAULT;
    int cols = CELL_NUM_EACH_ROW_DEFAULT;
    Position start;    // 默认左下角
    Position target;   // 默认右上角

    MazeConfig() { updateDefaultLayout(); }

    // 按指定尺寸创建配置（自动计算默认起点/终点：左下角 -> 右上角）
    static MazeConfig create(int rows_, int cols_)
    {
        MazeConfig cfg;
        cfg.rows = rows_;
        cfg.cols = cols_;
        cfg.updateDefaultLayout();
        return cfg;
    }

private:
    void updateDefaultLayout()
    {
        start = Position(rows - 1, 0);
        target = Position(0, cols - 1);
    }
};

struct GenerateResult
{
    Maze maze;
    pos_vec_t solution;   // 从起点到终点的解路径
};

/* ------------------------- 生成算法接口 ------------------------- */

class IMazeGenerator
{
public:
    virtual ~IMazeGenerator() = default;
    virtual GenerateResult generate(const MazeConfig& config) const = 0;
};

/* ------------------------- 递归回溯法 ------------------------- */

class RecursiveBacktrackerGenerator : public IMazeGenerator
{
public:
    GenerateResult generate(const MazeConfig& config) const override;
};

}

#endif // __MAZE_GENERATOR_HPP__
