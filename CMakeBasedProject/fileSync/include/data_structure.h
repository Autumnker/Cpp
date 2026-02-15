/*===============================================
*   文件名称：data_structure.h
*   创 建 者：hqyj
*   创建日期：2025年07月24日
*   描    述：常用数据结构
================================================*/
#ifndef _DATA_STRUCTURE_H
#define _DATA_STRUCTURE_H

#include <stdio.h>

/*################################################文件块线性表#################################################*/

#define BUFFER_SIZE 4069 // 文件块字节数(缓冲区大小)
#define BLOCK_SIZE 2048  // 线性表最大长度

/**
 * @struct file_block_info
 * @brief 块式线性表结构体
 * @var max_index  当前最大有效索引（0 ~ BLOCK_SIZE-1）
 * @var list      数据存储数组（固定长度 BLOCK_SIZE）
 */
typedef struct file_block_info
{
    int max_index;        // 当前最大索引（-1表示空表）
    int list[BLOCK_SIZE]; // 数据存储数组
} Block_Info_List;

/**
 * @brief 初始化块式线性表结构体
 * @return 块式线性表结构体指针
 * @warning 需要调用"bklist_delete"函数销毁该指针
 */
Block_Info_List *bklist_init();

/**
 * @brief 销毁块式线性表结构体指针
 * @param ptr 需要被销毁的块式线性表结构体指针
 * @return int 成功返回 0 失败返回 -1
 */
int bklist_delete(Block_Info_List *ptr);

/**
 * @brief 计算文件块buff中数据的哈希值
 * @param buff 存储文件块的缓冲区
 * @param buff_size 缓冲区大小
 * @param fun_name 调用该函数的函数名，用于返回错误信息
 * @return int 成功返回文件块哈希值 失败返回 -1
 */
int bklist_calcBlockItemHax(char *buff, int buff_size, const char *fun_name);

/**
 * @brief 获取线性表中指定索引的值
 * @param line_list 线性表指针
 * @param index     要访问的索引（0 ≤ index ≤ max_index）
 * @return int      成功返回对应位置的值，失败返回 -1
 */
int bklist_get(Block_Info_List *line_list, int index);

/**
 * @brief 向线性表插入数据
 * @param line_list 线性表指针
 * @param buff      待插入数据缓冲区指针
 * @param buff_size 缓冲区大小
 * @return int      成功返回插入后的 max_index，失败返回 -1
 * @warning
 * - 如果 buff_size 超过 BLOCK_SIZE，会自动截断
 * - 当表满时（max_index == BLOCK_SIZE-1），新数据会覆盖最旧数据（循环缓冲）
 */
int bklist_insert(Block_Info_List *line_list, char *buff, int buff_size);

/**
 * @brief 用文件流指针更新块式线性表
 * @param line_list 线性表指针
 * @param fp 文件流指针
 * @param buffer 读取文件内容缓冲区指针
 * @param buffer_size 读取文件内容缓冲区大小
 * @return int 成功返回0 失败返回 -1
 */
int bklist_updateByFile(Block_Info_List *line_list, FILE *fp, char *buffer, int buffer_size);

/**
 * @brief 修改线性表中指定位置的值
 * @param line_list 线性表指针
 * @param index     要修改的索引（0 ≤ index ≤ max_index）
 * @param list_item 新值
 * @return int      成功返回 0，失败返回 -1（索引越界）
 */
int bklist_change(Block_Info_List *line_list, int index, int list_item);

/**
 * @brief 重置线性表（不清空内存，仅重置指针）
 * @param line_list 线性表指针
 * @note 清空后 max_index = -1
 */
void bklist_clear(Block_Info_List *line_list);

/**
 * @brief 截断线性表
 * @param line_list 线性表指针
 * @param index 截断的起始位置(包含index)
 * @note 清空后 max_index = -1
 */
void bklist_trunc(Block_Info_List *line_list, int index);

/**
 * @brief 比较两个线性表的第一个差异点
 * @param line_listA 线性表A指针
 * @param line_listB 线性表B指针
 * @return int 成功返回第一个不同值的索引，完全相同时返回 -2， 错误返回 -1, 需要截断目标文件时返回 -3
 * @note 比较范围为两表共有的有效数据（min(max_indexA, max_indexB)）
 */
