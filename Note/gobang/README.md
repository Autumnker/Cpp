# 五子棋 (Gobang)

基于 C++17 和 Raylib 图形库的五子棋双人对战游戏。

## 环境要求

- **编译器**: MinGW-w64 (gcc/g++)
- **CMake**: 3.10+
- **系统**: Windows 64位
- **依赖**: Raylib 5.5 (已内置在 `lib/` 目录)

## 快速开始

### 1. 生成并编译

```bash
# 方式一：使用 generate.bat（推荐）
generate.bat

# 方式二：手动 CMake
cmake -B build -G "MinGW Makefiles"
cmake --build build
```

### 2. 运行

```bash
./build/gobang.exe
```
或者
```bash
./bin/gobang.exe
```

## 操作说明

| 操作 | 按键/鼠标 |
|------|----------|
| 落子 | 鼠标左键点击棋盘交叉点 |
| 重新开始 | 键盘 `R` 或点击"新游戏"按钮 |
| 悔棋 | 键盘 `U` 或点击"悔棋"按钮 |
| 退出游戏 | 键盘 `ESC` 或点击"退出"按钮 |

## 项目结构

```
gobang/
├── CMakeLists.txt              # CMake 构建配置
├── generate.bat                # 一键生成脚本
├── README.md
├── doc/
│   └── requirements.md         # 需求文档
├── include/
│   ├── chess.hpp               # 棋盘逻辑
│   ├── player.hpp              # 玩家管理
│   ├── ui.hpp                  # 界面绘制
│   └── utils.hpp               # 工具函数
├── src/
│   ├── main.cpp                # 程序入口
│   ├── chess.cpp               # 棋盘实现
│   ├── player.cpp              # 玩家实现
│   ├── ui.cpp                  # 界面实现
│   └── utils.cpp               # 工具实现
├── lib/
│   ├── fast-cpp-csv-parser/
│   │   └── csv.h               # CSV 解析库
│   └── raylib-5.5_win64_mingw-w64/
│       ├── include/            # Raylib 头文件
│       └── lib/                # Raylib 库文件
└── data/
    └── games.csv               # 对局历史记录
```

## 功能特性

- ✅ 15×15 标准五子棋棋盘
- ✅ 黑白双方轮流落子
- ✅ 精确的五子连珠胜负判定
- ✅ 悔棋功能（Undo）
- ✅ 棋盘重置（新游戏）
- ✅ 鼠标悬停棋子预览
- ✅ 最后落子位置高亮
- ✅ 获胜五连高亮显示
- ✅ 胜场统计
- ✅ 对局历史 CSV 存储
- ✅ 9 个星位标记（含天元）

## 游戏规则

1. 黑棋先手，双方轮流在棋盘交叉点落子
2. 先在横、竖、斜任意方向形成连续五子者获胜
3. 棋盘下满 225 子则为平局
4. 对局结束后自动保存记录到 `data/games.csv`
