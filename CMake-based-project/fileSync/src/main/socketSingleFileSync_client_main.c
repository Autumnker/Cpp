/*===============================================
*   文件名称：socketSingleFileSync_client_main.c
*   创 建 者：hqyj
*   创建日期：2025年07月26日
*   描    述：跨主机单个文件同步(客户端)
1.  发送连接请求[目标ip + (服务器上的)源文件路径 +
    (客户端上的)目标文件路径]

2.  阻塞接收服务器发送的"接收目标线性表请求",收
    到请求后,计算并发送目标线性表

3.  阻塞接"收服务器是否成功接收到目标文件线性表的响应",
    如果成功,则进入下一步;失败,则重新发送并再次等待
    "收服务器是否成功接收到目标文件线性表的响应"

4.  阻塞接收[待修改文件块索引,源块哈希值,源文件块]
    <或者是否同步完成响应,如果接收到同步完成响应,则返回
    "收到同步结束请求,同时退出">,
    接收成功则将目标文件相应位置的数据清空,再将源文件块内容
    写入目标文件中对应位置,再计算已经写入的数据块的哈希值,
    如果与源块哈希值相等,则修改自己对应位置的线性表,并发送
    "块同步成功响应",否则发送"同步失败请求"
================================================*/
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "../../include/utils.h"
#include "../../include/data_structure.h"
#include "../../include/socketSingleFileSync_client.h"

#define DEFAULT_SERVER_IP "192.168.80.128"
#define DEFAULT_SERVER_PORT "8080"

#define DEBUG 1

int main(int argc, char const *argv[])
{
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
    // 全局变量
    int client_quit = 0;
    int client_fd;
    int ret = -1;
    FILE *fp_dest = NULL;

    // 初始化套接字并连接服务器
    client_fd = client_socket_init(argv[1], argv[2]);
    my_err(ERR_INT, &client_fd, "main/client_socket_init");

    // 发送源路径并同步文件中的内容
    // 1.发送源文件路径
    ret = send_sourceFile_path(client_fd, argv[3]);
    if (ret == -1)
    {
        return -1;
    }

    // 2.在自己的文件夹下创建同步文件
    fp_dest = create_sync_file(argv[4], "r+");

    // 3.同步源文件中的内容
    ret = recv_content_from_server(client_fd, fp_dest);
    if (ret == -1)
    {
        return -1;
    }

    /*-----------------------------------------客户端逻辑------------------------------------------*/

    // 同步逻辑
    while (client_quit == 0)
    {
        printf("***%s***正在同步…… ---", argv[4]);
        ret = recv_content_from_server(client_fd, fp_dest);
        if (ret == -1)
        {
            break;
        }
        printf("--> ***%s***同步完成\n", argv[4]);
    }

    /*-----------------------------------------客户端逻辑------------------------------------------*/

    // 回收资源
    fclose(fp_dest);
    // 断开连接
    close(client_fd);
    return 0;
}
