#pragma once

#include <utility>
#include <vector>

/// 棋子类型
enum class Piece { EMPTY = 0, BLACK = 1, WHITE = 2 };

/// 游戏状态
enum class GameState { PLAYING, BLACK_WIN, WHITE_WIN, DRAW };

/// 单步落子记录
struct Move {
    int row;
    int col;
    Piece piece;
};

/// 15x15 五子棋棋盘
class ChessBoard {
public:
    static constexpr int BOARD_SIZE = 15;

    ChessBoard();

    /// 重置棋盘到初始状态
    void reset() noexcept;

    /// 在 (row, col) 落子；无效时返回 false
    bool placePiece(int row, int col, Piece piece);

    /// 撤销上一步落子
    bool undo();

    /// 获取指定位置的棋子
    Piece getPiece(int row, int col) const;

    /// 获取当前游戏状态
    GameState getState() const noexcept { return m_state; }

    /// 游戏是否已结束
    bool isGameOver() const noexcept;

    /// 已走的步数
    int getMoveCount() const noexcept { return static_cast<int>(m_history.size()); }

    /// 获取落子历史
    const std::vector<Move>& getHistory() const noexcept { return m_history; }

    /// 获取获胜的五连珠位置（仅当有人获胜时有效）
    const std::vector<std::pair<int, int>>& getWinLine() const noexcept { return m_winLine; }

    /// 获取最后一步落子
    Move getLastMove() const;

    /// 检查 (row, col) 是否在棋盘范围内
    static bool isValidPosition(int row, int col) noexcept;

    /// 棋盘大小
    static constexpr int size() noexcept { return BOARD_SIZE; }

private:
    Piece m_board[BOARD_SIZE][BOARD_SIZE];
    std::vector<Move> m_history;
    GameState m_state;
    std::vector<std::pair<int, int>> m_winLine; ///< 获胜线的位置

    /// 检查在 (row, col) 落子后是否获胜
    bool checkWin(int row, int col, Piece piece);
};
