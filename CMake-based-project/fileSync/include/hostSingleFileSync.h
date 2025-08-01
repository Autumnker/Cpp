/*===============================================
*   文件名称：hostSingleFileSync.h
*   创 建 者：hqyj
*   创建日期：2025年07月25日
*   描    述：本地单个文件同步相关函数
================================================*/
#ifndef _HOSTSINGLEFILESYNC_H
#define _HOSTSINGLEFILESYNC_H

#include <stdio.h>
#include "data_structure.h"

/**
 * @brief 同步所需参数的结构体
 * @var fp          用于初始化线性表的文件流指针
 * @var bklist_ptr  块式线性表指针
 * @var buffer      用于文件流/线性表数据交互的缓冲区指针
 */
typedef struct sync_st
{
    Block_Info_List *bklist_ptr;
    FILE *fp;
    char *buffer;
} Sync_st;

/**
 * @brief 单次文件同步函数,单次源文件不变的情况下的同步函数
 * @param sync_src  用源文件初始化后的同步结构体指针
 * @param sync_dest 用目标文件初始化后的同步结构体指针
 * @return 成功返回 0 失败返回 -1
 */
int syncFile(Sync_st *sync_src, Sync_st *sync_dest);

/**
 * @brief 阻塞文件同步函数，文件状态变化一次(ctrl+s),文件被同步一次
 * @param path_src 源文件路径
 * @param sync_src  用源文件初始化后的同步结构体指针
 * @param sync_dest 用目标文件初始化后的同步结构体指针
 * @return int 成功返回 0 失败返回 -1
 */
int blockingSync(const char *path_src, Sync_st *sync_src, Sync_st *sync_dest, int quit_signal);

/**
 * @brief 执行单个文件同步的函数
 * @param path_src 源文件路径
 * @param path_dest 备份文件路径
 * @return int 成功返回 0 失败返回 -1
 */
int runSingleFileSync(const char *path_src, const char *path_dest);

#endif
