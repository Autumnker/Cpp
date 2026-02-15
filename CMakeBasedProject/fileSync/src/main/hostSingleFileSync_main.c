/*===============================================
*   文件名称：hostSync.c
*   创 建 者：hqyj
*   创建日期：2025年07月24日
*   描    述：本机文件同步
1.  接收参数<源文件路径><目标文件路径>

2.  参数校验(判断源文件是否存在)[以只读方式打开源文件,
    以追加+创建+可读方式打开目标文件]

3.  分别读取源头/目标文件以初始化源文件、目标文件的块
    信息描述线性表

4.  当检测到源文件的"in_close_write"信号时，触发同步
    机制

5.  如果块信息描述线性表被填满，则"loop_count" + 1并且
    从下一块信息开始从头初始化线性表
================================================*/
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/inotify.h>

#include "../../include/utils.h"
#include "../../include/data_structure.h"
#include "../../include/hostSingleFileSync.h"

#define DEBUG 1

int main(int argc, char *argv[])
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
    strcat(buf1, "/debug/src.txt");
    argv[1] = buf1;

    strcat(buf2, "/debug/dest.txt");
    argv[2] = buf2;

    // printf("argv1 = %s\n", argv[1]);
    // printf("argv2 = %s\n", argv[2]);

#else
    // 参数校验
    if (argc < 3)
    {
        printf("%s <源文件路径> <同步文件路径>\n", argv[0]);
        return -1;
    }
#endif

    // 获得源文件、目标文件的文件流指针
    FILE *fp_src = fopen(argv[1], "r");
    my_err(ERR_PTR, fp_src, "fopen");
    FILE *fp_dest = fopen(argv[2], "r+");
    my_err(ERR_PTR, fp_dest, "fopen");

    // 初始化文件块描述线性表
    Block_Info_List *blockList_src = bklist_init();
    Block_Info_List *blockList_dest = bklist_init();

    // 获取文件块大小并创建缓冲区
    char buff_src[BUFFER_SIZE] = {'\0'};
    char buff_dest[BUFFER_SIZE] = {'\0'};

    // 初始化同步结构体
    Sync_st sync_src = {0};
    sync_src.bklist_ptr = blockList_src;
    sync_src.fp = fp_src;
    sync_src.buffer = buff_src;

    Sync_st sync_dest = {0};
    sync_dest.bklist_ptr = blockList_dest;
    sync_dest.fp = fp_dest;
    sync_dest.buffer = buff_dest;

    // 开始同步
    printf("以下文件已开启同步:\n%s\n", argv[2]);
    blockingSync(argv[1], &sync_src, &sync_dest, 0);
    // syncFile(&sync_src, &sync_dest);

    // 释放资源
    bklist_delete(blockList_src);
    bklist_delete(blockList_dest);
    fclose(fp_src);
    fclose(fp_dest);

    printf("程序成功退出!\n");

    return 0;
}
