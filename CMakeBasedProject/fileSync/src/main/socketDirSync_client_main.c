/*===============================================
*   文件名称：socketDirSync_client_main.c
*   创 建 者：hqyj
*   创建日期：2025年07月26日
*   描    述：跨主机文件夹同步(客户端)

1.  初始化套接字
2.  连接服务器
3.  接收路径数组
4.  为每个路径数组以多进程方式发起连接请求
5.  回收资源
================================================*/
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include "../../include/utils.h"
#include "../../include/hostDirSync.h"
#include "../../include/data_structure.h"
#include "../../include/socketDirSync_client.h"
#include "../../include/socketSingleFileSync_client.h"

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
    strcat(buf1, "/debug/srcDir");
    argv[3] = buf1;

    strcat(buf2, "/debug/destDir");
    argv[4] = buf2;

#else
    if (argc != 5)
    {
        fprintf(stderr, "Usage: %s <server_ip> <server_port> <src_dir_path> <dest_dir_path>\n", argv[0]);
        return 1;
    }
#endif
    // 全局变量
    int client_quit = 0;
    int client_fd;
    int ret = -1;
    char src_path_arr[MAX_FILE_NUM][MAX_DIR_NAME] = {'\0'};
    char dest_path_arr[MAX_FILE_NUM][MAX_DIR_NAME] = {'\0'};
    int file_count = 0;

    // 初始化套接字并连接服务器
    client_fd = client_socket_init(argv[1], argv[2]);
    my_err(ERR_INT, &client_fd, "main/client_socket_init");

    // 1.获取源文件路径数组
    ret = recv_filePath_array(client_fd, argv[3], src_path_arr, &file_count);
    if (ret == -1)
    {
        return -1;
    }

    // 2.在自己的文件夹下创建同步文件
    ret = createSyncDirs(src_path_arr, dest_path_arr, argv[3], argv[4], file_count);
    if (ret == -1)
    {
        return -1;
    }

    // // 调试
    // pathArrPrient(src_path_arr, file_count);
    // printf("\n---------------------------------------------------\n\n");
    // pathArrPrient(dest_path_arr, file_count);

    /*-----------------------------------------客户端逻辑------------------------------------------*/

    // 同步逻辑
    for (int i = 0; i < file_count; ++i)
    {
        usleep(100000); // 休眠0.1秒 -> 进程同步(用于临时模拟效果)


        pid_t pid = fork();

        if (pid == -1)
        {
            return my_err(ERR_INT, &pid, "main/fork");
        }
        else if (pid == 0)
        {
            // 目标文件的文件流指针
            FILE *dest_fp = fopen(dest_path_arr[i], "r+");
            if (-1 == my_err(ERR_PTR, dest_fp, "main/fopen"))
            {
                exit(EXIT_FAILURE);
            }

            // 连接服务器
            int sync_fd = client_socket_init(argv[1], argv[2]);
            if (-1 == my_err(ERR_INT, &sync_fd, "main/client_socket_init"))
            {
                exit(EXIT_FAILURE);
            }

            // 同步源文件中的内容
            ret = recv_content_from_server(sync_fd, dest_fp);
            if (ret == -1)
            {
                exit(EXIT_FAILURE);
            }

            while (client_quit == 0)
            {
                printf("***%s***正在同步…… ---", path_subtract(dest_path_arr[i], argv[4]));
                ret = recv_content_from_server(sync_fd, dest_fp);
                if (ret == -1)
                {
                    exit(EXIT_FAILURE);
                }
                printf("--> ***%s***同步完成\n", path_subtract(dest_path_arr[i], argv[4]));
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

    /*-----------------------------------------客户端逻辑------------------------------------------*/

    // 回收资源
    // 断开连接
    close(client_fd);
    return 0;
}
