#include "ui.hpp"

#include <cstdio>

#include "utils.hpp"

// ---- 颜色常量 ----
static const Color COLOR_BG           = {0xDE, 0xB8, 0x87, 255};  // 柔和木色
static const Color COLOR_BOARD_LINE   = {0x5D, 0x40, 0x37, 255};  // 深棕
static const Color COLOR_BLACK_STONE  = {0x1A, 0x1A, 0x1A, 255};  // 黑子
static const Color COLOR_WHITE_STONE  = {0xF0, 0xF0, 0xF0, 255};  // 白子
static const Color COLOR_WHITE_BORDER = {0x33, 0x33, 0x33, 255};  // 白子边框
static const Color COLOR_HOVER_BLACK  = {0x1A, 0x1A, 0x1A, 100};  // 悬停黑子预览
static const Color COLOR_HOVER_WHITE  = {0xF0, 0xF0, 0xF0, 100};  // 悬停白子预览
static const Color COLOR_LAST_MOVE    = {0xFF, 0x30, 0x30, 200};  // 最后落子标记
static const Color COLOR_WIN_HIGHLIGHT = {0xFF, 0xD7, 0x00, 180}; // 获胜高亮
static const Color COLOR_PANEL_BG     = {0xC8, 0xA0, 0x6E, 200};  // 面板背景
static const Color COLOR_TEXT         = {0x2D, 0x1B, 0x0E, 255};  // 文字色
static const Color COLOR_BTN_BG       = {0x8B, 0x6B, 0x4A, 255};  // 按钮背景
static const Color COLOR_BTN_HOVER    = {0xA0, 0x7D, 0x58, 255};  // 按钮悬停
static const Color COLOR_BTN_TEXT     = {0xFF, 0xF8, 0xEE, 255};  // 按钮文字

UI::UI(ChessBoard& board, PlayerManager& players)
    : m_board(board)
    , m_players(players)
    , m_shouldClose(false)
    , m_newGameRequested(false)
    , m_hoverRow(-1)
    , m_hoverCol(-1) {}

UI::~UI() {
    CloseWindow();
}

void UI::init() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Gomoku - Gobang");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);  // 自己处理 ESC
    updateButtonRects();
}

// ---- 坐标转换 ----

int UI::boardToScreenX(int col) const {
    return BOARD_OFFSET_X + col * CELL_SIZE;
}

int UI::boardToScreenY(int row) const {
    return BOARD_OFFSET_Y + row * CELL_SIZE;
}

std::pair<int, int> UI::screenToBoard(int x, int y) const {
    int col = (x - BOARD_OFFSET_X + CELL_SIZE / 2) / CELL_SIZE;
    int row = (y - BOARD_OFFSET_Y + CELL_SIZE / 2) / CELL_SIZE;
    if (col < 0 || col >= ChessBoard::BOARD_SIZE ||
        row < 0 || row >= ChessBoard::BOARD_SIZE) {
        return {-1, -1};
    }
    // 检查是否距离交叉点足够近（不超过半个格子）
    int cx = boardToScreenX(col);
    int cy = boardToScreenY(row);
    int dx = x - cx;
    int dy = y - cy;
    if (dx * dx + dy * dy > (CELL_SIZE / 2) * (CELL_SIZE / 2)) {
        return {-1, -1};
    }
    return {row, col};
}

// ---- 按钮区域 ----

void UI::updateButtonRects() {
    const float btnW = 150.0f;
    const float btnH = 40.0f;
    const float btnX = static_cast<float>(INFO_PANEL_X);
    const float startY = 380.0f;
    const float gap = 14.0f;

    m_btnNewGame = {btnX, startY, btnW, btnH};
    m_btnUndo    = {btnX, startY + btnH + gap, btnW, btnH};
    m_btnQuit    = {btnX, startY + (btnH + gap) * 2, btnW, btnH};
}

// ==================== 主绘制入口 ====================

void UI::draw() {
    BeginDrawing();
    ClearBackground(COLOR_BG);

    drawGrid();
    drawStarPoints();
    drawWinHighlight();
    drawPieces();
    drawLastMoveHighlight();
    drawHoverPreview();
    drawInfoPanel();

    if (m_board.isGameOver()) {
        drawGameOverOverlay();
    }

    EndDrawing();
}

// ==================== 各绘制模块 ====================

void UI::drawBackground() {
    // 背景已在 ClearBackground 中处理
}

void UI::drawGrid() {
    const int endX = boardToScreenX(ChessBoard::BOARD_SIZE - 1);
    const int endY = boardToScreenY(ChessBoard::BOARD_SIZE - 1);
    const int startX = boardToScreenX(0);
    const int startY = boardToScreenY(0);

    for (int i = 0; i < ChessBoard::BOARD_SIZE; ++i) {
        int x = boardToScreenX(i);
        int y = boardToScreenY(i);

        // 垂直线
        DrawLine(x, startY, x, endY, COLOR_BOARD_LINE);
        // 水平线
        DrawLine(startX, y, endX, y, COLOR_BOARD_LINE);
    }
}

