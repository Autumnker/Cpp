g++ main.cpp core\*.cpp render\*.cpp input\*.cpp clock\*.cpp app\*.cpp common\*.cpp ^
-I core -I render -I input -I clock -I app -I common ^
-I ..\lib\raylib-6.0_win64_mingw-w64\include\ ^
-L ..\lib\raylib-6.0_win64_mingw-w64\lib\ ^
-lraylib -lopengl32 -lgdi32 -lwinmm ^
-o maze
