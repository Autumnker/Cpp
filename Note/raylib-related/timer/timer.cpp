/**
 * 计时器 - Cyber 风格 - 响应式布局 + 拖拽调整大小 + 滚轮等比缩放
 */

#include <utility>
#include <string>
#include <chrono>
#include <cstdlib>
#include <algorithm>
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define WINDOW_TITLE  "Timer"
#define WINDOW_WIDTH  (1920 / 8)
#define WINDOW_HEIGHT (1080 / 10)
#define TARGET_FPS    60

// ==================== 主题 ====================
struct Theme
{
    Color bg;
    Color text_primary;
    Color text_secondary;
    Color button_normal;
    Color button_hover;
    Color button_pressed;
    Color button_locked;
    Color button_border;
    Color button_text;
    Color message;
    Color handle;

    static Theme cyber()
    {
        Theme t;
        t.bg             = Color{   8,   8,  20, 255 };
        t.text_primary   = Color{   0, 255, 255, 255 };
        t.text_secondary = Color{  80, 180, 200, 255 };
        t.button_normal  = Color{  25,  15,  50, 255 };
        t.button_hover   = Color{  70,  35, 120, 255 };
        t.button_pressed = Color{ 140,  55, 200, 255 };
        t.button_locked  = Color{ 180,  40,  40, 220 };
        t.button_border  = Color{   0, 200, 255, 255 };
        t.button_text    = Color{   0, 240, 255, 255 };
        t.message        = Color{ 255,  50, 120, 255 };
        t.handle         = Color{   0, 200, 255, 180 };
        return t;
    }
};

// ==================== 布局 ====================
struct Layout
{
    Rectangle input_box;
    Rectangle buttons[3];
    Rectangle resize_handle;
    float content_center_y;    // 输入框 / 倒计时共享的垂直中心
    float message_center_y;    // 消息的垂直中心（顶部）
    int input_font_size;
    int button_font_size;
    int label_font_size;
    int message_font_size;
    int timer_font_size;

    static Layout compute(int w, int h)
    {
        Layout l{};
        const float fw = (float)w;
        const float fh = (float)h;

        // ── 输入框 / 倒计时：整体视觉中心
        //    稍高于画布正中（0.44h），为顶部标签和底部按钮留出呼吸感
        const float input_w  = fw * 0.60f;
        const float input_h  = fh * 0.22f;
        const float input_cy = fh * 0.44f;
        l.content_center_y = input_cy;
        l.input_box = {
            (fw - input_w) / 2.0f,
            input_cy - input_h / 2.0f,
            input_w, input_h
        };

        l.input_font_size = std::min((int)(input_h * 0.60f), 22);
        l.label_font_size = std::max(10, (int)(input_h * 0.48f));
        l.timer_font_size = h / 4;

        // ── 按钮：输入框下方，与输入框保持自然间距
        const float btn_w   = fw * 0.27f;
        const float btn_h   = fh * 0.20f;
        const float gap     = fw * 0.03f;
        const float total   = btn_w * 3.0f + gap * 2.0f;
        const float start_x = (fw - total) / 2.0f;
        const float btn_y   = l.input_box.y + input_h + fh * 0.08f;
        for (int i = 0; i < 3; ++i)
            l.buttons[i] = { start_x + i * (btn_w + gap), btn_y, btn_w, btn_h };
        l.button_font_size = std::max(10, (int)(btn_h * 0.38f));

        // ── 状态消息：顶部区域（避免与倒计时数字重叠）
        l.message_font_size = h / 8;
        l.message_center_y  = fh * 0.12f;

        // ── 右下角缩放手柄
        const float hs = std::max(12.0f, std::min(fw, fh) * 0.06f);
        l.resize_handle = { fw - hs, fh - hs, hs, hs };

        return l;
    }
};

// ==================== 按钮 ====================
struct Button
{
    std::string label;
    Button() : label("") {}
    Button(const char* lbl) : label(lbl) {}
};

// ==================== 计时器 ====================
class Timer
{
public:
    int  time_out;
    int  curr_count;
    bool running;

    Timer(int timeout = 1500)
        : time_out(timeout), curr_count(timeout), running(false),
          last_tick_(std::chrono::steady_clock::now()) {}

    void update()
    {
        if (tick_second() && curr_count > 0)
        {
            --curr_count;
            if (curr_count == 0) running = false;
        }
    }

