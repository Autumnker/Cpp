/*===============================================
*   文件名称：hostSingleFileSync.c
*   创 建 者：hqyj
*   创建日期：2025年07月25日
*   描    述：本地单个文件同步相关函数
================================================*/

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include "../../include/hostSingleFileSync.h"
#include "../../include/utils.h"

int blockingSync(const char *path_src, Sync_st *sync_src, Sync_st *sync_dest, int quit_signal)
{
    // 创建监听描述符
    int fd = inotify_init();
    int wd = inotify_add_watch(fd, path_src, IN_CLOSE_WRITE | IN_MOVED_TO);

    int len = 0;
    int ret = 0;
    char buf[BUFFER_SIZE] = {'\0'};
    while (quit_signal == 0)
    {
        memset(buf, '\0', BUFFER_SIZE);
        len = read(fd, buf, BUFFER_SIZE);
        my_err(ERR_INT, &len, "blockingSync/read");
        for (char *ptr = buf; ptr < buf + len;)
        {
            struct inotify_event *event = (struct inotify_event *)ptr;
            if (event->mask & IN_CLOSE_WRITE || event->mask & IN_MOVED_TO)
            {
                ret = syncFile(sync_src, sync_dest);
                if (-1 == my_err(ERR_INT, &ret, "blockingSync/syncFile"))
                {
                    printf("blockingSync/syncFile\n");
                    return -1;
                }
            }

            ptr += sizeof(struct inotify_event) + event->len;
        }
    }

    return 0;
}

#if 0

// 旧思路(有BUG)
int syncFile(Sync_st *sync_src, Sync_st *sync_dest)
{
    // 解析参数
    Block_Info_List *bklist_src = sync_src->bklist_ptr;
    Block_Info_List *bklist_dest = sync_dest->bklist_ptr;
    FILE *fp_src = sync_src->fp;
    FILE *fp_dest = sync_dest->fp;
    char *buffer_src = sync_src->buffer;
    char *buffer_dest = sync_dest->buffer;

    // 文件位置指针归位
    fseek(fp_src, 0, SEEK_SET);
    fseek(fp_dest, 0, SEEK_SET);

    // 返回值接收变量
    int ret_src = -1, ret_dest = -1;
    int ret = -1;
    int differ_index = -1;
    char *tempBuffer = (char *)malloc(sizeof(char) * BUFFER_SIZE);

    // 初始化源文件线性表
    ret_src = bklist_updateByFile(bklist_src, fp_src, buffer_src, BUFFER_SIZE);
    my_err(ERR_INT, &ret_src, "bklist_updateByFile_src");

    printf("开始同步……");
    while (differ_index != -2)
    {
        // 重置参数
        ret_src = -1, ret_dest = -1;
        ret = -1;
        differ_index = -1;
        memset(tempBuffer, '\0', BUFFER_SIZE);

        // 初始化线性表
        while (ret_dest)
        {
            // 用文件流更新线性表
            fseek(fp_dest, 0, SEEK_SET); // 将文件位置指针移动到文件头部
            ret_dest = bklist_updateByFile(bklist_dest, fp_dest, buffer_dest, BUFFER_SIZE);
            my_err(ERR_INT, &ret_dest, "bklist_updateByFile_dest");
        }

        // printf("\n-------------------------------------------------------------------\n");
        // printf("打印线性表src\n");
        // bklist_print(bklist_src);
        // printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
        // printf("打印线性表dest\n");
        // bklist_print(bklist_dest);
        // printf("\n-------------------------------------------------------------------\n");

        // 找到第一个不同
        differ_index = bklist_getDifferIndex(bklist_src, bklist_dest);
        if (differ_index >= 0)
        {
            // 移动源文件位置指针并读取其中内容
            fseek(fp_src, differ_index * BUFFER_SIZE, SEEK_SET);
            ret = fread(tempBuffer, sizeof(char), BUFFER_SIZE, fp_src);
            my_err(ERR_INT, &ret, "fread");

            // 覆盖同步文件中相同位置的数据
            fseek(fp_dest, differ_index * BUFFER_SIZE, SEEK_SET);
            ret = fwrite(tempBuffer, sizeof(char), ret, fp_dest);
            my_err(ERR_INT, &ret, "fwrite");
        }
        else if (differ_index == -2)
        {
            printf("        同步完成!\n");
        }
        else if (differ_index == -1)
        {
            printf("发生错误:syncFile->differ_index = -1/\n");
            break;
        }
        else if (differ_index == -3)
        {
            fflush(fp_dest);               // 确保缓冲区写入
            ftruncate(fileno(fp_dest), 0); // 截断文件为 0 字节
            rewind(fp_dest);               // 重置文件指针到开头
        }
    }

    // 释放资源
    free(tempBuffer);
}

