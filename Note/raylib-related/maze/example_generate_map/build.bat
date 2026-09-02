@echo off
setlocal enabledelayedexpansion
set "EXTRA_FLAGS=%*"

if not "!EXTRA_FLAGS!"=="" (
    echo 附加编译参数：!EXTRA_FLAGS!
    goto DO_BUILD
)

:: 无命令行参数，弹出菜单
choice /c:123q /m "[1]绘制未访问区域 [2]以擦除效果展示 [3]全选 [q]退出"
if !errorlevel! equ 4 goto SCRIPT_EXIT
if !errorlevel! equ 3 goto BOTH
if !errorlevel! equ 2 goto ERASE_EFFECT
if !errorlevel! equ 1 goto DRAW_NOT_VISITED

:DRAW_NOT_VISITED
set "EXTRA_FLAGS=!EXTRA_FLAGS! -DDRAW_NOT_VISITED"
goto DO_BUILD

:ERASE_EFFECT
set "EXTRA_FLAGS=!EXTRA_FLAGS! -DERASE_EFFECT"
goto DO_BUILD

:BOTH
set "EXTRA_FLAGS=!EXTRA_FLAGS! -DDRAW_NOT_VISITED -DERASE_EFFECT"
goto DO_BUILD

:DO_BUILD
echo 最终编译参数：!EXTRA_FLAGS!
g++ .\*.cpp ..\src\logger.cpp ^
-I ..\lib\raylib-6.0_win64_mingw-w64\include\ -I .\ ^
-I ..\include ^
-L ..\lib\raylib-6.0_win64_mingw-w64\lib\ ^
-lraylib -lopengl32 -lgdi32 -lwinmm ^
!EXTRA_FLAGS! ^
-o exe_example

:SCRIPT_EXIT
endlocal
