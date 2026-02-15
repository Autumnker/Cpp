/*===============================================
*   文件名称：socketDirSync_server.c
*   创 建 者：hqyj
*   创建日期：2025年07月26日
*   描    述：跨主机文件夹同步客户端(源端)
================================================*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../../include/utils.h"
#include "../../../include/hostDirSync.h"
#include "../../../include/data_structure.h"
#include "../../../include/socketDirSync_server.h"

// 接收文件路径并创建/发送文件路径数组
int send_filePath_array(int client_fd, char *src_dir, char (*path_arr)[MAX_DIR_NAME], int *file_count)
{
    int msg_type = -1;
    int ret = -1;
    void *data = NULL;
    char *dir_path = NULL;
    ResponseData *recv_resp = NULL;
    ResponseData send_resp = {0};

    // 接收路径
    msg_type = receive_message(client_fd, &data);
    dir_path = (char *)data;
    ret = my_err(ERR_INT, &msg_type, "open_file_by_client/receive_message");
    ret = my_err(ERR_PTR, dir_path, "open_file_by_client/receive_message");
    if (ret == -1)
    {
        // 发送路径错误响应
        send_resp.status = RESPONSE_STATUS_FAILURE;
        send_resp.message = "服务器:请求解析失败";
        send_message(client_fd, MSG_RESPONSE, &send_resp);
        free(data);
        return -1;
    }

    // // 调试
    // printf("dir_path = %s\n", dir_path);
    strcpy(src_dir, dir_path);

    // 初始化文件路径数组
    ret = getFilePath(dir_path, path_arr, file_count);
    if (-1 == my_err(ERR_INT, &ret, "create_filePath_array/getFilePath"))
    {
        // 发送文件路径数组创建失败响应
        send_resp.status = RESPONSE_STATUS_FAILURE;
        send_resp.message = "服务器:文件路径数组创建失败";
        send_message(client_fd, MSG_RESPONSE, &send_resp);
        free(data);
        return -1;
    }

    // 发送给客户端
    // 发送文件数量(文件路径数组大小)
    BlockListData bklist = {0};
    bklist.top = *file_count;
    send_message(client_fd, MSG_CLIENT_LIST, &bklist);

    // 等待客户端响应
    msg_type = receive_message(client_fd, &data);
    if (msg_type == MSG_RESPONSE)
    {
        recv_resp = (ResponseData *)data;
        if (-1 == my_err(ERR_PTR, recv_resp, "create_filePath_array/receive_message: null ptr"))
        {
            free(data);
            return -1;
        }
        if (recv_resp->status == RESPONSE_STATUS_FAILURE)
        {
            printf("%s\n", recv_resp->message);
            free(data);
            return -1;
        }

        printf("%s\n", recv_resp->message);
        free(data);
    }
    else
    {
        printf("消息类型错误,需要:%d,实际上:%d\n", MSG_RESPONSE, msg_type);
        free(data);
        return -1;
    }

    // 循环发送文件路径数组
    for (int i = 0; i < *file_count; ++i)
    {
        send_message(client_fd, MSG_REQUEST, path_arr[i]);
    }

    // 接收响应
    msg_type = receive_message(client_fd, &data);
    if (msg_type == MSG_RESPONSE)
    {
        recv_resp = (ResponseData *)data;
        if (-1 == my_err(ERR_PTR, recv_resp, "create_filePath_array/receive_message: null ptr"))
        {
            free(data);
            return -1;
        }
        if (recv_resp->status == RESPONSE_STATUS_FAILURE)
        {
            printf("%s\n", recv_resp->message);
            free(data);
            return -1;
        }

        printf("%s\n", recv_resp->message);
        free(data);
    }
    else
    {
        printf("消息类型错误,需要:%d,实际上:%d\n", MSG_RESPONSE, msg_type);
        free(data);
        return -1;
    }

    // 返回状态
    return 0;
}

// 单个进程处理文件同步请求的逻辑