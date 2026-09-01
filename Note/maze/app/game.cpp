#include "game.hpp"
#include "logger.hpp"

#include <algorithm>

namespace maze {
namespace {

constexpr float PLAYER_HOLD_INTERVAL = 0.1f;   // 按住方向键的连续移动间隔（秒）
constexpr float ROBOT_BASE_SPEED = 10.0f;      // 机器人基础速度（格/秒）
constexpr int   ROBOT_MAX_SPEED_UP = 6;        // 机器人最大速度倍率

Cell::Direction arrow_direction(const IInput& input)
{
    if (input.isDown(Key::UP))    { return Cell::Direction::UP; }
    if (input.isDown(Key::RIGHT)) { return Cell::Direction::RIGHT; }
    if (input.isDown(Key::DOWN))  { return Cell::Direction::DOWN; }
    if (input.isDown(Key::LEFT))  { return Cell::Direction::LEFT; }
    return Cell::Direction::UN_KNOWN;
}

} // namespace

Game::Game(IMazeGenerator& generator, IRenderer& renderer, IInput& input, IClock& clock,
           const MazeConfig& config)
    : generator_(generator), renderer_(renderer), input_(input), clock_(clock), config_(config)
{
}

void Game::run()
{
    reset();
    renderer_.init(state_.maze());

    while (!renderer_.windowShouldClose())
    {
        float dt = clock_.deltaTime();
        update(dt);
        render();
    }
    renderer_.shutdown();
}

void Game::reset()
{
    GenerateResult result = generator_.generate(config_);
    state_.startNewGame(result.maze, result.solution);

    showPath_ = false;
    grabbed_ = false;
    moveTimer_ = 0.0f;
    robotSpeed_ = 1;
    robotTimer_ = 0.0f;
    winLogged_ = false;
}

void Game::update(float dt)
{
    handleInput();

    if (state_.robotMode())
    {
        updateRobot(dt);
    }
    else
    {
        updateManual(dt);
    }

    if (state_.isWin() && !winLogged_)
    {
        winLogged_ = true;
        Logger::instance().info(__FILE__, __FUNCTION__, __LINE__, "You win!");
    }
}

void Game::render()
{
    renderer_.beginFrame();
    if (showPath_) { renderer_.drawPath(state_.solution()); }
    renderer_.drawMaze(state_.maze());
    renderer_.drawPlayer(state_.player());
    renderer_.drawMessage(state_.isWin());
    renderer_.endFrame();
}

void Game::handleInput()
{
    if (input_.isPressed(Key::RESET)) { reset(); return; }
    if (state_.isWin() && input_.isPressed(Key::RESTART)) { reset(); return; }

    if (input_.isPressed(Key::SHOW_PATH)) { showPath_ = !showPath_; }
    if (input_.isPressed(Key::TOGGLE_ROBOT))
    {
        state_.setRobotMode(!state_.robotMode());
        robotTimer_ = 0.0f;
    }
}

void Game::updateManual(float dt)
{
    if (state_.isWin()) { return; }

    // 鼠标拖拽玩家
    if (input_.mouseDown())
    {
        Position cell = renderer_.screenToCell(input_.mousePosition());
        if (!grabbed_ && cell == state_.player())
        {
            grabbed_ = true;
        }
        else if (grabbed_)
        {
            state_.tryMovePlayer(direction_toward(state_.player(), cell));
        }
    }
    else
    {
        grabbed_ = false;
    }

    // 键盘移动（支持按住连续移动；新按下的方向优先）
    Cell::Direction dir = arrow_direction(input_);
    bool justPressed = input_.isPressed(Key::UP) || input_.isPressed(Key::DOWN) ||
                       input_.isPressed(Key::LEFT) || input_.isPressed(Key::RIGHT);
    if (justPressed)
    {
        if (input_.isPressed(Key::UP))          { dir = Cell::Direction::UP; }
        else if (input_.isPressed(Key::RIGHT))  { dir = Cell::Direction::RIGHT; }
        else if (input_.isPressed(Key::DOWN))   { dir = Cell::Direction::DOWN; }
        else if (input_.isPressed(Key::LEFT))   { dir = Cell::Direction::LEFT; }
    }

    if (dir != Cell::Direction::UN_KNOWN)
    {
        moveTimer_ += dt;
        if (justPressed || moveTimer_ >= PLAYER_HOLD_INTERVAL)
        {
            state_.tryMovePlayer(dir);
            moveTimer_ = 0.0f;
        }
    }
    else
    {
        moveTimer_ = 0.0f;
    }
}

void Game::updateRobot(float dt)
{
    if (state_.isWin()) { return; }

    if (input_.isPressed(Key::ROBOT_FASTER)) { robotSpeed_ = std::min(robotSpeed_ + 1, ROBOT_MAX_SPEED_UP); }
    if (input_.isPressed(Key::ROBOT_SLOWER)) { robotSpeed_ = std::max(robotSpeed_ - 1, 0); }
    if (robotSpeed_ <= 0) { return; }

    robotTimer_ += dt;
    if (robotTimer_ >= 1.0f / (ROBOT_BASE_SPEED * robotSpeed_))
    {
        Cell::Direction dir = robot_.suggest(state_);
        if (dir != Cell::Direction::UN_KNOWN) { state_.tryMovePlayer(dir); }
        robotTimer_ = 0.0f;
    }
}

} // namespace maze
