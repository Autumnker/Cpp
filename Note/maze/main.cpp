#include "game.hpp"
#include "maze_generator.hpp"
#include "raylib_renderer.hpp"
#include "raylib_input.hpp"
#include "raylib_clock.hpp"

/**
 * 组合根（Composition Root）：唯一知道"具体用哪个实现"的地方。
 * 需要替换算法/渲染后端/输入后端时，只改这里即可。
 */
int main()
{
    maze::RenderConfig renderConfig;

    maze::RaylibRenderer renderer(renderConfig);
    maze::RaylibInput input;
    maze::RaylibClock clock;
    maze::RecursiveBacktrackerGenerator generator;

    maze::Game game(generator, renderer, input, clock);
    game.run();
    return 0;
}