    void reset() { running = false; curr_count = time_out; }

private:
    std::chrono::steady_clock::time_point last_tick_;

    bool tick_second()
    {
        const auto now = std::chrono::steady_clock::now();
        if (now - last_tick_ >= std::chrono::seconds(1))
        {
            if (running) { last_tick_ += std::chrono::seconds(1); return true; }
            last_tick_ = now;
        }
        return false;
    }
};

// ==================== 到时提醒音 ====================
// 倒计时归零后持续提醒：立即响一声，此后每 interval 秒再响一声。
// 音频设备不可用时静默降级（不加载、不响，也不影响计时功能）。
class DingNotifier
{
public:
    explicit DingNotifier(float interval_seconds = 3.0f)
        : interval_(std::chrono::duration_cast<Clock::duration>(
              std::chrono::duration<float>(interval_seconds))) {}

    void load(const char* path)
    {
        if (!IsAudioDeviceReady()) return;

        ding_   = LoadSound(path);
        loaded_ = IsSoundValid(ding_);
    }

    void unload()
    {
        if (!loaded_) return;
        active_ = false;
        UnloadSound(ding_);
        loaded_ = false;
    }

    // 幂等：每帧调用也只会在进入提醒状态的那一帧播第一声
    // start 内部立即播一声，后续由 update 按 interval_ 续播
    void start()
    {
        if (active_) return;

        active_ = loaded_;
        if (!active_) return;
        PlaySound(ding_);
        next_ding_ = Clock::now() + interval_;
    }

    void stop() { active_ = false; }

    bool active() const { return active_; }

    void update()
    {
        if (!active_) return;

        const Clock::time_point now = Clock::now();
        if (now < next_ding_) return;

        PlaySound(ding_);
        next_ding_ = now + interval_;
    }

private:
    using Clock = std::chrono::steady_clock;

    Sound             ding_{};
    bool              loaded_     = false;
    bool              active_     = false;
    Clock::duration   interval_;
    Clock::time_point next_ding_{};
};

// ==================== 应用状态 ====================
struct App
{
    Timer  timer;
    Button buttons[3] = { Button("START"), Button("PAUSE"), Button("RESET") };
    Theme  theme = Theme::cyber();
    DingNotifier ding;

    bool locked      = false;
    bool show_input  = true;
    bool input_edit  = false;
    int  input_value = 1500;

    // resize 状态
    bool    resizing          = false;
    Vector2 resize_anchor_g   = { 0, 0 };
    Vector2 resize_anchor_sz  = { 0, 0 };
};

// ==================== 鼠标工具 ====================
static Vector2 get_global_mouse()
{
    const Vector2 wp = GetWindowPosition();
    const Vector2 mp = GetMousePosition();
    return { wp.x + mp.x, wp.y + mp.y };
}

// ==================== 窗口大小拖拽 ====================
static void handle_resize(App& app, const Layout& layout)
{
    if (app.locked)
    {
        app.resizing = false;
        return;
    }

    const Vector2 mouse_win    = GetMousePosition();
    const Vector2 mouse_global = get_global_mouse();

    if (!app.resizing &&
        CheckCollisionPointRec(mouse_win, layout.resize_handle) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        app.resizing         = true;
        app.resize_anchor_g  = mouse_global;
        app.resize_anchor_sz = { (float)GetScreenWidth(), (float)GetScreenHeight() };
    }

    if (app.resizing)
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            const Vector2 delta = {
                mouse_global.x - app.resize_anchor_g.x,
                mouse_global.y - app.resize_anchor_g.y
            };
            const int new_w = std::max(200, (int)(app.resize_anchor_sz.x + delta.x));
            const int new_h = std::max(120, (int)(app.resize_anchor_sz.y + delta.y));

            if (new_w != GetScreenWidth() || new_h != GetScreenHeight())
                SetWindowSize(new_w, new_h);
        }
        else
        {
            app.resizing = false;
        }
    }
}