int bklist_getDifferIndex(Block_Info_List *line_list_src, Block_Info_List *line_list_dest);

/**
 * @brief 打印块式线性表中的数据
 * @param line_list 块式线性表结构体指针
 */
void bklist_print(Block_Info_List *line_list);

/*################################################文件夹栈#################################################*/

#define MAX_DIR_NAME 1024   // 文件夹名最大长度
#define MAX_STACK_SIZE 1024 // 栈最大容量
#define MAX_FILE_NUM 2048   // 最大文件数量

/**
 * @brief 存储文件夹名称的栈
 * @var dir 文件夹名称
 * @var top 栈顶指针, -1 表示栈空
 */
typedef struct dir_stack
{
    char dir[MAX_STACK_SIZE][MAX_DIR_NAME];
    int top;
} Dir_Stack;

/**
 * @brief 初始化文件夹栈
 * @return Dir_Stack* 成功返回文件夹栈地址 失败返回 NULL
 */
Dir_Stack *dstack_init();

/**
 * @brief 删除文件夹栈(回收资源)
 * @param dir_stack 文件夹栈指针
 * @return int 成功返回 0 失败返回 -1
 */
int dstack_delete(Dir_Stack *dir_stack);

/**
 * @brief 将文件夹名压入栈中
 * @param dir_stack 文件夹栈指针
 * @param dir_name 要被压入栈的文件夹名称
 * @return int 成功返回 0 失败返回 -1
 */
int dstack_push(Dir_Stack *dir_stack, const char *dir_name);

/**
 * @brief 将栈顶的文件夹名字弹出
 * @param dir_stack 文件夹栈指针
 * @param dir_name 存储文件夹名子的字符数组的首地址
 * @return int 成功返回 0 失败返回 -1
 */
int dstack_pop(Dir_Stack *dir_stack, char *dir_name);

/**
 * @brief 将栈顶的文件夹名字赋值给dir_name但不弹出
 * @param dir_stack 文件夹栈指针
 * @param dir_name 存储文件夹名子的字符数组的首地址
 * @return int 成功返回 0 失败返回 -1
 */
int dstack_top(Dir_Stack *dir_stack, char *dir_name);

/**
 * @brief 检测文件夹栈是否为空
 * @param dir_stack 文件夹栈指针
 * @return int 空栈返回 1 否则返回 0 错误返回 -1
 */
int dstack_empty(Dir_Stack *dir_stack);

/**
 * @brief 打印文件夹栈
 * @param dir_satck 文件夹栈指针
 */
void dstack_print(Dir_Stack *dir_stack);

/*################################################跨主机通信相关#################################################*/

/*

消息格式类型:

一、源文件地址路径名
{
    类型:(客户端向服务器发送的)源文件地址路径
    信息:源文件路径地址
}

二、响应信息
{
    类型:对方刚刚发送的请求的响应结果(客户端/服务器双向发送)
    状态:成功/失败
    信息:对状态(导致这个状态的原因)的具体描述
}

三、客户端向服务器发送自己的文件描述线性表(int top 和 int list[STACK_SIZE])
{
    类型:客户端向服务器发送自己的线性表
    线性表: {
                int top;
                int list[STACK_SIZE];
            }
}

四、服务器发送给客户端的用于同步的信息
{
    类型:服务器发送给客户端的同步消息(客户端需要用这些消息来同步自己的数据)
    索引:同步索引
    哈希值:整形int
    源块内容:字符串[BUFFER_SIZE]
}

五、服务器向客户端发送"同步完成响应"
{
    类型: 服务器告诉客户端同步完成
}

*/

/**响应状态码 */
#define RESPONSE_STATUS_SUCCESS 1 // 成功响应状态码
#define RESPONSE_STATUS_FAILURE 0 // 失败响应状态码

