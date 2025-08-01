/*===============================================
*   文件名称：socketDirSync_server.h
*   创 建 者：hqyj
*   创建日期：2025年07月26日
*   描    述：跨主机文件夹同步服务端(源端)相关函数
================================================*/
#ifndef _SOCKETDIRSYNC_SERVER_H
#define _SOCKETDIRSYNC_SERVER_H

/**
 * @brief 接收文件路径并创建/发送文件路径数组
 * @param client_fd 连接套接字
 * @param src_dir 源文件夹路径(传出参数)
 * @param path_arr 文件路径数组
 * @param file_count 文件数量
 * @return int 成功返回 0, 失败返回 -1
 */
int send_filePath_array(int client_fd, char *src_dir, char (*path_arr)[MAX_DIR_NAME], int *file_count);

#endif
