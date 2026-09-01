#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

/**
 * 渲染抽象：应用层只依赖 IRenderer，不依赖具体渲染后端。
 * raylib 只允许出现在 IRenderer 的实现（RaylibRenderer）中。
 */

#include "maze.hpp"

#include <string>

namespace maze {

/* ------------------------- 渲染配置 ------------------------- */

struct RenderConfig
{
    std::string title = "maze";
    int fps = 60;
    int cellEdge = 30;    // 格子边长（像素）
    int offset = 15;      // 画布原点相对窗口原点的偏移

    RGBA backgroundColor{ 245, 245, 245, 255 };
    RGBA wallColor{ 0, 0, 0, 255 };
    RGBA playerColor{ 0, 200, 0, 255 };
    RGBA startColor{ 0, 0, 255, 255 };
    RGBA targetColor{ 255, 0, 0, 255 };
    RGBA solutionColor{ 255, 255, 0, 255 };
    RGBA messageColor{ 255, 0, 0, 255 };

    std::string startName = "start";
    std::string targetName = "target";
};

/* ------------------------- 渲染接口 ------------------------- */

class IRenderer
{
public:
    virtual ~IRenderer() = default;

    // 窗口生命周期（init 时需已知迷宫尺寸以确定窗口大小）
    virtual void init(const Maze& maze) = 0;
    virtual void shutdown() = 0;
    virtual bool windowShouldClose() const = 0;

    // 帧
    virtual void beginFrame() = 0;
    virtual void endFrame() = 0;

    // 绘制
    virtual void drawMaze(const Maze& maze) = 0;
    virtual void drawPlayer(const Position& position) = 0;
    virtual void drawPath(const pos_vec_t& path) = 0;
    virtual void drawMessage(bool isWin) = 0;

    // 屏幕坐标 -> 格子坐标
    virtual Position screenToCell(const Point& screen) const = 0;
};

}

#endif // __RENDERER_HPP__
