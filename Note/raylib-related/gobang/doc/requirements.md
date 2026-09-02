# 项目名称“gobang”五子棋
# 五子棋游戏 - 项目提示词

## 项目概述
开发一个基于 C++ 和 Raylib 图形库的五子棋（Gomoku）双人对战游戏。游戏需要在 Windows 平台下编译运行，使用 CMake 作为构建系统。

## 技术栈要求
- **语言标准**: C++17 (gnu++17)
- **图形库**: Raylib 5.5 (Windows 64位，MinGW-w64 版本)
- **构建工具**: CMake 3.10+
- **编译器**: MinGW-w64 (gcc/g++)
- **平台**: Windows 64位

## 功能需求

### 1. 核心游戏逻辑 (chess.cpp/hpp)
- 实现 15×15 标准五子棋棋盘
- 棋子放置逻辑（黑白双方轮流落子）
- 胜负判定算法（横、竖、斜五个方向）
- 棋盘状态管理（二维数组）
- 落子历史记录（支持悔棋功能）
- 棋盘重置功能

### 2. 玩家管理 (player.cpp/hpp)
- 支持双人模式（玩家1 vs 玩家2）
- 当前玩家切换逻辑
- 玩家分数统计
- 玩家名称输入（可选）

### 3. UI 界面 (ui.cpp/hpp)
- 基于 Raylib 绘制游戏界面
- 棋盘绘制（网格线、星位、棋子）
- 交互响应（鼠标点击、键盘事件）
- 游戏状态显示（当前玩家、获胜信息）
- 菜单/按钮系统（开始新游戏、悔棋、退出）
- 高分/历史记录显示（使用 CSV 存储）

### 4. 工具模块 (utils.cpp/hpp)
- 文件读写工具（CSV 格式使用 fast-cpp-csv-parser）
- 时间戳生成
- 配置管理（读取/保存游戏设置）
- 日志记录
- 如果fast-cpp-csv-parser中的csv库功能不完整则补充剩余需要的逻辑

### 5. 数据持久化
- 使用 `lib/fast-cpp-csv-parser/csv.h` 读写 CSV 文件
- 存储内容：对局历史（日期、玩家、步数、胜负）
- 存储位置：项目根目录下的 `data/games.csv`

## 项目结构要求

```
gobang/
├── CMakeLists.txt          # CMake 构建配置
├── doc/
│   └── requirements.md     # 本需求文档
├── include/                # 头文件 (.hpp)
│   ├── chess.hpp
│   ├── player.hpp
│   ├── ui.hpp
│   └── utils.hpp
├── lib/                    # 第三方库
│   └── fast-cpp-csv-parser/
│       └── csv.h           # CSV 解析器（单头文件）
│   └── raylib-5.5_win64_mingw-w64/
│       ├── include/
│       │   ├── raylib.h
│       │   ├── raymath.h
│       │   └── rlgl.h
│       └── lib/
│           ├── libraylib.a
│           ├── libraylibdll.a
│           └── raylib.dll
├── src/                    # 源文件 (.cpp)
│   ├── main.cpp
│   ├── chess.cpp
│   ├── player.cpp
│   ├── ui.cpp
│   └── utils.cpp
└── data/                   # 数据文件（运行时生成）
    └── games.csv
```

## CMake 配置要求

```cmake
cmake_minimum_required(VERSION 3.10)
project(gobang)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置库路径
set(RAYLIB_DIR ${CMAKE_SOURCE_DIR}/lib/raylib-5.5_win64_mingw-w64)

# 包含目录
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${RAYLIB_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib/fast-cpp-csv-parser
)

# 链接库
link_directories(${RAYLIB_DIR}/lib)

# 可执行文件
add_executable(gobang 
    src/main.cpp
    src/chess.cpp
    src/player.cpp
    src/ui.cpp
    src/utils.cpp
)

# 链接库
target_link_libraries(gobang 
    raylib
    winmm    # Windows 多媒体库（Raylib 依赖）
    gdi32    # Windows GDI（Raylib 依赖）
)

# 复制 DLL 到输出目录
add_custom_command(TARGET gobang POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${RAYLIB_DIR}/lib/raylib.dll
    $<TARGET_FILE_DIR:gobang>
)

# 创建数据目录
add_custom_command(TARGET gobang POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
    ${CMAKE_SOURCE_DIR}/data
)
```

