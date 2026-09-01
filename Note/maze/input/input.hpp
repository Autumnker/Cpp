#ifndef __INPUT_HPP__
#define __INPUT_HPP__

/**
 * 输入抽象：应用层只依赖 IInput，不依赖具体输入后端（raylib）。
 * Key 是语义化的按键，由具体实现（RaylibInput）映射到物理按键。
 */

#include "maze.hpp"

namespace maze {

enum class Key
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    RESET,          // R
    SHOW_PATH,      // V
    TOGGLE_ROBOT,   // T
    ROBOT_FASTER,   // 左 Shift
    ROBOT_SLOWER,   // 左 Ctrl
    RESTART,        // 胜利后 SPACE
};

class IInput
{
public:
    virtual ~IInput() = default;
    virtual bool isDown(Key key) const = 0;
    virtual bool isPressed(Key key) const = 0;
    virtual Point mousePosition() const = 0;
    virtual bool mouseDown() const = 0;
};

}

#endif // __INPUT_HPP__
