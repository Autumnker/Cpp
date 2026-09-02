#include "chess.hpp"

#include <algorithm>

ChessBoard::ChessBoard() {
    reset();
}

void ChessBoard::reset() noexcept {
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            m_board[r][c] = Piece::EMPTY;
        }
    }
    m_history.clear();
    m_winLine.clear();
    m_state = GameState::PLAYING;
}

bool ChessBoard::placePiece(int row, int col, Piece piece) {
    if (!isValidPosition(row, col))
        return false;
    if (m_board[row][col] != Piece::EMPTY)
        return false;
    if (isGameOver())
        return false;
    if (piece == Piece::EMPTY)
        return false;

    m_board[row][col] = piece;
    m_history.push_back({row, col, piece});

    // Check win
    if (checkWin(row, col, piece)) {
        m_state = (piece == Piece::BLACK) ? GameState::BLACK_WIN
                                          : GameState::WHITE_WIN;
    } else if (m_history.size() >= static_cast<std::size_t>(BOARD_SIZE * BOARD_SIZE)) {
        // 棋盘满了 → 平局
        m_state = GameState::DRAW;
    }

    return true;
}

bool ChessBoard::undo() {
    if (m_history.empty())
        return false;

    // Undo last move
    const Move& last = m_history.back();
    m_board[last.row][last.col] = Piece::EMPTY;
    m_history.pop_back();

    // Clear win state
    m_winLine.clear();
    m_state = GameState::PLAYING;

    return true;
}

Piece ChessBoard::getPiece(int row, int col) const {
    if (!isValidPosition(row, col))
        return Piece::EMPTY;
    return m_board[row][col];
}

bool ChessBoard::isGameOver() const noexcept {
    return m_state != GameState::PLAYING;
}

Move ChessBoard::getLastMove() const {
    if (m_history.empty())
        return {-1, -1, Piece::EMPTY};
    return m_history.back();
}

bool ChessBoard::isValidPosition(int row, int col) noexcept {
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

// ---- 胜负判定 ----
// 从落子位置向四个方向检查
bool ChessBoard::checkWin(int row, int col, Piece piece) {
    // 四个方向：(dr, dc)
    // 0: 水平, 1: 垂直, 2: 主对角线, 3: 副对角线
    const int dirs[4][2] = {
        {0, 1},   // 水平线 →
        {1, 0},   // 垂直线 ↓
        {1, 1},   // 主对角线 ↘
        {1, -1}   // 副对角线 ↙
    };

    for (int d = 0; d < 4; ++d) {
        int dr = dirs[d][0];
        int dc = dirs[d][1];

        // 收集该方向上所有连续的相同颜色棋子
        std::vector<std::pair<int, int>> line;
        line.push_back({row, col});

        // 向正方向延伸
        for (int i = 1; i < 5; ++i) {
            int nr = row + dr * i;
            int nc = col + dc * i;
            if (isValidPosition(nr, nc) && m_board[nr][nc] == piece) {
                line.push_back({nr, nc});
            } else {
                break;
            }
        }
        // 向负方向延伸
        for (int i = 1; i < 5; ++i) {
            int nr = row - dr * i;
            int nc = col - dc * i;
            if (isValidPosition(nr, nc) && m_board[nr][nc] == piece) {
                line.push_back({nr, nc});
            } else {
                break;
            }
        }

        if (line.size() >= 5) {
            m_winLine = std::move(line);
            return true;
        }
    }
    return false;
}
