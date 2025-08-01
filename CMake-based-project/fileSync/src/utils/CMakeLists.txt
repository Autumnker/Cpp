cmake_minimum_required(VERSION 3.10)
project(utils)
set(CMAKE_CXX_STANDARD 11)


# 收集源文件
file(GLOB UTILS ${CMAKE_CURRENT_SOURCE_DIR}/utils.c)

# 生成动态库文件
set(LIBRARY_OUTPUT_PATH ${MY_LIB})
add_library(utils SHARED ${UTILS})