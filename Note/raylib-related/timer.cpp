/** 
 * 计时器概念版 - 并未包含软件架构层面的考虑
*/

#include <utility>
#include <string>
#include <chrono>
#include "raylib.h"

#define WINDOW_TITLE "Timer"
#define WINDOW_WIDTH (1920 / 6)
#define WINDOW_HEIGHT (1080 / 6)
#define TARGET_FPS 60

static int time_out = 600;
static int curr_count = time_out;
static bool running = false;
static bool reset = false;

struct Button
{
    Vector2 pos_;
    float width_;
    float height_;
    Button():pos_(std::move(Vector2{0.0f, 0.0f})),width_(0.0f),height_(0.0f){}
    Button(Vector2 pos, float width, float height)
    : pos_(pos),width_(width),height_(height){}
};

Button btn_start{Vector2{WINDOW_WIDTH / 16 * 1, WINDOW_HEIGHT / 8 * 5}, WINDOW_WIDTH / 4, WINDOW_HEIGHT / 8 * 3};
Button btn_pause{Vector2{WINDOW_WIDTH / 16 * 6, WINDOW_HEIGHT / 8 * 5}, WINDOW_WIDTH / 4, WINDOW_HEIGHT / 8 * 3};
Button btn_reset{Vector2{WINDOW_WIDTH / 16 * 11, WINDOW_HEIGHT / 8 * 5}, WINDOW_WIDTH / 4, WINDOW_HEIGHT / 8 * 3};

bool passed_seconds(int sec)
{
#if 0
    static float last_update_time_ = 0.0f;
    last_update_time_ += GetFrameTime();
    if(last_update_time_ >= 1.0f)
    {
        last_update_time_ = 0.0f;
        return true;
    }
    return false;
#endif
    using steady_color = std::chrono::steady_clock;
    static steady_color::time_point last_update_time_ = steady_color::now();
    auto elapsed = steady_color::now() - last_update_time_;
    if(elapsed.count() >= sec)
    {
        if(running)
        {
            last_update_time_ += std::chrono::seconds(sec);
            return true;
        }
        else
        {
            last_update_time_ = steady_color::now();
        }
    }
    return false;
}

#include <iostream>
bool is_hover(const Button& button, const Vector2& point)
{
    if(!IsCursorOnScreen())
    {
        return false;
    }

    if(button.pos_.x <= point.x &&
        button.pos_.x + button.width_ >= point.x &&
        button.pos_.y <= point.y &&
        button.pos_.y + button.height_ >= point.y)
        {
            return true;
        }

        return false;
}

bool button_clicked(const Button& button)
{
    Vector2 mouse_pos = GetMousePosition();
    if(is_hover(button, mouse_pos) && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        return true;
    }

    return false;
}

enum ButtonState {
    BTN_NORMAL,
    BTN_HOVER,
    BTN_PRESSED
};

ButtonState get_button_state(const Button& button)
{
    Vector2 mouse_pos = GetMousePosition();
    bool hover = is_hover(button, mouse_pos);
    bool pressed = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    
    if (hover && pressed) return BTN_PRESSED;
    if (hover) return BTN_HOVER;
    return BTN_NORMAL;
}

void draw_button(const Button& button, const char* text, ButtonState state)
{
    Color bg_color;
    switch (state) {
        case BTN_NORMAL:  bg_color = WHITE; break;
        case BTN_HOVER:   bg_color = GRAY; break;
        case BTN_PRESSED: bg_color = (Color){0, 0, 0, 128}; break;
        default:          bg_color = WHITE; break;
    }
    
    Color text_color = BLACK;
    int text_font = button.width_ / 4;
    int text_width = MeasureText(text, text_font);
    int text_x = (button.width_ - text_width) / 2 + button.pos_.x;
    int text_y = (button.height_ - text_font) / 2 + button.pos_.y;

    DrawRectangle(button.pos_.x, button.pos_.y, button.width_, button.height_, bg_color);
    DrawText(text, text_x, text_y, text_font, text_color);
}

void draw_text()
{
    // 画倒计时数字
    int text_font = WINDOW_HEIGHT / 4;
    Color text_color = BLACK;
    const char* text = std::to_string(curr_count).c_str();
    int text_width = MeasureText(text, text_font);
    int text_x = (WINDOW_WIDTH - text_width) / 2;
    int text_y = (WINDOW_HEIGHT / 2 - text_font) / 2;
    DrawText(text, text_x, text_y, text_font, text_color);
}

void draw_paused()
{
    static std::string game_stat = "paused";
    Color message_color = RED;
    int font_size = WINDOW_HEIGHT / 8;
    int text_width = MeasureText(game_stat.c_str(), font_size);
    int x = (WINDOW_WIDTH - text_width) / 2;
    int y = (WINDOW_HEIGHT  - font_size) / 2;
    DrawText(game_stat.c_str(), x, y, font_size, message_color);
}

void update_game_stat()
{
    // 倒计时读秒
    if(passed_seconds(1))
    {
        curr_count--;
    }

    // 按下按钮
    if(button_clicked(btn_start))
    {
        running = true;
    }
    else if(button_clicked(btn_pause))
    {
        running = false;
    }
    else if(button_clicked(btn_reset))
    {
        running = false;
        reset = true;
        curr_count = time_out;
    }
}

void update_ui()
{
    draw_text();

    if(!running)
    {
        draw_paused();
    }
    
    struct ButtonData { const Button& btn; const char* label; };
    ButtonData btns[] = {
        {btn_start, "start"},
        {btn_pause, "pause"},
        {btn_reset, "reset"}
    };
    
    for (const auto& data : btns) {
        ButtonState state = get_button_state(data.btn);
        draw_button(data.btn, data.label, state);
    }
}

int main(int argc, char const *argv[])
{
    if(argc == 2)
    {
        time_out = atoi(argv[1]);
        curr_count = time_out;
    }

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(TARGET_FPS);
    
    while(!WindowShouldClose())
    {
        if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            if(IsWindowState(FLAG_WINDOW_UNDECORATED) && IsWindowState(FLAG_WINDOW_TOPMOST))
            {
                ClearWindowState(FLAG_WINDOW_UNDECORATED); // 恢复标题栏
                ClearWindowState(FLAG_WINDOW_TOPMOST);     // 取消窗口置顶
            }
            else
            {
                SetWindowState(FLAG_WINDOW_UNDECORATED);   // 隐藏标题栏
                SetWindowState(FLAG_WINDOW_TOPMOST);       // 窗口置顶
            }
        }

        update_game_stat();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        update_ui();

        EndDrawing();
    }
    
    return 0;
}
