/*===============================================
*   文件名称：utils.c
*   创 建 者：hqyj
*   创建日期：2025年07月24日
*   描    述：常用工具函数
================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../../include/cJSON.h"
#include "../../include/utils.h"
#include "../../include/hostSingleFileSync.h"

// 通用错误处理
int my_err(int ret_type, void *ret_value, const char *fun_name)
{
    if (ret_type == ERR_INT && *(int *)ret_value == -1 ||
        ret_type == ERR_PTR && ret_value == NULL)
    {
        perror(fun_name);
        return -1;
    }
    else
    {
        return 0;
    }
}

// 返回可执行文件所在目录
char *get_executable_dir()
{
    static char path[MAX_DIR_NAME];
    ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if (len == -1)
    {
        return NULL;
    }
    path[len] = '\0';
    return dirname(path);
}

// 拷贝文件内容
int file_copy(const char *src_path, const char *dest_path)
{
    if (src_path == NULL)
    {
        printf("file_copy: src_path = NULL\n");
        return -1;
    }
    if (dest_path == NULL)
    {
        printf("file_copy: dest_path = NULL\n");
        return -1;
    }

    // 打开文件
    FILE *fp_src = fopen(src_path, "r");
    if (-1 == my_err(ERR_PTR, fp_src, "file_copy/fopen: fp_src"))
    {
        return -1;
    }

    FILE *fp_dest = fopen(dest_path, "w");
    if (-1 == my_err(ERR_PTR, fp_dest, "file_copy/fopen fd_dest"))
    {
        return -1;
    }

    // 拷贝
    char buf[BUFFER_SIZE] = {'\0'};
    int ret = 0;
    while (0 != (ret = fread(buf, sizeof(char), BUFFER_SIZE, fp_src)))
    {
        if (-1 == fwrite(buf, sizeof(char), ret, fp_dest))
        {
            perror("file_copy/fwrite: ret = -1");
            fclose(fp_dest);
            fclose(fp_src);
            return -1;
        }
    }

    fclose(fp_dest);
    fclose(fp_src);

    return 0;
}

// 路径截取
char *path_subtract(const char *path_longer, const char *path_shorter)
{
    // 检查path_longer是否以path_shorter为前缀
    if (strstr(path_longer, path_shorter) != path_longer)
    {
        return NULL;
    }

    // 返回指针
    return (char *)(path_longer + strlen(path_shorter) + 1);
}

// 获取路径中的最后一层文件名
char *getSrcFileName(const char *path_src)
{
    if (path_src == NULL)
    {
        printf("getSrcFileName: path_src = NULL\n");
        return NULL;
    }

    // 获取最后一个 '/' 的位置
    int len = strlen(path_src);
    int last_index = -1;
    for (int i = 0; i < len; ++i)
    {
        if (path_src[i] == '/')
        {
            last_index = i;
        }
    }

    return (char *)(path_src + last_index + 1);
}

// 创建多级文件夹
int mkdirs(const char *path)
{
    if (path == NULL)
    {
        printf("mkdirs: path = NULL\n");
        return -1;
    }

    char temp[MAX_DIR_NAME];
    strncpy(temp, path, sizeof(temp));
    int len = strlen(temp);

    for (int i = 0; i < len; i++)
    {
        if (temp[i] == '/')
        {
            temp[i] = '\0'; // 暂时截断路径
            if (access(temp, F_OK) != 0)
            {                                            // 检查目录是否存在
                if (-1 == mkdir(temp, DEFAULT_DIR_ROLE)) // 创建目录
                {
                    printf("mkdirs: %s", temp);
                    return -1;
                }
            }
            temp[i] = '/'; // 恢复路径分隔符
        }
    }

    // 检查并创建最后一级目录
    if (access(temp, F_OK) != 0)
    {
        if (-1 == mkdir(temp, DEFAULT_DIR_ROLE))
        {
            printf("mkdirs/access/mkdir: %s", temp);
            return -1;
        }
    }
    return 0;
}

// 创建多级文件夹下的文件
int mkdirsEndWithFile(const char *src_path, const char *dest_path)
{
    if (dest_path == NULL)
    {
        printf("mkdirs: path = NULL\n");
        return -1;
    }

    char temp[MAX_DIR_NAME] = {'\0'};
    strcat(temp, dest_path);
    int len = strlen(temp);

    for (int i = 0; i < len; i++)
    {
        if (temp[0] == '/' && i == 0) // 防止根路径被截断导致字符串被读为空
        {
            continue;
        }

        if (temp[i] == '/')
        {
            temp[i] = '\0'; // 暂时截断路径
            if (access(temp, F_OK) != 0)
            {                                            // 检查目录是否存在
                if (-1 == mkdir(temp, DEFAULT_DIR_ROLE)) // 创建目录
                {
                    printf("mkdirsEndWithFile/mkdir: error\n");
                    return -1;
                }
            }
            temp[i] = '/'; // 恢复路径分隔符
        }
    }

    int ret = -1;
    // 同步文件
    ret = file_copy(src_path, dest_path);
    if (-1 == my_err(ERR_INT, &ret, "mkdirsEndWithFile/file_copy"))
    {
        return -1;
    }

    // // 创建文件
    // FILE *fp = fopen(src_path, "w");
    // ret = my_err(ERR_PTR, fp, "mkdirsEndWithFile/fopen");
    // if (-1 == ret)
    // {
    //     return -1;
    // }
    // fclose(fp);

    return 0;
}

// 创建多级文件夹下的文件(创建的是空文件)
int mkdirsEndWithFile_Empty(const char *path)
{
    if (path == NULL)
    {
        printf("mkdirs: path = NULL\n");
        return -1;
    }

    char temp[MAX_DIR_NAME] = {'\0'};
    strcat(temp, path);
    int len = strlen(temp);

    for (int i = 0; i < len; i++)
    {
        if (temp[0] == '/' && i == 0) // 防止根路径被截断导致字符串被读为空
        {
            continue;
        }

        if (temp[i] == '/')
        {
            temp[i] = '\0'; // 暂时截断路径
            if (access(temp, F_OK) != 0)
            {                                            // 检查目录是否存在
                if (-1 == mkdir(temp, DEFAULT_DIR_ROLE)) // 创建目录
                {
                    printf("mkdirsEndWithFile/mkdir: error\n");
                    return -1;
                }
            }
            temp[i] = '/'; // 恢复路径分隔符
        }
    }

    int ret = -1;

    // 创建文件
    FILE *fp = fopen(path, "w");
    ret = my_err(ERR_PTR, fp, "mkdirsEndWithFile/fopen");
    if (-1 == ret)
    {
        return -1;
    }
    fclose(fp);

    return 0;
}

// 从当前文件位置指针处截断文件
int truncateFileFromCurrPos(FILE *fp)
{
    if (fp == NULL)
    {
        perror("truncateFileFromCurrPos: fp = NULL\n");
        return -1;
    }

    // 获取当前文件指针位置（截断点）
    long truncate_pos = ftell(fp);
    if (truncate_pos == -1)
    {
        perror("truncateFileFromCurrPos/ftell: truncate_pos = -1\n");
        return -1;
    }

    // 获取文件描述符
    int fd = fileno(fp);
    if (fd == -1)
    {
        perror("truncateFileFromCurrPos/fileno: fd = -1\n");
        return -1;
    }

    // 调用 ftruncate 截断文件
    if (ftruncate(fd, (off_t)truncate_pos) == -1)
    {
        perror("truncateFileFromCurrPos/ftruncate: return = -1\n");
        return -1;
    }

    return 0;
}

/*################################################JSON相关#################################################*/

