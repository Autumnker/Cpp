#pragma once

#include <string>

#include "chess.hpp"
#include "player.hpp"

// raylib.h 定义了 BLACK/WHITE 颜色宏，与 Piece 枚举冲突
// 在包含 raylib.h 后取消定义它们
#include "raylib.h"
#ifdef BLACK
#undef BLACK
#endif
#ifdef WHITE
#undef WHITE
#endif

/// 处理所有 Raylib 绘制和输入
class UI {
public:
    // ---- 布局常量 ----
    static constexpr int WINDOW_WIDTH = 800;
    static constexpr int WINDOW_HEIGHT = 700;
    static constexpr int BOARD_OFFSET_X = 45;
    static constexpr int BOARD_OFFSET_Y = 55;
    static constexpr int CELL_SIZE = 37;
    static constexpr int BOARD_PIXEL = CELL_SIZE * (ChessBoard::BOARD_SIZE - 1);
    static constexpr int INFO_PANEL_X = BOARD_OFFSET_X + BOARD_PIXEL + 35;

    UI(ChessBoard& board, PlayerManager& players);
    ~UI();

    /// 初始化窗口和资源
    void init();

    /// 主绘制入口
    void draw();

    /// 处理鼠标/键盘输入
    void handleInput();

    /// 窗口是否应该关闭
    bool shouldClose() const noexcept { return m_shouldClose; }

    /// 是否请求了开始新游戏（R 键或 New Game 按钮）
    bool isNewGameRequested() const noexcept { return m_newGameRequested; }

    /// 清除新游戏请求标志
    void clearNewGameRequested() noexcept { m_newGameRequested = false; }

    /// 将屏幕像素坐标转换为棋盘 (row, col)；无效时返回 {-1,-1}
    std::pair<int, int> screenToBoard(int x, int y) const;

private:
    ChessBoard& m_board;
    PlayerManager& m_players;
    bool m_shouldClose;
    bool m_newGameRequested;

    // 当前悬停的棋盘格子（用于预览）
    int m_hoverRow;
    int m_hoverCol;

    // ---- 内部绘制函数 ----
    void drawBackground();
    void drawGrid();
    void drawStarPoints();
    void drawPieces();
    void drawHoverPreview();
    void drawLastMoveHighlight();
    void drawWinHighlight();
    void drawInfoPanel();
    void drawButton(Rectangle rect, const char* text, Color bg, Color fg);
    void drawGameOverOverlay();

    // 将棋盘坐标转换为屏幕像素坐标（交叉点）
    int boardToScreenX(int col) const;
    int boardToScreenY(int row) const;

    // 信息面板按钮区域
    Rectangle m_btnNewGame;
    Rectangle m_btnUndo;
    Rectangle m_btnQuit;

    void updateButtonRects();
};
