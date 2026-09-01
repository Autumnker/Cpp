#ifndef __RAYLIB_INPUT_HPP__
#define __RAYLIB_INPUT_HPP__

/**
 * raylib 输入实现（适配层），把语义化 Key 映射到 raylib 物理按键。
 */

#include "input.hpp"

namespace maze {

class RaylibInput : public IInput
{
public:
    bool isDown(Key key) const override;
    bool isPressed(Key key) const override;
    Point mousePosition() const override;
    bool mouseDown() const override;
};

}

#endif // __RAYLIB_INPUT_HPP__
