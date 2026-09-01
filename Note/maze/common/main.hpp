#ifndef __MAIN_HPP__
#define __MAIN_HPP__

#include "maze.hpp"

//========== 全局变量 ==========

extern const int start_row_;
extern const int start_col_;
extern const int target_row_;
extern const int target_col_;
extern MazeMap maze_map;
extern bool is_win_;
extern bool is_reset_;
extern bool robort_mode_;

//========== 全局函数 ==========

void init_game();                                // 初始化游戏
void check_win();                                // 检查是否胜利
void check_reset();                               // 检查是否重置游戏
void reset_game();                               // 重置游戏

#endif //__MAIN_HPP__