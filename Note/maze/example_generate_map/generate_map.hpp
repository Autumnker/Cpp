#ifndef __GENERATE_MAP_HPP__
#define __GENERATE_MAP_HPP__

#include <vector>

//========== 宏定义 ==========

#define CELL_EDGE 40                        // 格子边长
#define CELL_NUM_PRE_ROW 32                     // 每行格子数
#define CELL_NUM_PRE_COLUMN 20                   // 每列格子数
#define CANVAS_WIDTH (CELL_NUM_PRE_ROW * CELL_EDGE)        // 画布宽度
#define CANVAS_HEIGH (CELL_NUM_PRE_COLUMN * CELL_EDGE)      // 画布高度
#define CANVAS_OFFSET (CELL_EDGE / 2)               // 画布原点相较于窗口远点的偏移量
#define WINDOW_WIDTH (CANVAS_WIDTH + (CANVAS_OFFSET * 2))    // 窗口宽度 
#define WINDOW_HEIGH (CANVAS_HEIGH + (CANVAS_OFFSET * 2))    // 窗口高度
#define GENERATE_SPEED 10                      // 地图生成速度,单位 (CELL_EDGE / S)

//========== 数据结构 ==========

struct Cell
{
    bool wall[4];
    bool visited;
    Cell()
    {
        visited = false;
        for (int i = 0; i < 4; ++i)
        {
            wall[i] = true;
        }
    }
};

//========== API ==========

extern const int start_row_;                    // 起始行
extern const int start_col_;                    // 起始列
extern const int target_row_;                   // 目标行
extern const int target_col_;                   // 目标列
extern bool map_generate_done;                   // 地图生成成功
extern std::vector<std::vector<Cell>> cell_map_;          // 地图
extern std::vector<int> visited_path_;               // 历史路径

void generate_map(int& curr_row, int& curr_col);          // 地图生成算法

#endif //__GENERATE_MAP_HPP__
