cmake_minimum_required(VERSION 3.10)
project(client)
set(CMAKE_CXX_STANDARD 11)

# 收集源文件
file(GLOB SOCKET_DIR_SYNC_CLIENT ${CMAKE_CURRENT_SOURCE_DIR}/socketDirSync_client.c)
file(GLOB SOCKET_SINGLEFILE_SYNC_CLIENT ${CMAKE_CURRENT_SOURCE_DIR}/socketSingleFileSync_client.c)

# 动态库文件夹
link_directories(${MY_LIB})

# 生成动态库文件
set(LIBRARY_OUTPUT_PATH ${MY_LIB})
add_library(socketSingleFileSync_client SHARED ${SOCKET_SINGLEFILE_SYNC_CLIENT})
target_link_libraries(socketSingleFileSync_client utils data_structure cjson)

add_library(socketDirSync_client SHARED ${SOCKET_DIR_SYNC_CLIENT})
target_link_libraries(socketDirSync_client utils data_structure cjson hostDirSync)