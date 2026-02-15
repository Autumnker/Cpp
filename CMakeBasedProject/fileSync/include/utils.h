/*===============================================
*   文件名称：utils.h
*   创 建 者：hqyj
*   创建日期：2025年07月24日
*   描    述：常用工具函数
================================================*/
#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>

/*################################################错误处理相关#################################################*/

#define ERR_INT 1
#define ERR_PTR 0

/**
 * @brief 通用错误处理,判断上个函数的返回值是否达到了错误标准
 * @param ret_type 上个函数的返回值类型,分为 ERR_INT(int) 和 ERR_PTR(void *) 两种
 * @param ret_value 上个函数的返回值
 * @param fun_name 上个函数的函数名
 * @return int 触发错误返回 -1 否则返回 0
 */
int my_err(int ret_type, void *ret_value, const char *fun_name); // 通用错误处理

/*################################################文件路径相关#################################################*/

#define MAX_DIR_NAME 1024      // 文件夹名最大长度
#define DEFAULT_DIR_ROLE 0755  // 默认文件/文件夹权限

/**
 * @brief 返回可执行文件所在目录
 * @return static char* 可执行文件所在目录[静态变量]
 */
char *get_executable_dir();

/**
 * @brief 将src_file中的内容拷贝到dest_file中
 * @param src_path 源文件路径
 * @param dest_path 目标文件路径
 * @return int 成功返回 0 失败返回 -1
 */
int file_copy(const char *src_path, const char *dest_path);

/**
 * @brief 从一段长路径中截取后一段短路径("./dirA/dirB/fileC" - "./dirA" = "dirB/fileC")
 * @param path_longer 长路径,例如 "./dirA/dirB/fileC"
 * @param path_shorter 短路径,例如 "./dirA"
 * @return char* 成功返回 "dirB/fileC" 的起始地址, 失败返回 NULL
 */
char *path_subtract(const char *path_longer, const char *path_shorter);

/**
 * @brief 获取路径中最后一个文件/夹的名称
 * @param path_src 文件路径
 * @return char* 成功返回文件/文件夹名称首地址 失败返回 NULL
 */
char *getSrcFileName(const char *path_src);

/**
 * @brief 创建多级文件夹
 * @param path 文件夹路径
 * @return int 成功返回 0 失败返回 -1
 */
int mkdirs(const char *path);

/**
 * @brief 创造建多级文件夹下的文件,并将源文件中的内容复制到目标文件中
 * @param src_path 源文件路径
 * @param dest_path 目标文件路径
 * @return int 成功返回 0 失败返回 -1
 */
int mkdirsEndWithFile(const char *src_path, const char *dest_path);

/**
 * @brief 创建多级文件夹下的文件(创建的是空文件)
 * @param path 将要创建的文件路径
 * @return int 成功返回 0 失败返回 -1
 */
int mkdirsEndWithFile_Empty(const char *path);

/**
 * @brief 从当当前文件位置指针处截断文件
 * @param fp 将要被截断的文件流指针
 * @return 成功返回 0 失败返回 -1
 */
int truncateFileFromCurrPos(FILE *fp);

/*################################################JSON相关#################################################*/

/**
 * @brief 从文件中解析并提取JSON字段
 * @param json_path json文件的路径
 * @param object_name json对象的名字
 * @param key_name 键名
 * @param buffer 接收值的缓冲区
 * @param buffer_size 缓冲区大小
 * @return int 成功返回0 错误返回 -1 失败返回(没有相应数据) 1
 */
int parseJsonValue(const char *json_path, const char *object_name, const char *key_name,
                   char *buffer, size_t buffer_size);

#endif
