/*===============================================
*   文件名称：socketSingleFileSync_client.h
*   创 建 者：hqyj
*   创建日期：2025年07月26日
*   描    述：跨主机单个文件同步客户端(同步端)相关函数
================================================*/
#ifndef _SOCKETSINGLEFILESYNC_CLIENT_H
#define _SOCKETSINGLEFILESYNC_CLIENT_H

/**
 * @brief 初始化客户端套接字
 * @param server_ip 将要连接的服务器ip
 * @param server_port 将要连接的服务器端口
 * @return int 成功返回套接字,失败返回-1
 */
int client_socket_init(const char *server_ip, const char *server_port);

/**
 * @brief 发送源文件路径并接收响应
 * @param client_fd 连接套接字
 * @param path 源文件路径
 * @return int 服务器成功打开源文件返回 0 ,失败返回 -1
 */
int send_sourceFile_path(int client_fd, const char *path);

/**
 * @brief 创建同步文件
 * @param path 同步文件路径
 * @param mode 文件打开方式
 * @return FILE* 成功返回文件流指针,失败返回 NULL
 */
FILE *create_sync_file(const char *path, const char *mode);

/**
 * @brief 同步源文件内容
 * @param client_fd 连接套接字
 * @param fp_dest 同步文件流指针
 * @return int 成功返回 0, 失败返回 -1
 */
int recv_content_from_server(int client_fd, FILE *fp_dest);

#endif
