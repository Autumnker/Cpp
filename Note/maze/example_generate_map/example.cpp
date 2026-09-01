#include "generate_map.hpp"
#include "raylib.h"

static Color color_cell_edge_ = BLACK;
static Color color_curr_visit_ = GREEN;
static float radius_curr_visit_ = CELL_EDGE / 2;
#define TARGET_FPS 60
#define WINDOW_TITLE "example_generate_map"

// 绘制棋盘
void draw_cell_map()
{
    if (cell_map_.empty() || cell_map_[0].empty()) { return; }

    for (int row = 0; row < cell_map_.size(); ++row)
    {
        for (int col = 0; col < cell_map_[row].size(); ++col)
        {
#ifndef DRAW_NOT_VISITED
            if (!cell_map_[row][col].visited) { continue; } // 没被访问到的先不画
#endif // DRAW_NOT_VISITED
            // 计算坐标
            int x = CANVAS_OFFSET + col * CELL_EDGE;
            int y = CANVAS_OFFSET + row * CELL_EDGE;
#ifdef ERASE_EFFECT
            //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 擦除效果 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
            if (!cell_map_[row][col].visited)
            {
                DrawRectangle(x, y, CELL_EDGE, CELL_EDGE, GRAY);
                continue;
            }
            //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#endif // ERASE_EFFECT
            // 绘制
            if (cell_map_[row][col].wall[0])
            {
                DrawLine(x, y, x + CELL_EDGE, y, color_cell_edge_);
            }
            if (cell_map_[row][col].wall[1])
            {
                DrawLine(x + CELL_EDGE, y, x + CELL_EDGE, y + CELL_EDGE, color_cell_edge_);
            }
            if (cell_map_[row][col].wall[2])
            {
                DrawLine(x + CELL_EDGE, y + CELL_EDGE, x, y + CELL_EDGE, color_cell_edge_);
            }
            if (cell_map_[row][col].wall[3])
            {
                DrawLine(x, y + CELL_EDGE, x, y, color_cell_edge_);
            }
        }
    }
}

// 绘制完成界面
void draw_map_generate_done()
{
    if (!map_generate_done) { return; }

    const char* text = "Map Generate Done";
    int font_size = CELL_EDGE;
    int text_width = MeasureText(text, font_size);
    int x = CANVAS_OFFSET + (CANVAS_WIDTH - text_width) / 2;
    int y = CANVAS_OFFSET + (CANVAS_HEIGH - font_size) / 2;

    DrawRectangle(CANVAS_OFFSET, CANVAS_OFFSET, CANVAS_WIDTH, CANVAS_HEIGH, (Color) { 0, 0, 0, 90 });
    DrawText(text, x, y, font_size, RED);
}

// 绘制当前访问点
void draw_curr_visit(int curr_row, int curr_col)
{
    if (map_generate_done) { return; }

    int x = CANVAS_OFFSET + CELL_EDGE * curr_col + CELL_EDGE / 2;
    int y = CANVAS_OFFSET + CELL_EDGE * curr_row + CELL_EDGE / 2;
    DrawCircle(x, y, radius_curr_visit_, color_curr_visit_);
}

int main(int argc, char const* argv[])
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGH, WINDOW_TITLE);
    SetTargetFPS(TARGET_FPS);

    int curr_row = start_row_;
    int curr_col = start_col_;
    bool generate_done = false;
    while (!WindowShouldClose())
    {
        // 生成地图
        generate_map(curr_row, curr_col);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        draw_cell_map();
        draw_curr_visit(curr_row, curr_col);
        draw_map_generate_done();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
