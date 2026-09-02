#include "raylib_clock.hpp"

#include "raylib.h"

namespace maze {

float RaylibClock::deltaTime() const
{
    return GetFrameTime();
}

} // namespace maze