// ==================== 滚轮等比缩放 ====================
static void handle_zoom(App& app)
{
    if (app.locked || app.resizing) return;

    const float wheel = GetMouseWheelMove();
    if (wheel == 0.0f) return;

    const int w = GetScreenWidth();
    const int h = GetScreenHeight();

    const float factor = (wheel > 0.0f) ? 1.10f : (1.0f / 1.10f);

    // 初始固定比例
    constexpr float kBaseAspect =
        static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);

    // 最小尺寸（宽、高独立下限）
    constexpr int kMinW = 1920 / 12;
    constexpr int kMinH = 1080 / 12;

    // 按比例推算出"同时满足 MIN_W 和 MIN_H"的最小宽度
    const float min_w = std::max(static_cast<float>(kMinW),
                                 static_cast<float>(kMinH) * kBaseAspect);

    // 以宽度为基准缩放
    int new_w = static_cast<int>(w * factor);
    if (new_w < static_cast<int>(min_w))
        new_w = static_cast<int>(min_w);

    // 高度由宽度和比例唯一决定，天然保持比例
    int new_h = static_cast<int>(new_w / kBaseAspect);

    if (new_w == w && new_h == h) return;

    // 保持窗口中心不动
    const Vector2 win_pos = GetWindowPosition();
    const int new_x = static_cast<int>(win_pos.x + (w - new_w) / 2.0f);
    const int new_y = static_cast<int>(win_pos.y + (h - new_h) / 2.0f);

    SetWindowSize(new_w, new_h);
    SetWindowPosition(new_x, new_y);
}

// ==================== 输入处理 ====================
static void handle_input(App& app)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        if (IsWindowState(FLAG_WINDOW_UNDECORATED) &&
            IsWindowState(FLAG_WINDOW_TOPMOST))
        {
            ClearWindowState(FLAG_WINDOW_UNDECORATED);
            ClearWindowState(FLAG_WINDOW_TOPMOST);
            app.locked = false;
        }
        else
        {
            SetWindowState(FLAG_WINDOW_UNDECORATED);
            SetWindowState(FLAG_WINDOW_TOPMOST);
            app.locked = true;
            app.resizing = false;
        }
    }
}

// ==================== 按钮交互 ====================
static bool is_button_hovered(const Layout& layout, int idx)
{
    if (!IsCursorOnScreen()) return false;
    return CheckCollisionPointRec(GetMousePosition(), layout.buttons[idx]);
}
static bool is_button_clicked(const Layout& layout, int idx)
{
    return is_button_hovered(layout, idx) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}
static bool is_button_held(const Layout& layout, int idx)
{
    return is_button_hovered(layout, idx) && IsMouseButtonDown(MOUSE_BUTTON_LEFT);
}

// ==================== 逻辑更新 ====================
static void update_logic(App& app, const Layout& layout)
{
    app.timer.update();

    // 倒计时归零 → 进入提醒状态；START/RESET 使计数恢复正数后自动退出
    if (app.timer.curr_count == 0) app.ding.start();
    else                           app.ding.stop();
    app.ding.update();

    if (app.locked) return;

    if (is_button_clicked(layout, 0))         // START
    {
        if (app.show_input || app.timer.curr_count == 0)
        {
            // 全新开始：从输入框取新值
            app.input_edit       = false;
            app.timer.time_out   = app.input_value;
            app.timer.curr_count = app.timer.time_out;
            app.show_input       = false;
        }
        // 否则：从暂停恢复，curr_count 保持不变

        app.timer.running = true;
    }
    else if (is_button_clicked(layout, 1))    // PAUSE
    {
        app.timer.running = false;
    }
    else if (is_button_clicked(layout, 2))    // RESET
    {
        app.timer.reset();
        app.input_value = app.timer.time_out;
        app.show_input  = true;
    }
}

// ==================== 渲染 ====================
static void render_button(const App& app, const Layout& layout, int idx)
{
    const Rectangle r = layout.buttons[idx];
    const bool hovered = is_button_hovered(layout, idx);
    const bool held    = is_button_held(layout, idx);

    Color bg;
    if      (app.locked && hovered) bg = app.theme.button_locked;
    else if (held)                  bg = app.theme.button_pressed;
    else if (hovered)               bg = app.theme.button_hover;
    else                            bg = app.theme.button_normal;

    DrawRectangleRec(r, bg);
    DrawRectangleLinesEx(r, 1.0f, app.theme.button_border);

    const std::string& label = app.buttons[idx].label;
    const int font_size = layout.button_font_size;
    const int text_w    = MeasureText(label.c_str(), font_size);
    const int text_x    = static_cast<int>(r.x + (r.width  - text_w) / 2.0f);
    const int text_y    = static_cast<int>(r.y + (r.height - font_size) / 2.0f);

    DrawText(label.c_str(), text_x, text_y, font_size, app.theme.button_text);
}

