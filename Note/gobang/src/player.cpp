#include "player.hpp"

// ==================== 玩家类 ====================

Player::Player() : m_name(""), m_color(Piece::EMPTY), m_score(0) {}

Player::Player(const std::string& name, Piece color)
    : m_name(name), m_color(color), m_score(0) {}

// ==================== 玩家管理器 ====================

PlayerManager::PlayerManager()
    : m_player1("Black", Piece::BLACK)
    , m_player2("White", Piece::WHITE)
    , m_currentPlayer(Piece::BLACK) {}

void PlayerManager::switchPlayer() noexcept {
    m_currentPlayer = (m_currentPlayer == Piece::BLACK) ? Piece::WHITE
                                                        : Piece::BLACK;
}

void PlayerManager::resetScores() noexcept {
    m_player1.resetScore();
    m_player2.resetScore();
}

void PlayerManager::awardWin() {
    if (m_currentPlayer == Piece::BLACK) {
        m_player1.addScore();
    } else {
        m_player2.addScore();
    }
}
