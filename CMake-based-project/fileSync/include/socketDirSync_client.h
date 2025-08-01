/*===============================================
*   文件名称：socketDirSync_client.h
*   创 建 者：hqyj
*   创建日期：2025年07月26日
*   描    述：跨主机文件夹同步客户端(同步端)相关函数
================================================*/
#ifndef _SOCKETDIRSYNC_CLIENT_H
#define _SOCKETDIRSYNC_CLIENT_H

/**
 * @brief 发送源文件夹路径并获得源文件路径数组
 * @param client_fd 连接套接字
 * @param src_dir 源文件夹路径
 * @param src_path_arr 源文件路径数组(传出参数)
 * @param file_count 文件路径数量(传出参数)
 * @return int 成功返回 0, 失败返回 -1
 */
int recv_filePath_array(int client_fd, const char *src_dir, char (*src_path_arr)[MAX_DIR_NAME], int *file_count);

/**
 * @brief 创建同步文件夹/文件
 * @param src_path_arr 源文件路径数组
 * @param dest_path_arr 目标文件路径数组
 * @param src_dir 源文件夹路径
 * @param dest_dir 目标文件夹路径
 * @param file_count 源文件数量(源文件路径数组大小)
 * @return int 成功返回 0, 失败返回 -1
 */
int createSyncDirs(char (*src_path_arr)[MAX_DIR_NAME], char (*dest_path_arr)[MAX_DIR_NAME],
                   const char *src_dir, const char *dest_dir, int file_count);

#endif
