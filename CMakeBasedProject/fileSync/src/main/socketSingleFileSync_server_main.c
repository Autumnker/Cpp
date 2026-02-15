/*===============================================
*   文件名称：socketSingleFileSync_server_main.c
*   创 建 者：hqyj
*   创建日期：2025年07月26日
*   描    述：跨主机单个文件同步(服务器)
1.  接收连接请求

2.  判断文件路径是否正确,正确则响应,不正确则响应+断开

3.  打开源文件并监控状态(CTRL+S),触发同步时,计算源文
    件线性表,同时发送"接收目标文件线性表请求"

4.  阻塞接收目标文件线性表,并返回接收结果(成功则发送
    成功响应,失败则发送重传(接收目标文件线性表请求)请求)

5.  成功接收目标文件线性表后,每当发现哈希值不匹配的,情况,
    就将[源块哈希 + 索引 + 源块文件内容]发送给同步端

6.  阻塞等待目标端响应,如果响应中的哈希值与源块哈希相同
    则修改第一次收到的目标线性表中对应位置的哈希值为接收到
    的哈希值,并且继续检测源-目标线性表直到将源线性表完全遍历,
    完全遍历结束后表示同步完成,此时发送"同步完成请求"

7.  阻塞等待客户端"接收同步完成响应结果的响应",如果未收到则
    每过2秒重发一次"同步完成请求"(连发5次则报错退出),如果收
    到"接收同步完成响应结果的响应"则继续阻塞等待同步事件的触发
================================================*/
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "../../include/utils.h"
#include "../../include/data_structure.h"
#include "../../include/socketSingleFileSync_server.h"

#define DEFAULT_SERVER_IP "192.168.80.128"
#define DEFAULT_SERVER_PORT "8080"

#define DEBUG 1

int main(int argc, char const *argv[])
{
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
    // 全局变量
    int server_quit = 0;
    int server_fd, client_fd;
    int ret = -1;
    FILE *fp_src = NULL;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char src_path[MAX_DIR_NAME] = {'\0'};

    // 初始化套接字
    server_fd = server_socket_init(argv[1], argv[2]);
    my_err(ERR_INT, &server_fd, "main/server_socket_init");

    // 监听连接
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd == -1)
    {
        perror("accept failed");
    }

    /*-----------------------------------------服务器逻辑------------------------------------------*/

    // 获取路径并打开文件
    fp_src = open_file_by_client(client_fd, "r", &src_path);
    if (fp_src == NULL)
    {
        return -1;
    }
    printf("src_path = %s\n", src_path);

    // 发送文件内容到客户端
    ret = send_content_to_client(client_fd, fp_src);
    if (ret == -1)
    {
        return -1;
    }

    // 同步逻辑
    while (server_quit == 0)
    {
        if (listenSyncSingle(src_path))
        {
            printf("***%s***正在同步…… ---", src_path);
            ret = send_content_to_client(client_fd, fp_src);
            if (ret == -1)
            {
                break;
            }
            printf("--> ***%s***同步完成\n", src_path);
        }
    }

    /*-----------------------------------------客户端断开------------------------------------------*/

    // 回收资源
    fclose(fp_src);
    // 断开连接
    close(client_fd);
    // 关闭服务器套接字
    close(server_fd);
    return 0;

    return 0;
}