void UI::drawStarPoints() {
    // 星位：四角星、四边星、天元
    const int stars[][2] = {
        {3, 3}, {3, 7}, {3, 11},
        {7, 3}, {7, 7}, {7, 11},
        {11, 3}, {11, 7}, {11, 11}
    };
    constexpr int numStars = sizeof(stars) / sizeof(stars[0]);
    constexpr float radius = 3.5f;

    for (int s = 0; s < numStars; ++s) {
        int cx = boardToScreenX(stars[s][1]);
        int cy = boardToScreenY(stars[s][0]);
        DrawCircle(cx, cy, radius, COLOR_BOARD_LINE);
    }
}

void UI::drawPieces() {
    const float radius = CELL_SIZE * 0.43f;

    for (int r = 0; r < ChessBoard::BOARD_SIZE; ++r) {
        for (int c = 0; c < ChessBoard::BOARD_SIZE; ++c) {
            Piece p = m_board.getPiece(r, c);
            if (p == Piece::EMPTY) continue;

            int px = boardToScreenX(c);
            int py = boardToScreenY(r);

            if (p == Piece::BLACK) {
                DrawCircle(px, py, radius, COLOR_BLACK_STONE);
            } else {
                DrawCircle(px, py, radius, COLOR_WHITE_STONE);
                DrawCircleLines(px, py, radius, COLOR_WHITE_BORDER);
            }
        }
    }
}

void UI::drawHoverPreview() {
    if (m_board.isGameOver()) return;
    if (m_hoverRow < 0 || m_hoverCol < 0) return;
    if (m_board.getPiece(m_hoverRow, m_hoverCol) != Piece::EMPTY) return;

    int px = boardToScreenX(m_hoverCol);
    int py = boardToScreenY(m_hoverRow);
    const float radius = CELL_SIZE * 0.43f;

    Piece cur = m_players.getCurrentPlayer();
    if (cur == Piece::BLACK) {
        DrawCircle(px, py, radius, COLOR_HOVER_BLACK);
    } else {
        DrawCircle(px, py, radius, COLOR_HOVER_WHITE);
        DrawCircleLines(px, py, radius,
                        {0x33, 0x33, 0x33, 80});
    }
}

void UI::drawLastMoveHighlight() {
    Move last = m_board.getLastMove();
    if (last.row < 0) return;

    int px = boardToScreenX(last.col);
    int py = boardToScreenY(last.row);
    const float radius = 5.0f;
    DrawCircle(px, py, radius, COLOR_LAST_MOVE);
}

void UI::drawWinHighlight() {
    const auto& winLine = m_board.getWinLine();
    if (winLine.empty()) return;

    const float radius = CELL_SIZE * 0.43f + 2.0f;
    for (const auto& pos : winLine) {
        int px = boardToScreenX(pos.second);
        int py = boardToScreenY(pos.first);
        DrawCircle(px, py, radius, COLOR_WIN_HIGHLIGHT);
        // 重新绘制获胜线上的棋子（在高亮之上）
        Piece p = m_board.getPiece(pos.first, pos.second);
        if (p == Piece::BLACK) {
            DrawCircle(px, py, radius - 2.0f, COLOR_BLACK_STONE);
        } else {
            DrawCircle(px, py, radius - 2.0f, COLOR_WHITE_STONE);
            DrawCircleLines(px, py, radius - 2.0f, COLOR_WHITE_BORDER);
        }
    }
}

void UI::drawInfoPanel() {
    const int px = INFO_PANEL_X;
    const int titleY = 65;
    const int fontSize = 20;

    // ---- 面板背景 ----
    DrawRectangle(px - 10, 45, 195, 500, COLOR_PANEL_BG);

    // ---- 当前回合 ----
    const char* turnText;
    Color turnColor;
    if (m_board.isGameOver()) {
        GameState s = m_board.getState();
        if (s == GameState::BLACK_WIN) {
            turnText = "Black Wins!";
            turnColor = COLOR_BLACK_STONE;
        } else if (s == GameState::WHITE_WIN) {
            turnText = "White Wins!";
            turnColor = DARKGRAY;
        } else {
            turnText = "Draw!";
            turnColor = COLOR_TEXT;
        }
    } else {
        turnText = (m_players.getCurrentPlayer() == Piece::BLACK)
                       ? "Black's Turn"
                       : "White's Turn";
        turnColor = (m_players.getCurrentPlayer() == Piece::BLACK)
                        ? COLOR_BLACK_STONE
                        : DARKGRAY;
    }
    DrawText(turnText, px, titleY, fontSize, turnColor);

    // ---- 步数统计 ----
    char movesBuf[32];
    std::snprintf(movesBuf, sizeof(movesBuf), "Moves: %d", m_board.getMoveCount());
    DrawText(movesBuf, px, titleY + 35, fontSize, COLOR_TEXT);

    // ---- 分数 ----
    char scoreBuf[64];
    std::snprintf(scoreBuf, sizeof(scoreBuf), "Black: %d wins",
                  m_players.getPlayer1().getScore());
    DrawText(scoreBuf, px, titleY + 75, 18, COLOR_TEXT);

    std::snprintf(scoreBuf, sizeof(scoreBuf), "White: %d wins",
                  m_players.getPlayer2().getScore());
    DrawText(scoreBuf, px, titleY + 100, 18, COLOR_TEXT);

    // ---- 按钮 ----
    drawButton(m_btnNewGame, "New Game (R)", COLOR_BTN_BG, COLOR_BTN_TEXT);
    drawButton(m_btnUndo,    "Undo (U)",     COLOR_BTN_BG, COLOR_BTN_TEXT);
    drawButton(m_btnQuit,    "Quit (ESC)",   COLOR_BTN_BG, COLOR_BTN_TEXT);

    // ---- 操作提示 ----
    DrawText("Left Click: Place", px, 530, 16, COLOR_TEXT);
}

