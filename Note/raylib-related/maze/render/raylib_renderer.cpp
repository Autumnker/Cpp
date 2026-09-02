#include "raylib_renderer.hpp"

#include "raylib.h"
#include <algorithm>

namespace maze {
namespace {

// 核心层 RGBA -> raylib Color
Color to_ray_color(const RGBA& c)
{
    return Color{ c.r, c.g, c.b, c.a };
}

constexpr int MARKER_FONT_SIZE = 18;
constexpr int MESSAGE_FONT_SIZE = 48;

} // namespace

RaylibRenderer::RaylibRenderer(const RenderConfig& config) : config_(config) {}

void RaylibRenderer::init(const Maze& maze)
{
    mazeRows_ = maze.rows();
    mazeCols_ = maze.cols();

    int windowWidth = config_.cellEdge * mazeCols_ + 2 * config_.offset;
    int windowHeight = config_.cellEdge * mazeRows_ + 2 * config_.offset;

    InitWindow(windowWidth, windowHeight, config_.title.c_str());
    SetTargetFPS(config_.fps);

    // 设置窗口图标（若资源存在）
    Image icon = LoadImage("resources\\maze_icon.png");
    if (icon.data != nullptr)
    {
        ImageFormat(&icon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
        SetWindowIcon(icon);
        UnloadImage(icon);
    }

    initialized_ = true;
}

void RaylibRenderer::shutdown()
{
    if (initialized_)
    {
        CloseWindow();
        initialized_ = false;
    }
}

bool RaylibRenderer::windowShouldClose() const
{
    return WindowShouldClose();
}

void RaylibRenderer::beginFrame()
{
    BeginDrawing();
    ClearBackground(to_ray_color(config_.backgroundColor));
}

void RaylibRenderer::endFrame()
{
    EndDrawing();
}

void RaylibRenderer::positionToCanvas(const Position& position, int& x, int& y) const
{
    x = position.col * config_.cellEdge + config_.offset;
    y = position.row * config_.cellEdge + config_.offset;
}

void RaylibRenderer::drawMarker(const Position& position, const RGBA& color, const std::string& name) const
{
    int x, y;
    positionToCanvas(position, x, y);

    // 标记 3/4 面积的矩形
    int edge = config_.cellEdge / 4 * 3;
    DrawRectangle(x + config_.cellEdge / 8, y + config_.cellEdge / 8, edge, edge, to_ray_color(color));

    if (name.empty()) { return; }
    int textWidth = MeasureText(name.c_str(), MARKER_FONT_SIZE);
    int tx = (config_.cellEdge - textWidth) / 2 + config_.cellEdge * position.col + config_.offset;
    int ty = (config_.cellEdge - MARKER_FONT_SIZE) / 2 + config_.cellEdge * position.row + config_.offset;
    DrawText(name.c_str(), tx, ty, MARKER_FONT_SIZE, BLACK);
}

void RaylibRenderer::drawMaze(const Maze& maze)
{
    Color wallColor = to_ray_color(config_.wallColor);

    // 墙壁：只通过语义查询 hasWall 逐格绘制，不依赖迷宫内部结构
    for (int row = 0; row < maze.rows(); ++row)
    {
        for (int col = 0; col < maze.cols(); ++col)
        {
            for (int d = Cell::Direction::UP; d < Cell::Direction::UN_KNOWN; ++d)
            {
                if (!maze.hasWall(Position{ row, col }, static_cast<Cell::Direction>(d))) { continue; }

                int x = config_.cellEdge * col + config_.offset;
                int y = config_.cellEdge * row + config_.offset;
                switch (d)
                {
                case Cell::Direction::UP:
                    DrawLine(x, y, x + config_.cellEdge, y, wallColor);
                    break;
                case Cell::Direction::RIGHT:
                    DrawLine(x + config_.cellEdge, y, x + config_.cellEdge, y + config_.cellEdge, wallColor);
                    break;
                case Cell::Direction::DOWN:
                    DrawLine(x + config_.cellEdge, y + config_.cellEdge, x, y + config_.cellEdge, wallColor);
                    break;
                case Cell::Direction::LEFT:
                    DrawLine(x, y, x, y + config_.cellEdge, wallColor);
                    break;
                default:
                    break;
                }
            }
        }
    }

    // 起点 / 终点
    drawMarker(maze.start(), config_.startColor, config_.startName);
    drawMarker(maze.target(), config_.targetColor, config_.targetName);
}

void RaylibRenderer::drawPlayer(const Position& position)
{
    int x, y;
    positionToCanvas(position, x, y);
    float radius = config_.cellEdge / 2.0f;
    DrawCircle(x + config_.cellEdge / 2, y + config_.cellEdge / 2, radius, to_ray_color(config_.playerColor));
}

void RaylibRenderer::drawPath(const pos_vec_t& path)
{
    if (path.empty()) { return; }
    Color color = to_ray_color(config_.solutionColor);
    float half = config_.cellEdge / 2.0f;

    // 填充每个路径格子
    for (const auto& p : path)
    {
        int x = config_.cellEdge * p.col + config_.offset;
        int y = config_.cellEdge * p.row + config_.offset;
        DrawRectangle(x, y, config_.cellEdge, config_.cellEdge, color);
    }

    // 在相邻格子中心之间画粗线，保证拐角处不出现缺口
    for (size_t i = 0; i + 1 < path.size(); ++i)
    {
        Vector2 a{ config_.cellEdge * path[i].col + config_.offset + half,
                   config_.cellEdge * path[i].row + config_.offset + half };
        Vector2 b{ config_.cellEdge * path[i + 1].col + config_.offset + half,
                   config_.cellEdge * path[i + 1].row + config_.offset + half };
        DrawLineEx(a, b, config_.cellEdge * 0.6f, color);
    }
}

void RaylibRenderer::drawMessage(bool isWin)
{
    if (!isWin) { return; }

    int canvasWidth = config_.cellEdge * mazeCols_;
    int canvasHeight = config_.cellEdge * mazeRows_;
    DrawRectangle(config_.offset, config_.offset, canvasWidth, canvasHeight, Color{ 0, 0, 0, 128 });

    const char* msg = "Press SPACE to restart";
    int textWidth = MeasureText(msg, MESSAGE_FONT_SIZE);
    int x = (canvasWidth - textWidth) / 2 + config_.offset;
    int y = (canvasHeight - MESSAGE_FONT_SIZE) / 2 + config_.offset;
    DrawText(msg, x, y, MESSAGE_FONT_SIZE, to_ray_color(config_.messageColor));
}

Position RaylibRenderer::screenToCell(const Point& screen) const
{
    int col = (screen.x - config_.offset) / config_.cellEdge;
    int row = (screen.y - config_.offset) / config_.cellEdge;
    col = std::clamp(col, 0, mazeCols_ - 1);
    row = std::clamp(row, 0, mazeRows_ - 1);
    return Position(row, col);
}

} // namespace maze
