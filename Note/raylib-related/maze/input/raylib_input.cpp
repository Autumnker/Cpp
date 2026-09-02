#include "raylib_input.hpp"

#include "raylib.h"

namespace maze {
namespace {

// 语义化 Key -> raylib 物理按键
int to_ray_key(Key key)
{
    switch (key)
    {
    case Key::UP:           return KEY_UP;
    case Key::DOWN:         return KEY_DOWN;
    case Key::LEFT:         return KEY_LEFT;
    case Key::RIGHT:        return KEY_RIGHT;
    case Key::RESET:        return KEY_R;
    case Key::SHOW_PATH:    return KEY_V;
    case Key::TOGGLE_ROBOT: return KEY_T;
    case Key::ROBOT_FASTER: return KEY_LEFT_SHIFT;
    case Key::ROBOT_SLOWER: return KEY_LEFT_CONTROL;
    case Key::RESTART:      return KEY_SPACE;
    }
    return -1;
}

} // namespace

bool RaylibInput::isDown(Key key) const
{
    int k = to_ray_key(key);
    return k >= 0 && IsKeyDown(k);
}

bool RaylibInput::isPressed(Key key) const
{
    int k = to_ray_key(key);
    return k >= 0 && IsKeyPressed(k);
}

Point RaylibInput::mousePosition() const
{
    Vector2 m = GetMousePosition();
    return Point(static_cast<int>(m.x), static_cast<int>(m.y));
}

bool RaylibInput::mouseDown() const
{
    return IsMouseButtonDown(MOUSE_BUTTON_LEFT);
}

} // namespace maze
