g++ timer.cpp ^
-I ..\lib\raylib-6.0_win64_mingw-w64\include\ ^
-L ..\lib\raylib-6.0_win64_mingw-w64\lib\ ^
-lraylib -lopengl32 -lgdi32 -lwinmm ^
-o timer
