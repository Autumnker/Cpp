cmake_minimum_required(VERSION 3.10)
project(gobang)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# 强制源文件和输出编码为 UTF-8（解决 Windows 中文系统下 GBK/UTF-8 混乱）
add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)

# 设置库路径
set(RAYLIB_DIR ${CMAKE_SOURCE_DIR}/lib/raylib-5.5_win64_mingw-w64)

# 包含目录
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${RAYLIB_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib/fast-cpp-csv-parser
)

# 链接库目录
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
    winmm       # Windows 多媒体库（Raylib 依赖）
    gdi32       # Windows GDI（Raylib 依赖）
)

# 解决 mingw 下 __declspec(dllimport) 警告
target_compile_definitions(gobang PRIVATE RAYLIB_DLL)

# 复制 DLL 到输出目录
add_custom_command(TARGET gobang POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${RAYLIB_DIR}/lib/raylib.dll
    $<TARGET_FILE_DIR:gobang>
)

# 创建 data 目录
add_custom_command(TARGET gobang POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
    ${CMAKE_SOURCE_DIR}/data
)

# 设置项目输出目录
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
