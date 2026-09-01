#ifndef __RAYLIB_CLOCK_HPP__
#define __RAYLIB_CLOCK_HPP__

/**
 * raylib 帧时间实现（适配层）。
 */

#include "clock.hpp"

namespace maze {

class RaylibClock : public IClock
{
public:
    float deltaTime() const override;
};

}

#endif // __RAYLIB_CLOCK_HPP__