// 从文件中解析并提取JSON字段
int parseJsonValue(const char *json_path, const char *object_name, const char *key_name,
                   char *buffer, size_t buffer_size)
{
    // 检查输入参数
    if (!json_path || !object_name || !key_name || !buffer || buffer_size == 0)
    {
        printf("parseJsonValue: 参数错误\n");
        return -1; // 参数错误
    }

    // 打开JSON文件
    FILE *file = fopen(json_path, "r");
    my_err(ERR_PTR, file, "parseJsonValue/fopen");

    // 获取文件大小
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // 分配内存并读取文件内容
    char *json_data = (char *)malloc(size + 1);
    if (!json_data)
    {
        fclose(file);
        printf("parseJsonValue/malloc: 内存分配失败\n");
        return -1; // 内存分配失败
    }

    size_t bytes_read = fread(json_data, 1, size, file);
    fclose(file);
    if (bytes_read != (size_t)size)
    {
        free(json_data);
        printf("parseJsonValue/fread: 读取文件失败\n");
        return -1; // 读取文件失败
    }
    json_data[size] = '\0'; // 确保字符串以'\0'结尾

    // 解析JSON数据
    cJSON *root = cJSON_Parse(json_data);
    free(json_data); // 解析完成后释放内存

    if (!root)
    {
        printf("parseJsonValue/cJSON_Parse: JSON解析失败\n");
        return -1; // JSON解析失败
    }

    // 查找指定对象
    cJSON *object = cJSON_GetObjectItemCaseSensitive(root, object_name);
    if (!object || !cJSON_IsObject(object))
    {
        cJSON_Delete(root);
        printf("parseJsonValue/cJSON_GetObjectItemCaseSensitive: 对象不存在\n");
        return 1; // 对象不存在
    }

    // 查找指定键
    cJSON *item = cJSON_GetObjectItemCaseSensitive(object, key_name);
    if (!item)
    {
        cJSON_Delete(root);
        printf("parseJsonValue/cJSON_GetObjectItemCaseSensitive: 键不存在\n");
        return 1; // 键不存在
    }

    // 根据值的类型提取数据
    int result = -1;
    if (cJSON_IsString(item) && item->valuestring)
    {
        // 字符串类型
        strncpy(buffer, item->valuestring, buffer_size - 1);
        buffer[buffer_size - 1] = '\0';
        result = 0; // 成功
    }
    else if (cJSON_IsNumber(item))
    {
        // 数值类型 - 转换为字符串
        snprintf(buffer, buffer_size, "%g", item->valuedouble);
        result = 0; // 成功
    }
    else if (cJSON_IsBool(item))
    {
        // 布尔类型 - 转换为字符串
        strncpy(buffer, cJSON_IsTrue(item) ? "true" : "false", buffer_size);
        buffer[buffer_size - 1] = '\0';
        result = 0; // 成功
    }
    else if (cJSON_IsNull(item))
    {
        // NULL类型
        strncpy(buffer, "null", buffer_size);
        buffer[buffer_size - 1] = '\0';
        result = 0; // 成功
    }

    cJSON_Delete(root);
    return result;
}
