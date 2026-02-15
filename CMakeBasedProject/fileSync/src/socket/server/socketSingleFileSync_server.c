/*===============================================
*   文件名称：socketSingleFileSync_server.c
*   创 建 者：hqyj
*   创建日期：2025年07月26日
*   描    述：跨主机单个文件同步客户端(源端)
================================================*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/inotify.h>
#include "../../../include/utils.h"
#include "../../../include/data_structure.h"
#include "../../../include/socketSingleFileSync_server.h"

int server_socket_init(const char *server_ip, const char *server_port)
{
    int server_fd = -1;
    struct sockaddr_in server_addr;

    // 创建套接字
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("socket creation failed");
        return -1;
    }

    // 设置地址可重用
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        perror("setsockopt failed");
        close(server_fd);
        return -1;
    }

    // 配置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(server_port));
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0)
    {
        perror("invalid address");
        close(server_fd);
        return -1;
    }

    // 绑定地址和端口
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind failed");
        close(server_fd);
        return -1;
    }

    // 监听端口
    if (listen(server_fd, BACKLOG) == -1)
    {
        perror("listen failed");
        close(server_fd);
        return -1;
    }

    printf("Server listening on %s:%s...\n", server_ip, server_port);

    return server_fd;
}

FILE *open_file_by_client(int client_fd, const char *mode, char (*path)[MAX_DIR_NAME])
{
    int msg_type = -1;
    int ret = -1;
    void *data = NULL;
    ResponseData send_resp = {0};
    FILE *fp_src = NULL;

    // 接收源文件路径
    msg_type = receive_message(client_fd, &data);
    strcpy(*path, (char *)data);
    ret = my_err(ERR_INT, &msg_type, "open_file_by_client/receive_message");
    ret = my_err(ERR_PTR, *path, "open_file_by_client/receive_message");
    if (ret == -1)
    {
        // 发送路径错误响应
        send_resp.status = RESPONSE_STATUS_FAILURE;
        send_resp.message = "服务器:请求解析失败";
        send_message(client_fd, MSG_RESPONSE, &send_resp);
        free(data);
        return NULL;
    }

    // // 调试
    // printf("path = %s\n", *path);

    // 检验源路径是否正确
    fp_src = fopen(*path, mode);
    ret = my_err(ERR_PTR, fp_src, "open_file_by_client/fopen");
    if (ret == -1)
    {
        send_resp.status = RESPONSE_STATUS_FAILURE;
        send_resp.message = "服务器:文件打开失败";
        send_message(client_fd, MSG_RESPONSE, &send_resp);
        free(data);
        return NULL;
    }

    send_resp.status = RESPONSE_STATUS_SUCCESS;
    send_resp.message = "服务器:打开源文件成功";
    send_message(client_fd, MSG_RESPONSE, &send_resp);

    free(data);
    return fp_src;
}

int send_content_to_client(int client_fd, FILE *fp_src)
{
    // 重置文件位置指针到文件头
    rewind(fp_src);

    int ret = -1;
    void *data;
    char send_buff[BUFFER_SIZE] = {'\0'};
    MessageType msg_tyoe = -1;
    SyncBlockData sendSyncBlockData = {0};

    Block_Info_List *bklist_src_temp = bklist_init();
    ret = bklist_updateByFile(bklist_src_temp, fp_src, send_buff, BUFFER_SIZE);
    if (-1 == my_err(ERR_INT, &ret, "send_content_to_client/bklist_updateByFile"))
    {
        return -1;
    }
    int item_num = bklist_src_temp->max_index;

    // // 调试
    // printf("max_index = %d\n", max_index);

    // 发送数据块数量
    BlockListData list = {0};
    if (item_num == 0)
    {
        list.top = 0;
    }
    else
    {
        list.top = item_num - 1;
    }
    send_message(client_fd, MSG_CLIENT_LIST, &list);

    // 接收响应
    msg_tyoe = receive_message(client_fd, &data);
    if (msg_tyoe == MSG_RESPONSE)
    {
        ResponseData *recv_resp = (ResponseData *)data;
        if (-1 == my_err(ERR_PTR, recv_resp, "send_content_to_client/receive_message: null ptr"))
        {
            printf("解析客户端响应失败\n");
            bklist_delete(bklist_src_temp);
            free(data);
            return -1;
        }
        if (recv_resp->status == RESPONSE_STATUS_FAILURE) // 响应错误
        {
            printf("客户端未能成功接收到数据块数量\n");
            printf("客户端: %s\n", recv_resp->message);
            bklist_delete(bklist_src_temp);
            free(data);
            return -1;
        }
        free(data); // 成功接收到数据块数量,释放data指向的堆内存
    }
    else
    {
        printf("响应类型错误\n");
        bklist_delete(bklist_src_temp);
        free(data);
        return -1;
    }

    // 分块发送数据
    for (int i = 0; i <= list.top; ++i)
    {
        memset(send_buff, '\0', BUFFER_SIZE);
        fseek(fp_src, i * BUFFER_SIZE, SEEK_SET);
        ret = fread(send_buff, 1, BUFFER_SIZE, fp_src);
        if (-1 == my_err(ERR_INT, &ret, "send_content_to_client/fread"))
        {
            bklist_delete(bklist_src_temp);
            return -1;
        }

        // 调试
        // printf("%s", send_buff);
        // printf("\n---------------------------------------------------------------------------\n");
        // printf("0x%08X ", sendSyncBlockData.hax);

        strncpy(sendSyncBlockData.content, send_buff, BUFFER_SIZE);
        sendSyncBlockData.index = i;
        sendSyncBlockData.hax = bklist_src_temp->list[i]; // 用于接收端校验
        send_message(client_fd, MSG_SYNC_DATA, &sendSyncBlockData);
    }
    // printf("文件同步完成\n");
    bklist_delete(bklist_src_temp); // 释放堆内存

    // 源文件位置指针回到文件头
    rewind(fp_src);

    return 0;
}

int listenSyncSingle(const char *path)
{
    // 创建监听描述符
    int fd = inotify_init();
    int wd = inotify_add_watch(fd, path, IN_CLOSE_WRITE | IN_MOVED_TO);

    int len = 0;
    char buf[BUFFER_SIZE] = {'\0'};

    memset(buf, '\0', BUFFER_SIZE);
    len = read(fd, buf, BUFFER_SIZE);
    my_err(ERR_INT, &len, "blockingSync/read");
    for (char *ptr = buf; ptr < buf + len;)
    {
        struct inotify_event *event = (struct inotify_event *)ptr;
        if (event->mask & IN_CLOSE_WRITE || event->mask & IN_MOVED_TO)
        {
            return 1;
        }

        ptr += sizeof(struct inotify_event) + event->len;
    }

    return 0;
}