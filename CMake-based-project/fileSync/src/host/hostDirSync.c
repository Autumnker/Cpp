/*===============================================
*   文件名称：hostDirSync.c
*   创 建 者：hqyj
*   创建日期：2025年07月26日
*   描    述：本地文件夹同步相关函数
================================================*/

#include <string.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include "../../include/hostDirSync.h"
#include "../../include/data_structure.h"

int getFilePath(const char *path_src, char (*path_arr)[MAX_DIR_NAME], int *file_count)
{
    // 初始化文件名栈
    Dir_Stack *dstack = dstack_init();

    int ret = -1;                         // 返回值接收变量
    *file_count = 0;                      // 文件数量记录变量
    char topDir[MAX_DIR_NAME] = {'\0'};   // 记录栈顶文件夹名称
    char fullPath[MAX_DIR_NAME] = {'\0'}; // 记录完整文件路径名称
    DIR *fdir = NULL;                     // 目录流指针
    struct dirent *entry = NULL;          // 文件条目
    struct stat statbuf = {0};            // 文件状态

    // 将源文件夹根目录压入栈中
    ret = dstack_push(dstack, path_src);
    ret = my_err(ERR_INT, &ret, "getFilePath/dstack_push");
    if (-1 == ret)
    {
        dstack_delete(dstack);
        return -1;
    }

    // 迭代收集文件路径数据并赋值给文件路径数组
    while (dstack_empty(dstack) == 0)
    {
        // 弹出栈顶
        memset(topDir, '\0', MAX_DIR_NAME);
        ret = dstack_pop(dstack, topDir);
        ret = my_err(ERR_INT, &ret, "getFilePath/dstack_top");
        if (-1 == ret)
        {
            dstack_delete(dstack);
            return -1;
        }

        // 打开栈顶文件夹
        fdir = opendir(topDir);
        my_err(ERR_PTR, fdir, "getFilePath/opendir");

        // 读取条目
        while ((entry = readdir(fdir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") == 0 ||
                strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }

            // 构建完整路径
            memset(fullPath, '\0', MAX_DIR_NAME);

            strcat(fullPath, topDir);
            strcat(fullPath, "/");
            strcat(fullPath, entry->d_name);

            // 获取文件属性
            ret = stat(fullPath, &statbuf);
            ret = my_err(ERR_INT, &ret, "getFilePath/stat");
            if (-1 == ret)
            {
                dstack_delete(dstack);
                return -1;
            }

            // 判断条目类型
            if (S_ISDIR(statbuf.st_mode)) // 目录
            {
                ret = dstack_push(dstack, fullPath);
                ret = my_err(ERR_INT, &ret, "getFilePath/dstack_push");
                if (-1 == ret)
                {
                    dstack_delete(dstack);
                    return -1;
                }
            }
            else if (S_ISREG(statbuf.st_mode)) // 文件
            {
                strcpy(path_arr[*file_count], fullPath);
                *file_count += 1;
            }
        }
    }

    // 回收资源
    dstack_delete(dstack);

    return 0;
}

int srcPathToDestPath(char (*src_path_arr)[MAX_DIR_NAME], char (*dest_path_arr)[MAX_DIR_NAME], const char *src_head_path, const char *dest_head_path, int file_count)
{
    if (src_path_arr == NULL)
    {
        printf("srcPathToDestPath: src_path_arr = NULL\n");
        return -1;
    }
    if (dest_path_arr == NULL)
    {
        printf("srcPathToDestPath: dest_path_arr = NULL\n");
        return -1;
    }

    // 将源文件路径映射为目标文件路径
    int ret = -1;
    char *tail_path = NULL;
    for (int i = 0; i < file_count; ++i)
    {
        strcat(dest_path_arr[i], dest_head_path);
        strcat(dest_path_arr[i], "/");
        tail_path = path_subtract(src_path_arr[i], src_head_path);
        ret = my_err(ERR_INT, tail_path, "srcPathToDestPath/path_subtract");
        if (-1 == ret)
        {
            return -1;
        }
        strcat(dest_path_arr[i], tail_path);
    }

    return 0;
}

int createDirs(const char (*src_path_arr)[MAX_DIR_NAME], const char (*dest_path_arr)[MAX_DIR_NAME], int file_count)
{
    if (dest_path_arr == NULL)
    {
        printf("createDirs: dest_path_arr = NULL\n");
        return -1;
    }
    if (file_count <= 0)
    {
        printf("createDirs: file_count <= 0\n");
        return -1;
    }

    // 遍历路径并创建文件夹
    for (int i = 0; i < file_count; ++i)
    {
        if (-1 == mkdirsEndWithFile(src_path_arr[i], dest_path_arr[i]))
        {
            return -1;
        }
    }

    return 0;
}

void pathArrPrient(char (*path_arr)[MAX_DIR_NAME], int file_count)
{
    if (path_arr == NULL)
    {
        printf("pathArrPrient: path_arr = NULL\n");
        return;
    }
    if (file_count <= 0)
    {
        printf("pathArrPrient: file_count = 0\n");
        return;
    }

    for (int i = 0; i < file_count; ++i)
    {
        printf("%s\n", path_arr[i]);
    }
}