## 界面设计要求

### 主界面
- 窗口标题: "五子棋 - Gobang"
- 窗口大小: 800×700 像素（可调整）
- 背景颜色: 柔和木色 (#DEB887 或类似)
- 棋盘居中显示，留出边距用于信息展示

### 棋盘绘制
- 15×15 网格，使用深棕色线条
- 格子大小: 35-40 像素
- 星位: 天元(7,7)，小目(3,3)、(3,11)、(11,3)、(11,11)
- 棋子: 黑子（实心圆），白子（空心圆带边框）

### 交互方式
- 鼠标左键点击: 落子
- 键盘 R: 重新开始
- 键盘 U: 悔棋 (Undo)
- 键盘 ESC: 退出游戏

### 信息显示
- 当前回合: "黑棋回合" / "白棋回合"
- 游戏状态: "游戏进行中" / "黑棋胜!" / "白棋胜!" / "平局"
- 玩家分数: 显示双方胜场数
- 步数统计: 当前对局已下步数

## 实现要求

### 编码规范
- 使用面向对象编程思想
- 类名使用大写驼峰 (PascalCase)
- 成员变量使用 `m_` 前缀或下划线后缀
- 函数名使用小写驼峰 (camelCase)
- 合理使用 const 和 noexcept
- 添加必要的注释（特别是关键算法）

### 算法要求
- 胜负判定: 从落子位置向四个方向（水平、垂直、主对角线、副对角线）检查
- 优化: 只检查以落子为中心的周围区域，不遍历整个棋盘
- 使用方向向量数组: `{{1,0}, {0,1}, {1,1}, {1,-1}}`

### 用户体验
- 落子时有音效反馈（可选）
- 鼠标悬停时显示半透明棋子预览
- 高亮最后一次落子位置
- 获胜时高亮五子连珠

### 代码文件依赖关系
```
main.cpp
  ├── ui.hpp
  │   ├── chess.hpp
  │   ├── player.hpp
  │   └── utils.hpp
  ├── chess.hpp
  ├── player.hpp
  └── utils.hpp
      └── fast-cpp-csv-parser/csv.h
```

## 测试要点
1. 棋盘初始化是否正确
2. 落子交替是否正常（黑先白后）
3. 胜负判定是否准确（测试五个方向）
4. 禁止在已有棋子的位置落子
5. 悔棋功能是否撤销最后一步
6. 游戏结束后禁止继续落子
7. CSV 文件读写是否正常
8. 窗口关闭是否释放资源

## 扩展功能（可选加分项）
- AI 对战模式（简单权重算法）
- 计时器（每步限时）
- 游戏存档/读档
- 棋盘主题切换（多种配色方案）
- 网络对战（后续版本）

## 编译运行指令
```bash
# 配置
cmake -B build -G "MinGW Makefiles"

# 编译
cmake --build build

# 运行
./build/gobang.exe
```

## 注意事项
1. **不要**将 `raylib.dll` 添加到版本控制（使用 .gitignore）
2. 确保 `csv.h` 正确包含在项目中
3. 所有路径使用相对路径
4. 代码需要兼容 Windows 和 Linux（使用 CMake 条件编译）
5. 防止内存泄漏（使用智能指针或确保析构函数正确）
6. 添加合理的错误处理（文件打开失败、内存分配失败等）

---

## 期望交付物
1. 完整的源代码（所有 .cpp/.hpp 文件）
2. 可正常编译的 CMakeLists.txt
3. 编译后的可执行文件（gobang.exe）
4. 包含 CSV 示例数据的 data/games.csv
5. 简要的使用说明（README.md）

---

**生成时间**: 2026-07-11
**目标 AI**: Deep Seek V4 Pro
**项目类型**: 桌面游戏开发