void UI::drawButton(Rectangle rect, const char* text, Color bg, Color fg) {
    // 检测鼠标悬停
    Vector2 mouse = GetMousePosition();
    bool hover = CheckCollisionPointRec(mouse, rect);
    Color drawBg = hover ? COLOR_BTN_HOVER : bg;

    DrawRectangleRec(rect, drawBg);
    // 居中绘制文字
    int textW = MeasureText(text, 18);
    int textX = static_cast<int>(rect.x + (rect.width - textW) / 2.0f);
    int textY = static_cast<int>(rect.y + (rect.height - 18) / 2.0f);
    DrawText(text, textX, textY, 18, fg);

    // 边框
    DrawRectangleLinesEx(rect, 1.5f, {0x5D, 0x40, 0x37, 200});
}

void UI::drawGameOverOverlay() {
    const char* msg = nullptr;
    GameState s = m_board.getState();
    if (s == GameState::BLACK_WIN) {
        msg = "Black Wins!  Press R to restart";
    } else if (s == GameState::WHITE_WIN) {
        msg = "White Wins!  Press R to restart";
    } else {
        msg = "Draw!  Press R to restart";
    }

    int textW = MeasureText(msg, 28);
    int textX = (WINDOW_WIDTH - textW) / 2;
    int textY = WINDOW_HEIGHT - 55;

    // 半透明背景条
    DrawRectangle(0, textY - 8, WINDOW_WIDTH, 44,
                  {0x00, 0x00, 0x00, 120});
    DrawText(msg, textX, textY, 28, RAYWHITE);
}

// ==================== 输入处理 ====================

void UI::handleInput() {
    // ---- 键盘 ----
    if (IsKeyPressed(KEY_ESCAPE)) {
        m_shouldClose = true;
        return;
    }
    if (IsKeyPressed(KEY_R)) {
        m_newGameRequested = true;
        return;
    }
    if (IsKeyPressed(KEY_U)) {
        bool wasOver = m_board.isGameOver();
        m_board.undo();
        // 悔棋后需要切换回上一步的玩家
        // 但如果游戏之前已经结束（有人赢了），赢家还未切换过回合，不需要切换
        if (!wasOver && m_board.getMoveCount() > 0) {
            m_players.switchPlayer();
        }
        return;
    }

    // ---- 鼠标悬停检测 ----
    Vector2 mouse = GetMousePosition();
    auto [hRow, hCol] = screenToBoard(static_cast<int>(mouse.x),
                                       static_cast<int>(mouse.y));
    m_hoverRow = hRow;
    m_hoverCol = hCol;

    // ---- 鼠标点击 ----
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        int mx = static_cast<int>(mouse.x);
        int my = static_cast<int>(mouse.y);

        // 检查按钮点击
        if (CheckCollisionPointRec(mouse, m_btnNewGame)) {
            m_newGameRequested = true;
            return;
        }
        if (CheckCollisionPointRec(mouse, m_btnUndo)) {
            bool wasOver = m_board.isGameOver();
            m_board.undo();
            if (!wasOver && m_board.getMoveCount() > 0) {
                m_players.switchPlayer();
            }
            return;
        }
        if (CheckCollisionPointRec(mouse, m_btnQuit)) {
            m_shouldClose = true;
            return;
        }

        // 棋盘落子
        auto [row, col] = screenToBoard(mx, my);
        if (row >= 0 && col >= 0 && !m_board.isGameOver()) {
            Piece cur = m_players.getCurrentPlayer();
            if (m_board.placePiece(row, col, cur)) {
                // 落子成功：检查游戏是否结束
                if (!m_board.isGameOver()) {
                    m_players.switchPlayer();
                }
                // 注意：胜负处理（加分、保存）在 main.cpp 的游戏循环中完成
            }
        }
    }
}
