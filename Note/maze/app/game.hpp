#ifndef __GAME_HPP__
#define __GAME_HPP__

/**
 * 应用层：编排核心逻辑（GameState / RobotAI / 生成算法）与适配层
 * （IRenderer / IInput / IClock），自身不依赖 raylib。
 * 负责：游戏主循环、输入 -> 逻辑、机器人接管、重置/胜负。
 */

#include "maze.hpp"
#include "maze_generator.hpp"
#include "game_state.hpp"
#include "robot.hpp"
#include "renderer.hpp"
#include "input.hpp"
#include "clock.hpp"

namespace maze {

class Game
{
public:
    Game(IMazeGenerator& generator, IRenderer& renderer, IInput& input, IClock& clock,
         const MazeConfig& config = MazeConfig{});

    void run();

private:
    void reset();
    void update(float dt);
    void render();
    void handleInput();
    void updateManual(float dt);
    void updateRobot(float dt);

    IMazeGenerator& generator_;
    IRenderer& renderer_;
    IInput& input_;
    IClock& clock_;

    MazeConfig config_;
    GameState state_;
    RobotAI robot_;

    bool showPath_ = false;
    bool grabbed_ = false;       // 鼠标是否已抓住玩家（拖拽）
    float moveTimer_ = 0.0f;     // 按住方向键的连续移动计时
    int robotSpeed_ = 1;         // 机器人速度倍率
    float robotTimer_ = 0.0f;    // 机器人移动计时
    bool winLogged_ = false;
};

}

#endif // __GAME_HPP__
