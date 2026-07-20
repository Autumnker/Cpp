#include "chess.hpp"
#include "player.hpp"
#include "ui.hpp"
#include "utils.hpp"

#include <string>

/// 游戏记录 CSV 文件路径
static const char* GAMES_CSV = "data/games.csv";

/// 跨帧追踪游戏结束状态
static bool s_gameEnded = false;
static GameState s_finalState = GameState::PLAYING;
static int s_finalMoveCount = 0;

/// 每帧检测游戏是否结束，只记录状态，不保存
static void handleGameEnd(ChessBoard& board) {
    if (!board.isGameOver()) {
        s_gameEnded = false;
        return;
    }
    if (s_gameEnded)
        return;
    s_gameEnded = true;
    s_finalState = board.getState();
    s_finalMoveCount = board.getMoveCount();
}

/// 真正结束游戏：加分 + 保存 CSV（在重置或退出前调用）
static void finalizeGame(PlayerManager& players) {
    if (!s_gameEnded)
        return;

    GameState state = s_finalState;
    if (state == GameState::BLACK_WIN || state == GameState::WHITE_WIN) {
        players.awardWin();
    }

    GameRecord rec;
    rec.date = Utils::getCurrentTimeStr();
    rec.player1 = players.getPlayer1().getName();
    rec.player2 = players.getPlayer2().getName();
    rec.moveCount = s_finalMoveCount;

    if (state == GameState::BLACK_WIN) {
        rec.winner = "Black";
    } else if (state == GameState::WHITE_WIN) {
        rec.winner = "White";
    } else {
        rec.winner = "Draw";
    }

    Utils::saveGameRecord(rec, GAMES_CSV);
    Utils::log(std::string("Game saved -> ") + GAMES_CSV);

    s_gameEnded = false;
}

int main() {
    // ---- 初始化 ----
    ChessBoard board;
    PlayerManager players;
    UI ui(board, players);

    ui.init();
    Utils::log("Gomoku started!");

    // ---- 主循环 ----
    while (!ui.shouldClose() && !WindowShouldClose()) {
        ui.handleInput();
        handleGameEnd(board);

        // 检查是否请求新游戏（R 键或按钮）
        if (ui.isNewGameRequested()) {
            finalizeGame(players);   // 保存当前局结果
            board.reset();
            ui.clearNewGameRequested();
        }

        ui.draw();
    }

    // 退出前保存当前局结果（如果有）
    finalizeGame(players);
    Utils::log("Game exited.");
    return 0;
}
