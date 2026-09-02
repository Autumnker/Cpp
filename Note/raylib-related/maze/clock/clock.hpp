#ifndef __CLOCK_HPP__
#define __CLOCK_HPP__

/**
 * 帧时间抽象：避免应用层（Game）依赖 raylib 的 GetFrameTime。
 */

namespace maze {

class IClock
{
public:
    virtual ~IClock() = default;
    virtual float deltaTime() const = 0;
};

}

#endif // __CLOCK_HPP__
