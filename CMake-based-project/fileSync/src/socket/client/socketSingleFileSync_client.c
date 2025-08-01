/*===============================================
*   文件名称：socketSingleFileSync_client.c
*   创 建 者：hqyj
*   创建日期：2025年07月26日
*   描    述：跨主机单个文件同步客户端(同步端)
================================================*/
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../../../include/utils.h"
#include "../../../include/data_structure.h"
#include "../../../include/socketSingleFileSync_client.h"

// 初始化客户端套接字
int client_socket_init(const char *server_ip, const char *server_port)
{
    int client_fd;
    struct sockaddr_in server_addr;

    // 创建套接字
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1)
    {
        perror("socket creation failed");
        return -1;
    }

    // 配置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(server_port));
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0)
    {
        perror("invalid address");
        return -1;
    }

    // 连接服务器
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("connect failed");
        return -1;
    }

    printf("Connected to server %s:%s\n", server_ip, server_port);

    return client_fd;
}

// 发送源文件路径
int send_sourceFile_path(int client_fd, const char *path)
{
    int ret = -1;
    void *data = NULL;
    MessageType msg_type = -1;
    ResponseData *recv_resp = NULL;

    // 发送源文件路径
    send_message(client_fd, MSG_SOURCE_PATH, (void *)path);
    // 接收响应
    msg_type = receive_message(client_fd, &data);
    recv_resp = (ResponseData *)data;
    ret = my_err(ERR_INT, &msg_type, "main/receive_message");
    ret = my_err(ERR_PTR, recv_resp, "main/receive_message");
    if (ret == -1)
    {
        free(data);
        return -1;
    }
    printf("服务器:status: %d, message: %s\n", recv_resp->status, recv_resp->message);

    if (msg_type == MSG_RESPONSE && recv_resp->status == RESPONSE_STATUS_SUCCESS)
    {
        free(data);
        return 0;
    }
    free(data);
    return -1;
}

// 创建同步文件
FILE *create_sync_file(const char *path, const char *mode)
{
    FILE *fp_dest = fopen(path, mode);
    my_err(ERR_PTR, fp_dest, "create_sync_file/fopen");
    return fp_dest;
}

// 同步源文件内容
int recv_content_from_server(int client_fd, FILE *fp_dest)
{
    // 检查参数有效性
    if (client_fd < 0 || fp_dest == NULL)
    {
        printf("参数错误\n");
        errno = EINVAL;
        return -1;
    }

    // // 清空文件内容
    // if (ftruncate(fileno(fp_dest), 0) != 0)
    // {
    //     perror("清空文件失败\n");
    //     return -1;
    // }

    // 重置文件位置指针到文件头
    rewind(fp_dest);

    int ret = -1;
    int max_index = -1;
    int dest_max_index = -1;
    void *data = NULL;
    MessageType msg_type = -1;
    ResponseData send_resp = {0};
    SyncBlockData *recv_syncBlockData = NULL;
    char recv_buf[BUFFER_SIZE] = {'\0'};

    // 接收数据块数量
    msg_type = receive_message(client_fd, &data);
    if (msg_type == MSG_CLIENT_LIST)
    {
        BlockListData *list = (BlockListData *)data;
        if (-1 == my_err(ERR_PTR, list, "recv_content_fron_server/receive_message: null ptr"))
        {
            // 发送失败响应
            send_resp.status = RESPONSE_STATUS_FAILURE;
            send_resp.message = "客户端:data被解析为空指针";
            send_message(client_fd, MSG_RESPONSE, &send_resp);

            free(data);
            return -1;
        }
        // 接收最大块数量-1(索引)
        max_index = list->top;

        // 发送成功响应
        send_resp.status = RESPONSE_STATUS_SUCCESS;
        send_resp.message = "客户端:成功接收到最大块数量";
        send_message(client_fd, MSG_RESPONSE, &send_resp);
        free(data);
    }
    else
    {
        // 消息类型错误
        send_resp.status = RESPONSE_STATUS_FAILURE;
        send_resp.message = "客户端:消息类型错误";
        send_message(client_fd, MSG_RESPONSE, &send_resp);
        free(data);
        return -1;
    }
    // printf("max index = %d\n", max_index);

    // 计算目标文件块数(索引)
    fseek(fp_dest, 0, SEEK_END);
    int dest_Bytes = ftell(fp_dest);
    rewind(fp_dest);
    if (dest_Bytes % BUFFER_SIZE == 0)
    {
        if (dest_Bytes == 0)
        {
            dest_max_index = 0;
        }
        else
        {
            dest_max_index = dest_Bytes / BUFFER_SIZE - 1; // 索引
        }
    }
    else
    {
        dest_max_index = dest_Bytes / BUFFER_SIZE; // 索引
    }

    // // 调试
    // printf("源文件块数量:%d\n", max_index);
    // printf("目标文件快数:%d\n", dest_max_index);

    // 如果dest_max_index > max_index则将目标文件中多出的部分截断
    if (max_index <= dest_max_index)
    {
        ftruncate(fileno(fp_dest), max_index * BUFFER_SIZE);
        rewind(fp_dest);
    }

    // 接收消息
    for (int i = 0; i <= max_index; ++i)
    {
        msg_type = receive_message(client_fd, &data);
        if (msg_type == MSG_SYNC_DATA)
        {
            recv_syncBlockData = (SyncBlockData *)data;
            if (-1 == my_err(ERR_PTR, recv_syncBlockData, "recv_content_fron_server/receive_message: null ptr"))
            {
                free(data);
                return -1;
            }

            // 同步校验
            if (recv_syncBlockData->index != i)
            {
                printf("接收与发送块文件不同步:发送块编号:%d,接收块编号:%d\n", recv_syncBlockData->index, i);
                free(data);
                return -1;
            }

            // 将源块数据读入缓冲区
            memset(recv_buf, '\0', BUFFER_SIZE);
            strncpy(recv_buf, recv_syncBlockData->content, BUFFER_SIZE);
            int hax = bklist_calcBlockItemHax(recv_buf, BUFFER_SIZE, "recv_content_fron_server");

            // // 调试
            // printf("0x%08X ", hax);

            if (hax != recv_syncBlockData->hax) // 校验错误
            {
                printf("校验错误:第%d块数据,源端hax:%d,接收端hax:%d\n", i, recv_syncBlockData->hax, hax);
                free(data);
                return -1;
            }

            // // 调试
            // printf("recv_buff = %s", recv_buf);

            // 将块数据写入文件
            fseek(fp_dest, i * BUFFER_SIZE, SEEK_SET);
            if (i == max_index)
            {
                ret = fwrite(recv_buf, 1, strlen(recv_buf), fp_dest);
            }
            else
            {
                ret = fwrite(recv_buf, 1, BUFFER_SIZE, fp_dest);
            }
            // printf("fwrite ret = %d\n",ret);
            fflush(fp_dest);
            if (-1 == my_err(ERR_INT, &ret, "recv_content_fron_server/fwrite"))
            {
                free(data);
                return -1;
            }

            // // 调试
            // memset(recv_buf, '\0', BUFFER_SIZE);
            // fseek(fp_dest, i * BUFFER_SIZE, SEEK_SET);
            // fread(recv_buf, 1, BUFFER_SIZE, fp_dest);
            // printf("%s", recv_buf);
            // printf("\n---------------------------------------------------------------------------\n");

            free(data); // 写入成功,释放堆内存
        }
        else
        {
            printf("消息类型错误,需要:%d,实际为:%d\n", MSG_SYNC_DATA, msg_type);
            free(data);
            return -1;
        }
    }

    return 0;
}