/**消息类型字符串 */
#define MSG_TYPE_STR_SOURCE_PATH "source_path"       // 消息类型-源文件路径
#define MSG_TYPE_STR_RESPONSE "response"             // 消息类型-响应
#define MSG_TYPE_STR_REQUEST "request"               // 消息类型-请求
#define MSG_TYPE_STR_CLIENT_BLOCK_LIST "client_list" // 消息类型-同步文件(目标文件)描述线性表
#define MSG_TYPE_STR_SYNC_DATA "sync_data"           // 消息类型-同步源块数据
#define MSG_TYPE_STR_SYNC_COMPLETE "sync_complete"   // 消息类型-同步完成

/**请求类型字符串 */
#define REQUEST_STR_GET_CLIENT_LIST "get_client_list" // 获取客户端同步文件线性表

/**
 * @brief 消息类型枚举
 * @param MSG_SOURCE_PATH 源文件路径(客户端向服务端发送的请求同步的源文件地址)
 * @param MSG_RESPONSE 响应信息(对某一个请求的响应)
 * @param MSG_REQUEST 请求信息(向对方发送请求)
 * @param MSG_CLIENT_LIST 客户端同步文件(目标文件)的文件描述线性表
 * @param MSG_SYNC_DATA 服务器发送给客户端的用于同步文件块的信息
 * @param MSG_SYNC_COMPLETE 服务器向客户端发送的"同步结束"的消息
 */
typedef enum
{
    MSG_SOURCE_PATH,  // 源文件路径(客户端向服务端发送的请求同步的源文件地址)
    MSG_RESPONSE,     // 响应信息(对某一个请求的响应)
    MSG_REQUEST,      // 请求信息(向对方发送请求)
    MSG_CLIENT_LIST,  // 客户端同步文件(目标文件)的文件描述线性表
    MSG_SYNC_DATA,    // 服务器发送给客户端的用于同步文件块的信息
    MSG_SYNC_COMPLETE // 服务器向客户端发送的"同步结束"的消息
} MessageType;

/**
 * @brief 响应结构体
 * @param status 状态(失败=RESPONSE_STATUS_FAILURE,成功=RESPONSE_STATUS_SUCCESS)
 * @param message 状态描述信息
 */
typedef struct
{
    int status;    // 0=失败, 1=成功
    char *message; // 描述信息
} ResponseData;

/**
 * @brief 客户端发送给服务器的目标文件描述线性表
 * @param top 线性表最大索引
 * @param items 线性表
 */
typedef struct
{
    int top;
    int items[BLOCK_SIZE];
} BlockListData;

/**
 * @brief 服务器发送给客户端用于同步的源文件块信息
 * @param index 需要同步的块的索引
 * @param hax 同步块在源文件线性表中的哈希值
 * @param content 同步块在源文件中的内容
 */
typedef struct
{
    int index;
    int hax;
    char content[BUFFER_SIZE];
} SyncBlockData;

/**
 * @brief 封装用于跨主机通信的 JSON 消息
 * @param msg_type 消息类型,定义在MessageType枚举中
 * @param data 消息
 * @return char* 成功返回消息字符串首地址, 失败返回 NULL
 * @warning 返回值: 返回的指针指向的是一块堆内存,因为函数内部使用了
 * cJSON_PrintUnformatted ,使用完毕后必须手动释放该内存以避免内存泄漏
 */
char *build_json_message(MessageType msg_type, void *data);

/**
 * @brief 发送信息
 * @param socket_fd 通信套接字
 * @param msg_type 消息类型
 * @param data 将要发送的数据(需要与 msg_type 保持一致)
 */
void send_message(int socket_fd, MessageType msg_type, void *data);

/**
 * @brief 接收并解析数据
 * @param socket_fd 通信套接字
 * @param parsed_data 传出参数,用于指向所接收到的消息的地址
 * @return MessageType 成功:返回所接收到的消息的类型,可用于判断用哪种结构体转化
 * pparsed_data的指针类型,以读取消息内容 失败:消息类型错误时返回-1
 * @warning parsed_data 指向的是一块堆内存,因为函数内部使用了strdup、malloc，
 * 使用完毕后必须手动释放parsed_data,以避免内存泄漏
 */
MessageType receive_message(int socket_fd, void **parsed_data);

#endif
