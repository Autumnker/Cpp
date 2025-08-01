cmake_minimum_required(VERSION 3.10)
project(hostDirSync)
set(CMAKE_CXX_STANDARD 11)


# 收集源文件
file(GLOB HOST_DIR_SYNC ${CMAKE_CURRENT_SOURCE_DIR}/hostDirSync.c)
file(GLOB HOST_SINGLEFILE_SYNC ${CMAKE_CURRENT_SOURCE_DIR}/hostSingleFileSync.c)

# 生成动态库文件
set(LIBRARY_OUTPUT_PATH ${MY_LIB})
add_library(hostDirSync SHARED ${HOST_DIR_SYNC})
add_library(hostSingleFileSync SHARED ${HOST_SINGLEFILE_SYNC})