/*===============================================
*   文件名称：socketDirSync_server_main.c
*   创 建 者：hqyj
*   创建日期：2025年07月26日
*   描    述：跨主机文件夹同步(服务器)

1.  初始化套接字
2.  监听连接请求
3.  接收源文件夹路径
4.  创建文件路径数据并发送给客户端
5.  循环创建进程进行文件夹同步
6.  回收资源
================================================*/
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "../../include/utils.h"
#include "../../include/hostDirSync.h"
#include "../../include/data_structure.h"
#include "../../include/socketDirSync_server.h"
#include "../../include/socketSingleFileSync_server.h"

// #define DEFAULT_SERVER_IP "192.168.6.172"
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
    int run = 1;
    int server_fd, client_fd;
    int ret = -1;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char path_arr[MAX_FILE_NUM][MAX_DIR_NAME] = {'\0'};
    int file_count = 0;
    char src_dir[MAX_DIR_NAME] = {'\0'};

    // 初始化套接字
    server_fd = server_socket_init(argv[1], argv[2]);
    my_err(ERR_INT, &server_fd, "main/server_socket_init");

    // 监听连接
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd == -1)
    {
        perror("accept failed");
    }

    // 获取路径并创建文件路径数组
    ret = send_filePath_array(client_fd, src_dir, path_arr, &file_count);
    if (ret == -1)
    {
        return -1;
    }

    // // 调试
    // pathArrPrient(path_arr, file_count);

    /*-----------------------------------------服务器逻辑------------------------------------------*/

    // 同步逻辑
    for (int i = 0; i < file_count; ++i)
    {
        usleep(100000); // 休眠0.1秒 -> 进程同步(用于临时模拟效果)

        // 监听同步连接请求
        int sync_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (sync_fd == -1)
        {
            perror("accept failed");
        }

        pid_t pid = fork();

        if (pid == -1)
        {
            return my_err(ERR_INT, &pid, "main/fork");
        }
        else if (pid == 0)
        {
            FILE *src_fp = fopen(path_arr[i], "r");
            if (-1 == my_err(ERR_PTR, src_fp, "main/fopen"))
            {
                exit(EXIT_FAILURE);
            }

            // 发送第一次同步所需文件内容到客户端
            if (-1 == send_content_to_client(sync_fd, src_fp))
            {
                exit(EXIT_FAILURE);
            }

            while (server_quit == 0)
            {
                if (listenSyncSingle(path_arr[i]))
                {
                    printf("***%s***正在同步…… ---", path_subtract(path_arr[i], src_dir));
                    ret = send_content_to_client(sync_fd, src_fp);
                    if (ret == -1)
                    {
                        exit(EXIT_FAILURE);
                    }
                    printf("--> ***%s***同步完成\n", path_subtract(path_arr[i], src_dir));
                }
            }

            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < file_count; ++i)
    {
        int wstat;
        pid_t qpid = waitpid(-1, &wstat, 0);
        printf("子进程%d已退出,退出状态为%d\n", qpid, wstat);
    }

    /*-----------------------------------------客户端断开------------------------------------------*/

    // 回收资源
    // 断开连接
    close(client_fd);
    // 关闭服务器套接字
    close(server_fd);

    return 0;
}
