/*===============================================
*   文件名称：socketSingleClient.c
*   创 建 者：hqyj    
*   创建日期：2025年07月28日
*   描    述：有bug但是更节省资源的旧思路
================================================*/
#include <stdio.h>

#if 0 // 旧思路
int main(int argc, char *argv[])
{

    int client_quit = 0;
    int client_fd;
    int ret = -1;
    char buffer[BUFFER_SIZE];
    char recv_buf[BUFFER_SIZE];
    char send_buf[BUFFER_SIZE];
    void *data = NULL;
    MessageType msg_type = -1;
    ResponseData *recv_resp = NULL;
    ResponseData send_resp = {0};
    FILE *fp_dest = NULL;
    char *request = NULL;
    SyncBlockData *recv_syncBlockData = NULL;

    // 调试模式下自动填充参数
#if DEBUG
    argv[1] = DEFAULT_SERVER_IP;
    argv[2] = DEFAULT_SERVER_PORT;

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
    argv[3] = buf1;

    strcat(buf2, "/debug/dest.txt");
    argv[4] = buf2;

#else
    if (argc != 5)
    {
        fprintf(stderr, "Usage: %s <server_ip> <server_port> <src_file_path> <dest_file_path>\n", argv[0]);
        return 1;
    }
#endif

    // 初始化套接字
    client_fd = client_socket_init(argv[1], argv[2]);
    my_err(ERR_INT, &client_fd, "main/client_socket_init");

    /*-----------------------------------------客户端逻辑------------------------------------------*/

    // 1.发送源文件路径
    while (1)
    {
        // 发送源文件路径
        send_message(client_fd, MSG_SOURCE_PATH, argv[3]);
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
            break;
        }
        free(data);
        sleep(1); // 避免陷入死循环
    }
    printf("连接成功,将同步服务器中的以下文件: %s\n", argv[3]);

    // 2.在自己的文件夹下创建同步文件
    while (fp_dest == NULL)
    {
        fp_dest = fopen(argv[4], "a+");
        if (fp_dest == NULL)
        {
            printf("同步文件打开失败,请输入同步文件路径:");
            scanf("%s", argv[4]);
        }
    }
    printf("同步文件打开成功,路径为:%s\n", argv[4]);

    // 同步源文件中的内容
    while (1)
    {
        // 接收消息
        msg_type = receive_message(client_fd, &data);
        if (msg_type == MSG_REQUEST)
        {
            request = (char *)data;
            // 接收到了"获取客户端"目标文件线性表""请求
            if (strcmp(request, REQUEST_STR_GET_CLIENT_LIST) == 0)
            {
                break; // 跳转到下一个循环
            }
        }

        recv_syncBlockData = (SyncBlockData *)data;
        ret = my_err(ERR_INT, &msg_type, "main/receive_message");
        ret = my_err(ERR_PTR, recv_syncBlockData, "main/receive_message");
        if (ret == -1)
        {
            free(data);
            return -1;
        }

        // 同步数据
        memset(recv_buf, '\0', BUFFER_SIZE);
        strcpy(recv_buf, recv_syncBlockData->content);

        // // 调试
        // printf("%s", recv_buf);
        // printf("\n---------------------------------------------------------------------------\n");

        // 判断退出条件
        if (recv_syncBlockData->index > recv_syncBlockData->hax)
        {
            free(data);
            break;
        }

        ret = fwrite(recv_buf, 1, BUFFER_SIZE, fp_dest);
        free(data);
    }
    printf("文件同步完成\n");

    // 进入同步循环
    while (client_quit == 0)
    {
        // 3.向服务器发送"接收目标文件线性表请求"
        if (msg_type == MSG_REQUEST && strcmp(request, REQUEST_STR_GET_CLIENT_LIST) == 0)
        {
            // 调试
            printf("收到\"接收文件线性表请求\"\n");

            free(data);
            // 计算bklis
            Block_Info_List *bklist = bklist_init();
            ret = bklist_updateByFile(bklist, fp_dest, buffer, BUFFER_SIZE);
            my_err(ERR_INT, &ret, "main/bklist_updateByFile");
            if (ret == -1)
            {
                break;
            }

            // 将bklist从堆区复制到栈区
            BlockListData bklist_data = {0};
            bklist_data.top = bklist->max_index;
            for (int i = 0; i <= bklist_data.top; ++i)
            {
                bklist_data.items[i] = bklist->list[i];
            }
            bklist_delete(bklist); // 释放堆区

            // 发送线性表
            send_message(client_fd, MSG_CLIENT_LIST, &bklist_data);
            // printf("发送线性表\n");
        }
        else
        {
            printf("请求不正确,应该为%s,实际为:%s\n", REQUEST_STR_GET_CLIENT_LIST, request);
            free(data);
            break;
        }

        // 接收线性表接收情况响应
        msg_type = receive_message(client_fd, &data);
        recv_resp = (ResponseData *)data;
        ret = my_err(ERR_INT, &msg_type, "main/receive_message");
        ret = my_err(ERR_PTR, recv_resp, "main/receive_message");
        if (ret == -1)
        {
            printf("响应解析失败\n");
            free(data);
            break;
        }

        if (recv_resp->status == RESPONSE_STATUS_SUCCESS)
        {
            printf("%s\n", recv_resp->message);
            free(data);
        }

        // 4.阻塞接收同步数据
        // 生成目标文件线性表
        Block_Info_List *send_bklist = bklist_init();
        ret = bklist_updateByFile(send_bklist, fp_dest, send_buf, BUFFER_SIZE);
        ret = my_err(ERR_INT, &ret, "main/bklist_updateByFile");
        if (ret == -1)
        {
            printf("生成目标文件线性表失败\n");
            bklist_delete(send_bklist);
            break;
        }
        while (1)
        {
            msg_type = receive_message(client_fd, &data);
            if (msg_type == MSG_SYNC_COMPLETE) // 同步结束
            {
                // 调试
                printf("-----消息类型为同步结束-----\n");

                printf("同步完成\n");
                free(data);
                break;
            }
            else if (msg_type == MSG_SYNC_DATA) // 同步数据块
            {
                // 调试
                printf("-----消息类型为同步数据块-----\n");

                recv_syncBlockData = (SyncBlockData *)data;
                ret = my_err(ERR_PTR, recv_syncBlockData, "main/receive_message");
                if (ret == -1)
                {
                    printf("解析同步数据块失败\n");
                    send_resp.status = RESPONSE_STATUS_FAILURE;
                    send_resp.message = "解析同步数据块失败";
                    send_message(client_fd, MSG_RESPONSE, &send_resp);

                    free(data);
                    break;
                }

                // 读取内容到缓冲区
                memset(recv_buf, '\0', BUFFER_SIZE);
                strcpy(recv_buf, recv_syncBlockData->content);
                // 计算接受到的数据块的哈希值,与源哈希值比较,正确则向目标文件中写入数据块,否则返回错误响应
                int hax = bklist_calcBlockItemHax(recv_buf, BUFFER_SIZE, "main");

                // 调试
                printf("recv_buf hax = %d, src hax = %d\n", hax, recv_syncBlockData->hax);

                // 计算哈希并与源哈希比较
                // 如果相等,发送成功响应;如果失败,则发送失败响应同时跳过此次文件级同步
                if (hax == recv_syncBlockData->hax)
                {
                    // 判断index与前目标线性表的最大索引的大小关系
                    if (recv_syncBlockData->index > send_bklist->max_index)
                    {
                        // 目标文件线性表max_index + 1
                        send_bklist->max_index = recv_syncBlockData->index;
                    }

                    // 更新目标文件中的数据块
                    int index_temp = recv_syncBlockData->index;
                    int write_size = 0;
                    fseek(fp_dest, index_temp * BUFFER_SIZE, SEEK_SET);
                    ret = fwrite(recv_buf, 1, BUFFER_SIZE, fp_dest);
                    ret = my_err(ERR_INT, &ret, "main/fwrite");
                    if (ret == -1) // 写入失败
                    {
                        printf("写入数据块失败\n");
                        send_resp.status = RESPONSE_STATUS_FAILURE;
                        send_resp.message = "写入数据块失败";
                        send_message(client_fd, MSG_RESPONSE, &send_resp);

                        free(data);
                        break;
                    }

                    // 更新本地同步文件线性表
                    send_bklist->list[index_temp] = hax;

                    printf("数据块同步成功\n");
                    send_resp.status = RESPONSE_STATUS_SUCCESS;
                    send_resp.message = "数据块同步成功";
                    send_message(client_fd, MSG_RESPONSE, &send_resp);

                    free(data);
                    continue;
                }
                else
                {
                    printf("数据块同步失败\n");
                    send_resp.status = RESPONSE_STATUS_FAILURE;
                    send_resp.message = "数据块同步失败";
                    send_message(client_fd, MSG_RESPONSE, &send_resp);

                    free(data);
                    break;
                }
                free(data);
            }
            else
            {
                printf("消息类型错误,需要:%d或%d,实际为:%d\n", MSG_SYNC_COMPLETE, MSG_SYNC_DATA, msg_type);
                break;
                free(data);
            }
        }
        // 回收资源
        bklist_delete(send_bklist);
    }

    /*-----------------------------------------客户端逻辑------------------------------------------*/

    // 断开连接
    close(client_fd);
    return 0;
}
#endif
