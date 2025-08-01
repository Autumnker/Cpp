cmake_minimum_required(VERSION 3.10)
project(fileSync)
set(CMAKE_CXX_STANDARD 11)

# 创建全局变量
set(MY_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(MY_DRC ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(MY_BIN ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(MY_LIB ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# 包含头文件目录
include_directories(${MY_INCLUDE})

# 收集子项目
add_subdirectory(src/main)
add_subdirectory(src/utils)
add_subdirectory(src/host)
add_subdirectory(src/socket/client)
add_subdirectory(src/socket/server)
add_subdirectory(src/data_structure)

