#ifndef __ROBOT_HPP__
#define __ROBOT_HPP__

/**
 * 机器人 AI：观察世界状态，建议下一步移动方向。
 * 只做"建议"，不直接修改任何状态，因此易于替换与测试。
 */

#include "maze.hpp"
#include "game_state.hpp"

namespace maze {

class RobotAI
{
public:
    // 返回建议的移动方向；无法给出建议时返回 UN_KNOWN
    Cell::Direction suggest(const GameState& state) const;
};

}

#endif // __ROBOT_HPP__
