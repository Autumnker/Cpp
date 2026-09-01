#ifndef __RAYLIB_RENDERER_HPP__
#define __RAYLIB_RENDERER_HPP__

/**
 * raylib 渲染器实现（适配层）。
 * 注意：raylib 只出现在本文件及其 .cpp 中。
 */

#include "renderer.hpp"

namespace maze {

class RaylibRenderer : public IRenderer
{
public:
    explicit RaylibRenderer(const RenderConfig& config);

    void init(const Maze& maze) override;
    void shutdown() override;
    bool windowShouldClose() const override;

    void beginFrame() override;
    void endFrame() override;

    void drawMaze(const Maze& maze) override;
    void drawPlayer(const Position& position) override;
    void drawPath(const pos_vec_t& path) override;
    void drawMessage(bool isWin) override;

    Position screenToCell(const Point& screen) const override;

private:
    void positionToCanvas(const Position& position, int& x, int& y) const;
    void drawMarker(const Position& position, const RGBA& color, const std::string& name) const;

    RenderConfig config_;
    int mazeRows_ = 0;
    int mazeCols_ = 0;
    bool initialized_ = false;
};

}

#endif // __RAYLIB_RENDERER_HPP__
