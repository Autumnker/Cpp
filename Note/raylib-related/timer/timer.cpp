/** 
 * 计时器概念版 - 并未包含软件架构层面的考虑
*/

#include <utility>
#include <string>
#include <chrono>
#include "raylib.h"

#define WINDOW_TITLE "Timer"
#define WINDOW_WIDTH (1920 / 5)
#define WINDOW_HEIGHT (1080 / 5)
#define TARGET_FPS 60

static int time_out = 1500;
static int curr_count = time_out;
static bool running = false;
static bool lock_btn = false;

struct Button
{
    enum ButtonState {
        BTN_NORMAL,
        BTN_HOVER,
        BTN_PRESSED,
        BTN_LOCKED,
        BTN_STATE_SIZE,
        BTN_UNKNOWN = BTN_STATE_SIZE
    };

    std::string label_;
    Vector2 pos_;
    float width_;
    float height_;

    Button():label_(""),pos_(std::move(Vector2{0.0f, 0.0f})),width_(0.0f),height_(0.0f){}
    Button(const char* label, Vector2 pos, float width, float height)
    : label_(label), pos_(pos),width_(width),height_(height){}
};


Button btn_start{"start", Vector2{WINDOW_WIDTH / 16 * 1, WINDOW_HEIGHT / 8 * 5}, WINDOW_WIDTH / 4, WINDOW_HEIGHT / 8 * 3};
Button btn_pause{"pause", Vector2{WINDOW_WIDTH / 16 * 6, WINDOW_HEIGHT / 8 * 5}, WINDOW_WIDTH / 4, WINDOW_HEIGHT / 8 * 3};
Button btn_reset{"reset", Vector2{WINDOW_WIDTH / 16 * 11, WINDOW_HEIGHT / 8 * 5}, WINDOW_WIDTH / 4, WINDOW_HEIGHT / 8 * 3};

const Button btns[] = {btn_start, btn_pause, btn_reset};

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

Button::ButtonState get_button_state(const Button& button)
{
    Vector2 mouse_pos = GetMousePosition();
    bool hover = is_hover(button, mouse_pos);
    bool pressed = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    
    if (!lock_btn && hover && pressed) return Button::ButtonState::BTN_PRESSED;
    if (lock_btn && hover) return Button::ButtonState::BTN_LOCKED;
    if (hover) return Button::ButtonState::BTN_HOVER;
    return Button::ButtonState::BTN_NORMAL;
}

void draw_button(const Button& button, const char* text, Button::ButtonState state)
{
    Color bg_color;
    switch (state) {
        case Button::ButtonState::BTN_NORMAL:  bg_color = WHITE; break;
        case Button::ButtonState::BTN_HOVER:   bg_color = GRAY; break;
        case Button::ButtonState::BTN_PRESSED: bg_color = (Color){0, 0, 0, 128}; break;
        case Button::ButtonState::BTN_LOCKED:  bg_color = (Color){200, 50, 50, 200}; break;
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

void draw_message(const std::string& msg)
{
    Color message_color = RED;
    int font_size = WINDOW_HEIGHT / 8;
    int text_width = MeasureText(msg.c_str(), font_size);
    int x = (WINDOW_WIDTH - text_width) / 2;
    int y = (WINDOW_HEIGHT  - font_size) / 2;
    DrawText(msg.c_str(), x, y, font_size, message_color);
}

void update_game_stat()
{
    // 倒计时读秒
    if(passed_seconds(1))
    {
        if(curr_count > 0)
        {
            curr_count--;
        }
        else
        {
            running = false;
        }
    }

    // 按下按钮

    if(get_button_state(btn_start) == Button::ButtonState::BTN_PRESSED)
    {
        running = true;
    }
    else if(get_button_state(btn_pause) == Button::ButtonState::BTN_PRESSED)
    {
        running = false;
    }
    else if(get_button_state(btn_reset) == Button::ButtonState::BTN_PRESSED)
    {
        running = false;
        curr_count = time_out;
    }
}

bool any_button_state(Button::ButtonState button_state)
{
    for(const auto& btn : btns)
    {
        auto state = get_button_state(btn);
        if(state == button_state)
        {
            return true;
        }
    }
    return false;
}

void update_ui()
{
    constexpr size_t BTN_COUNT = sizeof(btns) / sizeof(btns[0]);

    bool any_locked = false;
    Button::ButtonState btn_states[BTN_COUNT];
    
    for(int i = 0; i < BTN_COUNT; ++i)
    {
        Button::ButtonState stat = get_button_state(btns[i]);
        btn_states[i] = stat;
        if(stat == Button::ButtonState::BTN_LOCKED)
        {
            any_locked = true;
        }
    }

    draw_text();

    if(!running)
    {
        draw_message("Paused");
    }
    else if(any_locked)
    {
        draw_message("Buttons Locked");
    }
    
    for (int i = 0; i < BTN_COUNT; ++i) 
    {
        draw_button(btns[i], btns[i].label_.c_str(), btn_states[i]);
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
    SetExitKey(KEY_NULL);   // 默认按键"ESC"不退出
    
    while(!WindowShouldClose())
    {
        if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            if(IsWindowState(FLAG_WINDOW_UNDECORATED) && IsWindowState(FLAG_WINDOW_TOPMOST))
            {
                ClearWindowState(FLAG_WINDOW_UNDECORATED); // 恢复标题栏
                ClearWindowState(FLAG_WINDOW_TOPMOST);     // 取消窗口置顶
                lock_btn = false;
            }
            else
            {
                SetWindowState(FLAG_WINDOW_UNDECORATED);   // 隐藏标题栏
                SetWindowState(FLAG_WINDOW_TOPMOST);       // 窗口置顶
                lock_btn = true;
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
