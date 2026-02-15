cmake_minimum_required(VERSION 3.10)
project(main)
set(CMAKE_CXX_STANDARD 11)


# 收集源文件
file(GLOB HOST_SINGLEFILE_SYNC_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/hostSingleFileSync_main.c)
file(GLOB HOST_DIR_SYNC_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/hostDirSync_main.c)
file(GLOB SOCKET_SINGLEFILE_SYNC_SERVER_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/socketSingleFileSync_server_main.c)
file(GLOB SOCKET_SINGLEFILE_SYNC_CLIENT_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/socketSingleFileSync_client_main.c)
file(GLOB SOCKET_DIR_SYNC_SERVER_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/socketDirSync_server_main.c)
file(GLOB SOCKET_DIR_SYNC_CLIENT_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/socketDirSync_client_main.c)

# 连接动态库目录
link_directories(${MY_LIB})

# 设置可执行文件输出目录
set(EXECUTABLE_OUTPUT_PATH ${MY_BIN})

# 生成可执行文件
# 本地单个文件同步
add_executable(hostSingleFileSync_main ${HOST_SINGLEFILE_SYNC_MAIN})
target_link_libraries(hostSingleFileSync_main hostSingleFileSync data_structure utils cjson)
# 本地文件夹同步
add_executable(hostDirSync_main ${HOST_DIR_SYNC_MAIN})
target_link_libraries(hostDirSync_main hostDirSync hostSingleFileSync data_structure utils cjson)
# 跨主机单个文件同步服务器
add_executable(socketSingleFileSync_server_main ${SOCKET_SINGLEFILE_SYNC_SERVER_MAIN})
target_link_libraries(socketSingleFileSync_server_main utils data_structure socketSingleFileSync_server cjson)
# 跨主机单个文件同步客户端
add_executable(socketSingleFileSync_client_main ${SOCKET_SINGLEFILE_SYNC_CLIENT_MAIN})
target_link_libraries(socketSingleFileSync_client_main utils data_structure socketSingleFileSync_client cjson)
# 跨主机文件夹同步客户端
add_executable(socketDirSync_client_main ${SOCKET_DIR_SYNC_CLIENT_MAIN})
target_link_libraries(socketDirSync_client_main utils data_structure socketDirSync_client socketSingleFileSync_client)
# 跨主机文件夹同步服务器
add_executable(socketDirSync_server_main ${SOCKET_DIR_SYNC_SERVER_MAIN})
target_link_libraries(socketDirSync_server_main utils data_structure socketDirSync_client socketDirSync_server socketSingleFileSync_client socketSingleFileSync_server)
