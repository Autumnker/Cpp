:: 用 windres 命令将 .rc 文件编译成 .o 对象文件
windres resources\app.rc -O coff -o app.res.o

g++ main.cpp app.res.o core\*.cpp render\*.cpp input\*.cpp clock\*.cpp app\*.cpp common\*.cpp ^
-I .\ -I core -I render -I input -I clock -I app -I common ^
-I ..\lib\raylib-6.0_win64_mingw-w64\include\ ^
-L ..\lib\raylib-6.0_win64_mingw-w64\lib\ ^
-lraylib -lopengl32 -lgdi32 -lwinmm ^
-o maze
