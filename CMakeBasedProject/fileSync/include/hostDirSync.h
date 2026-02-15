/*===============================================
*   文件名称：hostDirSync.h
*   创 建 者：hqyj
*   创建日期：2025年07月26日
*   描    述：本地文件夹同步相关函数
================================================*/
#ifndef _HOSTDIRSYNC_H
#define _HOSTDIRSYNC_H

#include "utils.h"

#define MAX_FILE_NAME_SIZE 256 // 文件名字的长度

/**
 * @brief 获取源文件夹中所有文件的路径并赋值给"path_arr",并将文件数量通过"file_count"传出
 * @param path_src 源文件夹路径
 * @param path_arr 文件路径数组
 * @param file_count 记录文件数量的变量
 * @return int 成功返回 0 失败返回 -1
 */
int getFilePath(const char *path_src, char (*path_arr)[MAX_DIR_NAME], int *file_count);

/**
 * @brief 将源文件路径映射到目标文件路径上
 * @param src_path_arr 源文件路径数组
 * @param dest_path_arr 目标文件路径数组(传出参数)
 * @param src_head_path 源同步文件夹的根路径
 * @param dest_head_path 目标文件夹的根路径
 * @param file_count 源同步文件数量
 * @return int 成功返回 0 失败返回 -1
 */
int srcPathToDestPath(char (*src_path_arr)[MAX_DIR_NAME], char (*dest_path_arr)[MAX_DIR_NAME], const char *src_head_path, const char *dest_head_path, int file_count);

/**
 * @brief 创建目标文件/文件夹并同步文件内容
 * @param src_path_arr 源文件/文件夹路径数组
 * @param dest_path_src 目标文件/文件夹路径数组
 * @param file_count 文件数量
 * @return int 成功返回 0 失败返回 -1
 */
int createDirs(const char (*src_path_arr)[MAX_DIR_NAME], const char (*dest_path_arr)[MAX_DIR_NAME], int file_count);

/**
 * @brief 打印文件路径数组中的元素
 * @param path_arr 文件路径数组首地址
 * @param file_count 文件数量
 * @return void
 */
void pathArrPrient(char (*path_arr)[MAX_DIR_NAME], int file_count);

#endif