static void render_input(App& app, const Layout& layout)
{
    // 标签：贴在输入框上方，间距约为标签高度的 30%
    const char* label = "TIMEOUT";
    const int lf = layout.label_font_size;
    const int lx = static_cast<int>(layout.input_box.x);
    const int ly = static_cast<int>(
        layout.input_box.y - lf - layout.input_box.height * 0.20f);

    DrawText(label, lx, ly, lf, app.theme.text_secondary);

    GuiSetStyle(DEFAULT, TEXT_SIZE, layout.input_font_size);
    if (GuiValueBox(layout.input_box, nullptr, &app.input_value, 1, 9999, app.input_edit))
        app.input_edit = !app.input_edit;
}

static void render_timer(const App& app, const Layout& layout)
{
    const char* text = TextFormat("%d", app.timer.curr_count);
    const int font_size = layout.timer_font_size;
    const int text_w    = MeasureText(text, font_size);
    const int x = (GetScreenWidth() - text_w) / 2;
    const int y = static_cast<int>(layout.content_center_y - font_size / 2.0f);

    DrawText(text, x, y, font_size, app.theme.text_primary);
}

static void render_message(const App& app, const Layout& layout, const char* msg)
{
    const int font_size = layout.message_font_size;
    const int text_w    = MeasureText(msg, font_size);
    const int x = (GetScreenWidth() - text_w) / 2;
    const int y = static_cast<int>(layout.message_center_y - font_size / 2.0f);

    DrawText(msg, x, y, font_size, app.theme.message);
}

static void render_resize_handle(const App& app, const Layout& layout)
{
    if (app.locked) return;

    const Rectangle r = layout.resize_handle;
    constexpr int kLines = 3;
    const float step = r.width / (kLines + 1);

    const bool hovered = CheckCollisionPointRec(GetMousePosition(), r);
    const Color color = hovered ? app.theme.message : app.theme.handle;

    for (int i = 1; i <= kLines; ++i)
    {
        const float off = step * i;
        DrawLineEx({ r.x + off,     r.y + r.height },
                   { r.x + r.width, r.y + off },
                   1.5f, color);
    }
}

static void render(App& app, const Layout& layout)
{
    if (app.show_input) render_input(app, layout);
    else                render_timer(app, layout);

    for (int i = 0; i < 3; ++i)
        render_button(app, layout, i);

    if (!app.timer.running && !app.show_input)
    {
        render_message(app, layout, "Paused");
    }
    else if (app.locked)
    {
        const bool any_hovered = is_button_hovered(layout, 0)
                              || is_button_hovered(layout, 1)
                              || is_button_hovered(layout, 2);
        if (any_hovered) render_message(app, layout, "Buttons Locked");
    }

    render_resize_handle(app, layout);
}

// ==================== 入口 ====================
int main(int argc, char const *argv[])
{
    App app;

    if (argc == 2)
    {
        const int t = std::atoi(argv[1]);
        if (t > 0)
        {
            app.timer.time_out   = t;
            app.timer.curr_count = t;
            app.input_value      = t;
        }
    }

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(TARGET_FPS);
    SetExitKey(KEY_NULL);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    InitAudioDevice();
    app.ding.load("resources\\ding.mp3");

    GuiLoadStyle("..\\lib\\raygui-5.0\\styles\\cyber\\style_cyber.rgs");

    Image icon = LoadImage("resources\\icon_timer.png");
    if (icon.data != nullptr)
    {
        ImageFormat(&icon, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
        SetWindowIcon(icon);
        UnloadImage(icon);
    }

    while (!WindowShouldClose())
    {
        const Layout layout = Layout::compute(GetScreenWidth(), GetScreenHeight());

        handle_input(app);
        handle_resize(app, layout);
        handle_zoom(app);              // ← 新增：滚轮等比缩放
        update_logic(app, layout);

        BeginDrawing();
        ClearBackground(app.theme.bg);
        render(app, layout);
        EndDrawing();
    }

    app.ding.unload();
    CloseAudioDevice();

    return 0;
}