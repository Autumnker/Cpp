/*===============================================
*   文件名称：socketSingleFileSync_server.h
*   创 建 者：hqyj
*   创建日期：2025年07月26日
*   描    述：跨主机单个文件同步服务端(源端)相关函数
================================================*/
#ifndef _SOCKETSINGLEFILESYNC_SERVER_H
#define _SOCKETSINGLEFILESYNC_SERVER_H

#define BACKLOG 20 // 最大客户端连接数量

/**
 * @brief 初始化服务器套接字
 * @param server_ip 服务器ip
 * @param server_port 服务器端口
 * @return int 成功返回套接字,失败返回-1
 */
int server_socket_init(const char *server_ip, const char *server_port);

/**
 * @brief 接收文件路径并打开该文件
 * @param client_fd 连接套接字
 * @param mode 文件打开方式
 * @param (*path)[MAX_DIR_NAME] 用于存储源文件路径的字符串数组指针(传出参数)
 * @return FILE* 成功返回文件流指针, 失败返回NULL
 */
FILE *open_file_by_client(int client_fd, const char *mode, char (*path)[MAX_DIR_NAME]);

/**
 * @brief 将指定路径中的文件内容发送给客户端
 * @param client_fd 连接套接字
 * @param src_fp 源文件流指针
 * @return int 成功返回 0, 失败返回 -1
 */
int send_content_to_client(int client_fd, FILE *src_fp);

/**
 * @brief 监听源文件状态变化(CTRL + S)
 * @param path 源文件路径
 * @return int 监听到状态变化返回 1, 否则返回 0
 */
int listenSyncSingle(const char *path);

#endif
