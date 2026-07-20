@echo off
chcp 65001 >nul
echo ==============================
echo  五子棋 Gobang - 构建脚本
echo ==============================

:: 确保 data 目录存在
if not exist "data" (
    echo ---创建 data 目录...
    mkdir data
)

:: 如果存在 build 目录，删除它
if exist "build" (
    echo ---正在删除旧的 build 目录...
    rd /s /q "build"
)

:: 创建并进入 build
mkdir build
cd build

:: 执行 CMake 配置
echo ---执行 cmake 配置...
cmake .. -G "MinGW Makefiles"

:: 执行编译
echo.
echo ---开始编译...
cmake --build .

echo.
echo ==============================
echo  构建完成!
echo  运行: build\gobang.exe
echo ==============================

echo 执行 make
make

:: 回到上级目录
cd ..

echo.
echo ==============================
echo  构建完成！
echo ==============================