#else
// 新思路(按块替换)
int syncFile(Sync_st *sync_src, Sync_st *sync_dest)
{
    // 解析参数
    Block_Info_List *bklist_src = sync_src->bklist_ptr;
    Block_Info_List *bklist_dest = sync_dest->bklist_ptr;
    FILE *fp_src = sync_src->fp;
    FILE *fp_dest = sync_dest->fp;
    char *buffer_src = sync_src->buffer;
    char *buffer_dest = sync_dest->buffer;

    // 文件位置指针归位
    fseek(fp_src, 0, SEEK_SET);
    fseek(fp_dest, 0, SEEK_SET);

    // 返回值接收变量
    int ret_src = -1, ret_dest = -1;
    int ret = -1;
    int differ_index = -1;
    char tempBuffer[BUFFER_SIZE] = {'\0'};

    // 初始化线性表
    ret_src = bklist_updateByFile(bklist_src, fp_src, buffer_src, BUFFER_SIZE);
    my_err(ERR_INT, &ret_src, "syncFile/bklist_updateByFile_src");

    ret_dest = bklist_updateByFile(bklist_dest, fp_dest, buffer_dest, BUFFER_SIZE);
    my_err(ERR_INT, &ret_dest, "syncFile/bklist_updateByFile_dest");

    // 得到第一个不同值的索引
    differ_index = bklist_getDifferIndex(bklist_src, bklist_dest);
    // printf("differ_index = %d\n", differ_index);

    // 目标文件 > 源文件, 截断目标文件到与源文件大小相同
    if (differ_index == -3)
    {
        // 将目标文件的文件位置指针移动到与源文件末尾相同的位置
        ret = fseek(fp_dest, bklist_src->max_index * BUFFER_SIZE, SEEK_SET);
        my_err(ERR_INT, &ret, "syncFile/fseek");
        ret = truncateFileFromCurrPos(fp_dest);
        my_err(ERR_INT, &ret, "syncFile/truncateFileFromCurrPos");

        // 目标文件位置指针回到文件头位置
        ret = fseek(fp_dest, 0, SEEK_SET);
        my_err(ERR_INT, &ret, "syncFile/fseek");

        // 刷新目标文件线性表
        ret = bklist_updateByFile(bklist_dest, fp_dest, buffer_dest, BUFFER_SIZE);
        my_err(ERR_INT, &ret_dest, "syncFile/bklist_updateByFile_dest");

        // 重新计算第一个不同位置索引
        differ_index = bklist_getDifferIndex(bklist_src, bklist_dest);
    }

    if (differ_index >= 0)
    {
        // 创建循环，使目标文件与源文件同步
        printf("开始同步……");
        for (int i = differ_index; i <= bklist_src->max_index; ++i)
        {
            // 重置参数
            ret_src = -1, ret_dest = -1;
            ret = -1;
            // 从 i 开始比较两个线性表，不同则同步，相同则跳过
            if (bklist_src->list[i] != bklist_dest->list[i])
            {
                // printf("\nsrc:\n");
                // bklist_print(bklist_src);
                // printf("\ndest:\n");
                // bklist_print(bklist_dest);

                // 清空目标文件该块中原本数据
                // 获取目标块中原有数据大小
                memset(tempBuffer, '\0', BUFFER_SIZE);
                ret = fseek(fp_dest, i * BUFFER_SIZE, SEEK_SET);
                my_err(ERR_INT, &ret, "syncFile/fseek");
                ret_dest = fread(tempBuffer, sizeof(char), BUFFER_SIZE, fp_dest);
                my_err(ERR_INT, &ret_dest, "syncFile/fread");
                // 将原有数据替换为'\0'
                memset(tempBuffer, '\0', BUFFER_SIZE);
                ret = fseek(fp_dest, i * BUFFER_SIZE, SEEK_SET);
                my_err(ERR_INT, &ret, "syncFile/fseek");
                ret_dest = fwrite(tempBuffer, sizeof(char), ret_dest, fp_dest);
                my_err(ERR_INT, &ret_dest, "syncFile/fwrite");
                // 刷新缓存
                fflush(fp_dest);
                memset(tempBuffer, '\0', BUFFER_SIZE);

                // 读取源文件块内容
                ret = fseek(fp_src, i * BUFFER_SIZE, SEEK_SET);
                my_err(ERR_INT, &ret, "syncFile/fseek");
                ret_src = fread(tempBuffer, sizeof(char), BUFFER_SIZE, fp_src);
                my_err(ERR_INT, &ret_src, "syncFile/fread");

                // 覆盖同步文件中相同位置的数据
                ret = fseek(fp_dest, i * BUFFER_SIZE, SEEK_SET);
                my_err(ERR_INT, &ret, "syncFile/fseek");
                ret_dest = fwrite(tempBuffer, sizeof(char), ret_src, fp_dest);
                my_err(ERR_INT, &ret_dest, "syncFile/fwrite");
                fflush(fp_dest);

                // 从目标块中读取刚写入的内容并计算其hax值                memset(tempBuffer, '\0', BUFFER_SIZE);
                memset(tempBuffer, '\0', BUFFER_SIZE);
                ret = fseek(fp_dest, i * BUFFER_SIZE, SEEK_SET);
                my_err(ERR_INT, &ret, "syncFile/syncFile/fseek");
                ret_dest = fread(tempBuffer, sizeof(char), BUFFER_SIZE, fp_dest);
                my_err(ERR_INT, &ret_dest, "syncFile/fread");
                int hax = 0; // 如果ret_dest = 0,表示目标块中无内容,其hax固然为0
                if (ret_dest > 0)
                {
                    hax = bklist_calcBlockItemHax(tempBuffer, ret_dest, "syncFile");
                    my_err(ERR_INT, &hax, "syncFile/bklist_calcBlockItemHax");
                }

                if (i > bklist_dest->max_index) // 扩展目标线性表有效数据长度以适配源线性表
                {
                    bklist_dest->max_index = i;
                }

                if (hax != bklist_src->list[i]) // 如果刚写入的数据的hax值与源线性表中hax值不同则再次同步该块
                {
                    printf("第 %d 块文件同步失败\n", i);
                    break;

                    // printf("第 %d 块文件同步失败,是否重试?(yes/no)\n", i);
                    // char buf[128] = {'\0'};
                    // scanf("%s", buf);
                    // if (strncmp(buf, "yes", 3) == 0)
                    // {
                    //     printf("正在重试……\n");
                    //     i--;
                    //     continue;
                    // }
                    // else
                    // {
                    //     printf("退出本次同步\n");
                    //     break;
                    // }
                }
                else // 如果目标块hax与源线性表中hax一致,则修改目标线性表hax值
                {
                    ret = bklist_change(bklist_dest, i, hax);
                    my_err(ERR_INT, &ret, "syncFile/bklist_change");
                }

                // printf("\nindex = %d\n", i + 1);
                // printf("\n-------------------------------------------------------------------\n");
                // printf("打印线性表src\n");
                // bklist_print(bklist_src);
                // printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
                // printf("打印线性表dest\n");
                // bklist_print(bklist_dest);
                // printf("\n-------------------------------------------------------------------\n");
            }
        }
        printf("        同步完成\n");
    }
    else if (differ_index == -1)
    {
        printf("发生错误:syncFile->differ_index = -1/\n");
    }
}

