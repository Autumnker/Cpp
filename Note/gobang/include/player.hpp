#pragma once

#include <string>

#include "chess.hpp"

/// 玩家信息
class Player {
public:
    Player();
    explicit Player(const std::string& name, Piece color);

    const std::string& getName() const noexcept { return m_name; }
    void setName(const std::string& name) { m_name = name; }

    Piece getColor() const noexcept { return m_color; }
    void setColor(Piece color) { m_color = color; }

    int getScore() const noexcept { return m_score; }
    void addScore(int delta = 1) noexcept { m_score += delta; }
    void resetScore() noexcept { m_score = 0; }

private:
    std::string m_name;
    Piece m_color;
    int m_score;
};

/// 管理回合切换和分数记录
class PlayerManager {
public:
    PlayerManager();

    /// 切换到另一名玩家
    void switchPlayer() noexcept;

    /// 获取当前轮到谁
    Piece getCurrentPlayer() const noexcept { return m_currentPlayer; }

    /// 获取玩家引用
    Player& getPlayer1() noexcept { return m_player1; }
    Player& getPlayer2() noexcept { return m_player2; }
    const Player& getPlayer1() const noexcept { return m_player1; }
    const Player& getPlayer2() const noexcept { return m_player2; }

    /// 重置双方分数
    void resetScores() noexcept;

    /// 增加当前玩家的胜场数
    void awardWin();

private:
    Player m_player1;       // 黑方（先手）
    Player m_player2;       // 白方（后手）
    Piece m_currentPlayer;  // 当前回合
};
