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
    int cell_edge = 12;
    int rows = 680 / cell_edge;
    int cols = 960 / cell_edge;

    maze::RenderConfig renderConfig;
    renderConfig.cellEdge = cell_edge;
    maze::MazeConfig mazeConfig(rows, cols);

    maze::RaylibRenderer renderer(renderConfig);
    maze::RaylibInput input;
    maze::RaylibClock clock;
    maze::RecursiveBacktrackerGenerator generator;

    maze::Game game(generator, renderer, input, clock, mazeConfig);
    game.run();
    return 0;
}
