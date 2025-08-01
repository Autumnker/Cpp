cmake_minimum_required(VERSION 3.10)
project(server)
set(CMAKE_CXX_STANDARD 11)

# 收集源文件
file(GLOB SOCKET_DIR_SYNC_SERVER ${CMAKE_CURRENT_SOURCE_DIR}/socketDirSync_server.c)
file(GLOB SOCKET_SINGLEFILE_SYNC_SERVER ${CMAKE_CURRENT_SOURCE_DIR}/socketSingleFileSync_server.c)

# 动态库文件夹
link_directories(${MY_LIB})

# 生成动态库文件
set(LIBRARY_OUTPUT_PATH ${MY_LIB})
add_library(socketSingleFileSync_server SHARED ${SOCKET_SINGLEFILE_SYNC_SERVER})
target_link_libraries(socketSingleFileSync_server utils data_structure cjson)

add_library(socketDirSync_server SHARED ${SOCKET_DIR_SYNC_SERVER})
target_link_libraries(socketDirSync_server utils hostDirSync data_structure cjson)