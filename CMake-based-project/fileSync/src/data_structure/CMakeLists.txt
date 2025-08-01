cmake_minimum_required(VERSION 3.10)
project(data_structure)
set(CMAKE_CXX_STANDARD 11)

# 收集源文件
find_package(ZLIB REQUIRED)
file(GLOB DATA_STRUCTURE ${CMAKE_CURRENT_SOURCE_DIR}/data_structure.c)

# 生成动态库文件
set(LIBRARY_OUTPUT_PATH ${MY_LIB})
add_library(data_structure SHARED ${DATA_STRUCTURE})
target_link_libraries(data_structure PRIVATE ZLIB::ZLIB)