#endif

int runSingleFileSync(const char *path_src, const char *path_dest)
{
    // 返回值接收函数
    int ret = -1;

    // 获得源文件、目标文件的文件流指针
    FILE *fp_src = fopen(path_src, "r");
    my_err(ERR_PTR, fp_src, "runSingleFileSync/fopen: fp_src = NULL");

    FILE *fp_dest = fopen(path_dest, "r+");
    my_err(ERR_PTR, fp_dest, "runSingleFileSync/fopen: fp_dest = NULL");

    // 初始化文件块描述线性表
    Block_Info_List *blockList_src = bklist_init();
    my_err(ERR_PTR, blockList_src, "runSingleFileSync/bklist_init: blockList_src = NULL");

    Block_Info_List *blockList_dest = bklist_init();
    my_err(ERR_PTR, blockList_dest, "runSingleFileSync/bklist_init: blockList_dest = NULL");

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
    ret = blockingSync(path_src, &sync_src, &sync_dest, 0);
    my_err(ERR_INT, &ret, "runSingleFileSync/blockingSync");

    // ret = syncFile(&sync_src, &sync_dest);

    // 释放资源
    bklist_delete(blockList_src);
    bklist_delete(blockList_dest);
    fclose(fp_src);
    fclose(fp_dest);

    return 0;
}
