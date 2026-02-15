/*===============================================
*   文件名称：socketSingleServer.c
*   创 建 者：hqyj    
*   创建日期：2025年07月28日
*   描    述：有bug但是更节省资源的旧思路
================================================*/

#if 0 // 旧思路
int main(int argc, char *argv[])
{
    int server_quit = 0;
    int server_fd, client_fd;
    int ret = -1;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char recv_buf[BUFFER_SIZE];
    char send_buf[BUFFER_SIZE];
    void *data = NULL;
    char *src_file_path = NULL;
    MessageType msg_type = -1;
    ResponseData send_resp = {0};
    ResponseData *recv_resp = NULL;
    Block_Info_List recv_bklist = {0};
    SyncBlockData sync_block_data = {0};
    FILE *fp_src = NULL;

    // 调试模式下自动填充参数
#if DEBUG
    argv[1] = DEFAULT_SERVER_IP;
    argv[2] = DEFAULT_SERVER_PORT;
#else
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <ip> <port>\n", argv[0]);
        return 1;
    }
#endif

    // 初始化套接字
    server_fd = server_socket_init(argv[1], argv[2]);
    my_err(ERR_INT, &server_fd, "main/server_socket_init");

    // 接收连接请求
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd == -1)
    {
        perror("accept failed");
    }

    /*-----------------------------------------服务器逻辑------------------------------------------*/

    // 1. 接收源文件路径
    while (1)
    {
        msg_type = receive_message(client_fd, &data);
        src_file_path = (char *)data;
        ret = my_err(ERR_INT, &msg_type, "main/receive_message");
        ret = my_err(ERR_PTR, src_file_path, "main/receive_message");
        if (ret == -1)
        {
            // 发送路径错误响应
            send_resp.status = RESPONSE_STATUS_FAILURE;
            send_resp.message = "请求解析失败";
            send_message(client_fd, MSG_RESPONSE, &send_resp);
            free(data);
            sleep(1); // 避免陷入死循环
            continue;
        }
        // 检验源路径是否正确
        fp_src = fopen(src_file_path, "r");
        ret = my_err(ERR_PTR, fp_src, "main/fopen");
        if (ret == -1)
        {
            send_resp.status = RESPONSE_STATUS_FAILURE;
            send_resp.message = "文件打开失败";
            send_message(client_fd, MSG_RESPONSE, &send_resp);
            free(data);
            sleep(1); // 避免陷入死循环
            continue;
        }

        send_resp.status = RESPONSE_STATUS_SUCCESS;
        send_resp.message = "打开源文件成功";
        send_message(client_fd, MSG_RESPONSE, &send_resp);

        free(data);
        break;
        sleep(1); // 避免陷入死循环
    }
    printf("源文件打开成功\n");

    // 将源文件内容复制到目标文件中
    // 计算bklist_src
    Block_Info_List *bklist_src_temp = bklist_init();
    ret = bklist_updateByFile(bklist_src_temp, fp_src, send_buf, BUFFER_SIZE);
    my_err(ERR_INT, &ret, "main/bklist_updateByFile");
    int max_index = bklist_src_temp->max_index;
    bklist_delete(bklist_src_temp);

    // 分块发送数据
    for (int i = 0; i <= max_index; ++i)
    {
        memset(send_buf, '\0', BUFFER_SIZE);
        ret = fread(send_buf, 1, BUFFER_SIZE, fp_src);

        // // 调试
        // printf("%s", send_buf);
        // printf("\n---------------------------------------------------------------------------\n");

        strcpy(sync_block_data.content, send_buf);
        sync_block_data.index = i;
        sync_block_data.hax = max_index; // 用于存储最大索引,让客户端判断何时接收完毕
        send_message(client_fd, MSG_SYNC_DATA, &sync_block_data);

        // // 调试
        // sleep(3);
    }
    // 源文件位置指针回到文件头
    rewind(fp_src);
    printf("文件同步完成\n");

    // 进入同步循环
    while (server_quit == 0)
    {
        // 2.如果触发同步信号,则发送"接收目标文件线性表请求"
        if (1) // 这里是同步触发信号
        {
            // 3.获取客户端"目标文件线性表"
            // 发送"接收目标文件线性表"请求
            send_message(client_fd, MSG_REQUEST, REQUEST_STR_GET_CLIENT_LIST);
            // printf("发送获取线性表请求\n");

            // 接收响应
            msg_type = receive_message(client_fd, &data);
            BlockListData *recv_bklist_temp = (BlockListData *)data;
            // printf("接收线性表,top = %d\n", recv_bklist->top);

            ret = my_err(ERR_INT, &msg_type, "main/receive_message");
            ret = my_err(ERR_PTR, recv_bklist_temp, "main/receive_message");
            if (ret == -1)
            {
                send_resp.status = RESPONSE_STATUS_FAILURE;
                send_resp.message = "解析\"目标文件描述线性表\"失败";
                send_message(client_fd, MSG_RESPONSE, &send_resp);
                printf("解析\"目标文件描述线性表\"失败\n");
                free(data);
                break;
            }

            // 将存储在堆区的目标文件线性表复制到栈区
            bklist_clear(&recv_bklist);
            recv_bklist.max_index = recv_bklist_temp->top;
            for (int i = 0; i <= recv_bklist.max_index; ++i)
            {
                recv_bklist.list[i] = recv_bklist_temp->items[i];
            }
            // printf("线性表复制到栈区成功\n");

            free(data); // 释放堆区中的接收目标文件线性表

            // 发送成功接收到目标文件线性表响应
            send_resp.status = RESPONSE_STATUS_SUCCESS;
            send_resp.message = "服务器:成功接收到\"目标文件线性表\"";
            send_message(client_fd, MSG_RESPONSE, &send_resp);
            printf("成功接收到\"目标文件线性表\"\n");

            // 调试
            printf("目标文件线性表 max_index = %d\n", recv_bklist.max_index);
            for (int i = 0; i <= recv_bklist.max_index; ++i)
            {
                printf("%d ", recv_bklist.list[i]);
            }
            puts("");

            // 4.开始进行同步操作
            // 计算源文件线性表
            Block_Info_List *bklist_src = bklist_init();
            ret = bklist_updateByFile(bklist_src, fp_src, send_buf, BUFFER_SIZE);
            ret = my_err(ERR_INT, &ret, "main/bklist_updateByFile");
            if (ret == -1)
            {
                break;
            }

            // 调试
            printf("源文件线性表 max_index = %d\n", bklist_src->max_index);
            for (int i = 0; i <= bklist_src->max_index; ++i)
            {
                printf("%d ", bklist_src->list[i]);
            }
            puts("");

            for (int i = 0; i <= bklist_src->max_index; ++i)
            {
                // 当第i块数据不同步时
                if (bklist_src->list[i] != recv_bklist.list[i])
                {
                    // 读出源文件中相应位置的内容
                    fseek(fp_src, i * BUFFER_SIZE, SEEK_SET);
                    ret = fread(send_buf, 1, BUFFER_SIZE, fp_src);
                    my_err(ERR_INT, &ret, "main/fread: i * BUFFER_SIZE");

                    // 将[索引号 + 源哈希值 + 源数据块]发送给客户端
                    sync_block_data.index = i;
                    sync_block_data.hax = bklist_src->list[i];
                    strcpy(sync_block_data.content, send_buf);
                    send_message(client_fd, MSG_SYNC_DATA, &sync_block_data);

                    // 调试
                    printf("index = %d\n", sync_block_data.index);
                    printf("hax = %d\n", sync_block_data.hax);
                    // printf("content = %s\n", sync_block_data.content);

                    // 阻塞接收客户端响应
                    msg_type = receive_message(client_fd, &data);
                    recv_resp = (ResponseData *)data;
                    ret = my_err(ERR_INT, &msg_type, "main/receive_message");
                    ret = my_err(ERR_PTR, recv_resp, "main/receive_message");
                    if (ret == -1)
                    {
                        printf("同步块信息响应解析失败\n");
                        break;
                    }

                    // 调试
                    printf("recv_resp.status = %d\n", recv_resp->status);
                    printf("recv_resp.message = %s\n", recv_resp->message);

                    // 成功则修改接收线性表,失败则跳过此次文件级同步
                    if (recv_resp->status == RESPONSE_STATUS_SUCCESS)
                    {
                        printf("%s\n", recv_resp->message);
                    }
                    else
                    {
                        printf("%s\n", recv_resp->message);
                        free(data);
                        break; // 跳过此次文件级同步
                    }
                    free(data);
                }
            }

            // 回收资源
            bklist_delete(bklist_src);

            // 发送"同步结束响应"
            send_message(client_fd, MSG_SYNC_COMPLETE, NULL);

            sleep(2); // 模拟同步信号触发
        }
    }

    /*-----------------------------------------客户端断开------------------------------------------*/

    // 断开连接
    close(client_fd);
    // 关闭服务器套接字
    close(server_fd);
    return 0;
}
#endif