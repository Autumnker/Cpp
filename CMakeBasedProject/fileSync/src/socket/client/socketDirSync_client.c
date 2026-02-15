/*===============================================
*   文件名称：socketDirSync_client.c
*   创 建 者：hqyj
*   创建日期：2025年07月26日
*   描    述：跨主机文件夹同步客户端(同步端)
================================================*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../../include/utils.h"
#include "../../../include/hostDirSync.h"
#include "../../../include/data_structure.h"
#include "../../../include/socketDirSync_client.h"

// 发送源文件夹路径
int recv_filePath_array(int client_fd, const char *src_dir, char (*src_path_arr)[MAX_DIR_NAME], int *file_count)
{
    int msg_type = -1;
    int ret = -1;
    void *data = NULL;
    char *dir_path = NULL;
    ResponseData *recv_resp = NULL;
    ResponseData send_resp = {0};
    BlockListData *recv_bklist = NULL;
    char *request = NULL;

    // 发送路径
    send_message(client_fd, MSG_REQUEST, (void *)src_dir);

    // 接收路径数组大小(文件数量)
    msg_type = receive_message(client_fd, &data);
    if (msg_type == MSG_CLIENT_LIST)
    {
        recv_bklist = (BlockListData *)data;
        if (-1 == my_err(ERR_PTR, recv_bklist, "recv_filePath_array/receive_message: null ptr"))
        {
            // 参数解析失败
            send_resp.status = RESPONSE_STATUS_FAILURE;
            send_resp.message = "客户端:解析文件数组大小信息失败";
            send_message(client_fd, MSG_RESPONSE, &send_resp);

            free(data);
            return -1;
        }
        *file_count = recv_bklist->top;
        free(data);

        // 发送成功接收响应
        send_resp.status = RESPONSE_STATUS_SUCCESS;
        send_resp.message = "客户端:成功收到文件数组大小";
        send_message(client_fd, MSG_RESPONSE, &send_resp);
    }
    else
    {
        // 消息类型错误
        send_resp.status = RESPONSE_STATUS_FAILURE;
        send_resp.message = "客户端:错误的消息类型";
        send_message(client_fd, MSG_RESPONSE, &send_resp);

        printf("消息类型错误,需要:%d,实际上:%d\n", MSG_CLIENT_LIST, msg_type);
        free(data);
        return -1;
    }

    // 接收文件路径数组
    for (int i = 0; i < *file_count; ++i)
    {
        msg_type = receive_message(client_fd, &data);
        if (msg_type == MSG_REQUEST)
        {
            request = (char *)data;
            if (-1 == my_err(ERR_PTR, request, "recv_filePath_array/receive_message"))
            {
                // 参数解析失败
                send_resp.status = RESPONSE_STATUS_FAILURE;
                send_resp.message = "客户端:解析文件路径信息失败";
                send_message(client_fd, MSG_RESPONSE, &send_resp);

                free(data);
                return -1;
            }

            // 给文件路径数组赋值
            strcpy(src_path_arr[i], request);
            free(data);
        }
        else
        {
            // 消息类型错误
            send_resp.status = RESPONSE_STATUS_FAILURE;
            send_resp.message = "客户端:错误的消息类型";
            send_message(client_fd, MSG_RESPONSE, &send_resp);

            printf("消息类型错误,需要:%d,实际上:%d\n", MSG_REQUEST, msg_type);
            free(data);
            return -1;
        }
    }

    // 发送接收成功响应
    send_resp.status = RESPONSE_STATUS_SUCCESS;
    send_resp.message = "客户端:成功接收到文件路径数组";
    send_message(client_fd, MSG_RESPONSE, &send_resp);

    // 返回状态
    return 0;
}

// 创建同步文件夹/文件
int createSyncDirs(char (*src_path_arr)[MAX_DIR_NAME], char (*dest_path_arr)[MAX_DIR_NAME],
                   const char *src_dir, const char *dest_dir, int file_count)
{
    int ret = -1;

    // 映射源文件路径到目标文件夹中
    ret = srcPathToDestPath(src_path_arr, dest_path_arr, src_dir, dest_dir, file_count);
    if (-1 == my_err(ERR_INT, &ret, "createSyncDirs/srcPathToDestPath"))
    {
        return -1;
    }

    // 创建目标文件夹/文件
    for (int i = 0; i < file_count; ++i)
    {
        // printf("调试--->%s\n", dest_path_arr[i]);
        ret = mkdirsEndWithFile_Empty(dest_path_arr[i]);
        if (-1 == my_err(ERR_INT, &ret, "createSyncDirs/mkdirsEndWithFile_Empty"))
        {
            return -1;
        }
    }

    return 0;
}

// 进程处理单个文件同步的逻辑