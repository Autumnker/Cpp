/*===============================================
*   文件名称：hostDieSync.c
*   创 建 者：hqyj
*   创建日期：2025年07月25日
*   描    述：本机文件夹同步
1.  参数校验

2.  创建文件路径数组

3.  用迭代方式收集源文件夹路径数据并初始化文件路径数组

4.  根据文件夹路径数组在目标文件夹下创建文件夹与文件

5.  根据目标文件夹(前缀)/文件夹路径数组(后缀)的方式
    为每个文件开辟同步进程
================================================*/

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "../../include/utils.h"
#include "../../include/hostDirSync.h"
#include "../../include/data_structure.h"
#include "../../include/hostSingleFileSync.h"

#define DEBUG 1

int main(int argc, char const *argv[])
{
#if DEBUG

    char buf1[BUFFER_SIZE] = {'\0'};
    char buf2[BUFFER_SIZE] = {'\0'};
    char *root_path = get_executable_dir();
    my_err(ERR_PTR, root_path, "main/get_executable_dir");
    strcat(buf1, root_path);
    strcat(buf2, root_path);

    // 去掉一级目录
    for (int i = strlen(root_path) - 1; i >= 0; --i)
    {
        if (root_path[i] == '/')
        {
            // 只能动全局变量，不能动静态变量
            buf1[i] = '\0';
            buf2[i] = '\0';
            break;
        }
    }
    strcat(buf1, "/debug/srcDir");
    argv[1] = buf1;

    strcat(buf2, "/debug/destDir");
    argv[2] = buf2;

    // printf("argv1 = %s\n", argv[1]);
    // printf("argv2 = %s\n", argv[2]);

#else
    //  参数校验
    if (argc < 3)
    {
        printf("argc < 3: %s <源文件夹> <目标文件夹>\n", argv[0]);
        return -1;
    }
#endif
    // 返回值记录变量
    int ret = -1;

    // 文件路径数组
    char src_path_arr[MAX_FILE_NUM][MAX_DIR_NAME] = {'\0'};
    char dest_path_arr[MAX_FILE_NUM][MAX_DIR_NAME] = {'\0'};
    int file_count = 0;
    // 收集源文件夹路径数据并初始化文件路径数组
    ret = getFilePath(argv[1], src_path_arr, &file_count);
    my_err(ERR_INT, &ret, "getFilePath");

    // 将源文件路径映射到目标文件路径上
    ret = srcPathToDestPath(src_path_arr, dest_path_arr, argv[1], argv[2], file_count);
    my_err(ERR_INT, &ret, "srcPathToDestPath");

    // 在目标根目录下创建所需文件/文件夹
    ret = createDirs(src_path_arr, dest_path_arr, file_count);
    my_err(ERR_INT, &ret, "createDirs");

    // 为每个文件创建同步进程
    pid_t *pid_arr = (pid_t *)malloc(sizeof(pid_t) * file_count);
    my_err(ERR_PTR, pid_arr, "malloc");

    printf("以下文件已开启同步……\n");
    for (int i = 0; i < file_count; ++i)
    {
        // 创建线程
        pid_t pid = fork();
        my_err(ERR_INT, &pid, "fork");
        if (-1 == ret)
        {
            return -1;
        }

        // 子进程
        else if (pid == 0)
        {
            // 构建目标文件路径
            char dest_path[MAX_DIR_NAME] = {'\0'};
            strcat(dest_path, argv[2]);
            strcat(dest_path, "/");
            char *tail_path = NULL;
            tail_path = path_subtract(src_path_arr[i], argv[1]);
            my_err(ERR_PTR, tail_path, "path_subtract");
            strcat(dest_path, tail_path);

            printf("%s\n", dest_path);

            ret = runSingleFileSync(src_path_arr[i], dest_path);
            my_err(ERR_INT, &ret, "runSingleFileSync");

            exit(EXIT_SUCCESS);
        }

        // 父进程
        pid_arr[i] = pid;
    }

    int wstat;
    pid_t qpid = wait(&wstat);

    free(pid_arr);

    printf("文件同步结束\n");

    return 0;
}
