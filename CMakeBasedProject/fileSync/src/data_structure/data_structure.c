/*===============================================
*   文件名称：data_structure.c
*   创 建 者：hqyj
*   创建日期：2025年07月24日
*   描    述：常用数据结构
================================================*/
#include <zlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "../../include/utils.h"
#include "../../include/cJSON.h"
#include "../../include/data_structure.h"
/*################################################文件块线性表#################################################*/

int bklist_calcBlockItemHax(char *buff, int buff_size, const char *fun_name)
{
    if (fun_name == NULL)
    {
        printf("%s/bklist_calcBlockItemInfo: 函数名不能为空\n", fun_name);
        return -1;
    }

    if (buff == NULL)
    {
        printf("%s/bklist_calcBlockItemInfo: 参数错误-buff = NULL\n", fun_name);
        return -1;
    }

    if (buff_size <= 0)
    {
        printf("%s/bklist_calcBlockItemHax: buffer_size <= 0\n", fun_name);
        return -1;
    }

    // if (buff_size > BLOCK_SIZE)
    // {
    //     printf("%s/bklist_calcBlockItemInfo: 参数错误-buff的长度(%d)大于BLOCK_SIZE(%d)\n", fun_name, buff_size, BLOCK_SIZE);
    //     return -1;
    // }

    // 计算块信息
#if 1 // 旧思路
    int hax = 0;
    for (int i = 0; i < buff_size; ++i)
    {
        hax += buff[i]; // int的表示范围 > BUFFER_SIZE(在主函数文件中定义为4069) * 255
    }
    return hax;
#else
    // 新思路
    int hax = crc32(0, (const Bytef *)buff, buff_size) & 0x7FFFFFFF; // 最高位置0(为了使uint_32与int类型匹配)
    return hax;
#endif
}

Block_Info_List *bklist_init()
{
    Block_Info_List *node = (Block_Info_List *)malloc(sizeof(Block_Info_List));
    if (node == NULL)
    {
        perror("bklist_init/malloc");
        return NULL;
    }
    node->max_index = -1;
    for (int i = 0; i < BLOCK_SIZE; ++i)
    {
        node->list[i] = 0;
    }
    return node;
}

int bklist_delete(Block_Info_List *ptr)
{
    if (ptr == NULL)
    {
        printf("bklist_delete: 参数错误-ptr为空\n");
        return -1;
    }
    free(ptr);
    return 0;
}

int bklist_get(Block_Info_List *line_list, int index)
{
    if (line_list == NULL)
    {
        printf("bklist_get: 参数错误-line_list = NULL\n");
        return -1;
    }

    if (index < 0 || index >= BLOCK_SIZE)
    {
        printf("bklist_get: 参数错误-index不在合理范围内[0,%d]\n", BLOCK_SIZE - 1);
        return -1;
    }

    if (index > line_list->max_index)
    {
        printf("bklist_get: 参数错误-index大于当前最大索引[0,%d]\n", line_list->max_index);
        return -1;
    }

    return line_list->list[index];
}

int bklist_insert(Block_Info_List *line_list, char *buff, int buff_size)
{
    if (line_list == NULL)
    {
        printf("bklist_insert: 参数错误-line_list = NULL\n");
        return -1;
    }

    // 计算哈希值
    int hex = bklist_calcBlockItemHax(buff, buff_size, "bklist_insert");
    if (hex < 0)
    {
        printf("bklist_insert: hex < 0\n");
        return -1;
    }

    line_list->max_index++;
    if (line_list->max_index >= BLOCK_SIZE)
    {
        printf("bklist_insert: 线性表满\n");
        return -1;
    }
    line_list->list[line_list->max_index] = hex;

    return 0;
}

int bklist_updateByFile(Block_Info_List *line_list, FILE *fp, char *buffer, int buffer_size)
{
    if (line_list == NULL)
    {
        printf("bklist_updateByFile: 参数错误-line_list = NULL\n");
        return -1;
    }

    if (fp == NULL)
    {
        printf("bklist_updateByFile: 参数错误-fp = NULL\n");
        return -1;
    }

    if (buffer == NULL)
    {
        printf("bklist_updateByFile: 参数错误-buffer = NULL\n");
        return -1;
    }

    if (buffer_size <= 0)
    {
        printf("bklist_updateByFile: 参数错误-buffer——size <= 0\n");
        return -1;
    }

    // 清空线性表
    bklist_clear(line_list);
    fflush(fp);

    // 将文件位置指针移动到文件头
    rewind(fp);

    // 循环读取文件
    int ret = -1;
    int hax = 0;
    while (ret != 0)
    {
        memset(buffer, '\0', buffer_size); // 清空缓冲区
        ret = fread(buffer, sizeof(char), buffer_size, fp);
        if (-1 == ret)
        {
            perror("bklist_updateByFile/fread");
            bklist_clear(line_list);
            // 将文件位置指针移动到文件头
            rewind(fp);
            return -1;
        }

        // 插入
        if (-1 == bklist_insert(line_list, buffer, buffer_size))
        {
            bklist_clear(line_list);
            // 将文件位置指针移动到文件头
            rewind(fp);
            return -1;
        }
    }
    // 将文件位置指针移动到文件头
    rewind(fp);
    return 0;
}

int bklist_change(Block_Info_List *line_list, int index, int list_item)
{
    if (line_list == NULL)
    {
        printf("bklist_change: 参数错误-line_list = NULL\n");
        return -1;
    }

    if (index < 0 || index > line_list->max_index)
    {
        printf("bklist_change: 参数错误-index不在正确范围内[0,%d]\n", line_list->max_index);
        return -1;
    }

    line_list->list[index] = list_item;
    return 0;
}

void bklist_clear(Block_Info_List *line_list)
{
    if (line_list == NULL)
    {
        printf("bklist_clear: 参数错误-line_list = NULL\n");
        return;
    }

    for (int i = 0; i <= BLOCK_SIZE; ++i)
    {
        line_list->list[i] = -1;
    }
    line_list->max_index = -1;
}

void bklist_trunc(Block_Info_List *line_list, int index)
{
    if (line_list == NULL)
    {
        printf("bklist_trunc: 参数错误-line_list = NULL\n");
        return;
    }

    if (index < 0 || index > line_list->max_index)
    {
        printf("bklist_trunc: 参数错误-index[%d] 不在正确的范围内[0,%d]\n", index, line_list->max_index);
        return;
    }

    for (int i = index; i <= line_list->max_index; ++i)
    {
        line_list->list[i] = -1;
    }
    line_list->max_index = index - 1;
}

int bklist_getDifferIndex(Block_Info_List *line_list_src, Block_Info_List *line_list_dest)
{
    if (line_list_src == NULL)
    {
        printf("bklist_getDifferIndex: 参数错误-line_listA = NULL\n");
        return -1;
    }

    if (line_list_dest == NULL)
    {
        printf("bklist_getDifferIndex: 参数错误-line_listB = NULL\n");
        return -1;
    }

    int max_src = line_list_src->max_index;
    int max_dest = line_list_dest->max_index;

    if (max_src < max_dest)
    {
        return -3; // 说明源文件比目标文件小，需要截断目标文件
    }

    // 如果目标线性表未初始化，其max_index = 0
    if (max_dest < 0)
    {
        return 0; // 从头开始
    }
    // 找不同
    for (int i = 0; i <= max_dest; ++i)
    {
        if (line_list_src->list[i] != line_list_dest->list[i])
        {
            return i;
        }
    }
    // 如果前minIndex的数据中没有不同的
    if (max_src > max_dest)
    {
        return ++max_dest;
    }

    if (max_src == max_dest)
    {
        return -2;
    }

    return -1;
}

void bklist_print(Block_Info_List *line_list)
{
    if (line_list == NULL)
    {
        printf("bklist_print: 参数错误-line_list = NULL\n");
        return;
    }

    for (int i = 0; i <= line_list->max_index; ++i)
    {
        if (line_list->list[i] == -1)
        {
            break;
        }
        printf("%d ", line_list->list[i]);
    }
}

/*################################################文件夹栈#################################################*/

Dir_Stack *dstack_init()
{
    Dir_Stack *node = (Dir_Stack *)malloc(sizeof(Dir_Stack));
    if (node == NULL)
    {
        perror("dstack_init");
        return NULL;
    }
    node->top = -1;
    for (int i = 0; i < MAX_STACK_SIZE; ++i)
    {
        memset(node->dir[i], '\0', MAX_DIR_NAME);
    }
    return node;
}

int dstack_delete(Dir_Stack *dir_stack)
{
    if (dir_stack == NULL)
    {
        printf("dstack_delete: dir_stack = NULL\n");
        return -1;
    }
    free(dir_stack);
}

int dstack_push(Dir_Stack *dir_stack, const char *dir_name)
{
    if (dir_stack == NULL)
    {
        printf("dstack_push: dir_stack = NULL\n");
        return -1;
    }
    if (dir_name == NULL)
    {
        printf("dstack_push: dir_name = NULL\n");
        return -1;
    }
    if (dir_stack->top >= MAX_STACK_SIZE)
    {
        printf("dstack_push: 栈满\n");
        return -1;
    }

    dir_stack->top++;
    strcpy(dir_stack->dir[dir_stack->top], dir_name);
    return 0;
}

int dstack_pop(Dir_Stack *dir_stack, char *dir_name)
{
    if (dir_stack == NULL)
    {
        printf("dstack_pop: dir_stack = NULL\n");
        return -1;
    }
    if (dir_name == NULL)
    {
        printf("dstack_pop: dir_name = NULL\n");
        return -1;
    }
    if (dir_stack->top < 0)
    {
        printf("dstack_push: 栈空\n");
        return -1;
    }

    strcpy(dir_name, dir_stack->dir[dir_stack->top]);
    memset(dir_stack->dir[dir_stack->top], '\0', MAX_DIR_NAME);
    dir_stack->top--;
    return 0;
}

int dstack_top(Dir_Stack *dir_stack, char *dir_name)
{
    if (dir_stack == NULL)
    {
        printf("dstack_pop: dir_stack = NULL\n");
        return -1;
    }
    if (dir_name == NULL)
    {
        printf("dstack_pop: dir_name = NULL\n");
        return -1;
    }
    if (dir_stack->top < 0)
    {
        printf("dstack_push: 栈空\n");
        return -1;
    }

    strcpy(dir_name, dir_stack->dir[dir_stack->top]);
    return 0;
}

int dstack_empty(Dir_Stack *dir_stack)
{
    if (dir_stack == NULL)
    {
        printf("dstack_empty: dir_stack = NULL\n");
        return -1;
    }

    if (dir_stack->top == -1)
    {
        return 1;
    }
    else if (dir_stack->top >= 0)
    {
        return 0;
    }
    else
    {
        printf("dstack_empty: dir_stack->top < -1\n");
        return -1;
    }

    return -1;
}

void dstack_print(Dir_Stack *dir_stack)
{
    if (dir_stack == NULL)
    {
        printf("dstack_print: dir_stack = NULL\n");
        return;
    }

    for (int i = 0; i <= dir_stack->top; ++i)
    {
        printf("%s ", dir_stack->dir[i]);
    }
}

/*################################################跨主机通信相关#################################################*/

// 封装用于跨主机通信的 JSON 消息
char *build_json_message(MessageType msg_type, void *data)
{
    cJSON *root = cJSON_CreateObject();

    switch (msg_type)
    {
    case MSG_SOURCE_PATH:; // 源文件路径
        {
            char *path = (char *)data;
            cJSON_AddStringToObject(root, "type", MSG_TYPE_STR_SOURCE_PATH);
            cJSON_AddStringToObject(root, "path", path);
            break;
        }
    case MSG_RESPONSE:; // 响应消息
        {
            ResponseData *resp = (ResponseData *)data;
            cJSON_AddStringToObject(root, "type", MSG_TYPE_STR_RESPONSE);
            cJSON_AddStringToObject(root, "status", resp->status ? "success" : "fail");
            cJSON_AddStringToObject(root, "message", resp->message);
            break;
        }
    case MSG_REQUEST:; // 请求消息
        {
            char *request = (char *)data;
            cJSON_AddStringToObject(root, "type", MSG_TYPE_STR_REQUEST);
            cJSON_AddStringToObject(root, "request", request);
            break;
        }
    case MSG_CLIENT_LIST:; // 同步文件描述线性表
        {
            BlockListData *list = (BlockListData *)data;
            cJSON_AddStringToObject(root, "type", MSG_TYPE_STR_CLIENT_BLOCK_LIST);
            cJSON *list_obj = cJSON_CreateObject();
            cJSON_AddNumberToObject(list_obj, "top", list->top);
            cJSON_AddItemToObject(list_obj, "items", cJSON_CreateIntArray(list->items, list->top + 1));
            cJSON_AddItemToObject(root, "list", list_obj);
            break;
        }
    case MSG_SYNC_DATA:; // 源文件索引/哈希/块信息
        {
            SyncBlockData *sync = (SyncBlockData *)data;
            cJSON_AddStringToObject(root, "type", MSG_TYPE_STR_SYNC_DATA);
            cJSON_AddNumberToObject(root, "index", sync->index);
            cJSON_AddNumberToObject(root, "hax", sync->hax);
            cJSON_AddStringToObject(root, "content", sync->content);
            break;
        }
    case MSG_SYNC_COMPLETE:; // "同步完成"消息
        cJSON_AddStringToObject(root, "type", MSG_TYPE_STR_SYNC_COMPLETE);
        break;
    }

    char *json_str = cJSON_PrintUnformatted(root); // cJSON_PrintUnformatted返回的是堆区指针,必须手动释放
    cJSON_Delete(root);
    return json_str;
}

// 发送信息
void send_message(int socket_fd, MessageType msg_type, void *data)
{
    // 0.接收函数返回值
    int ret = -1;

    // 1. 构建 JSON
    char *json_str = build_json_message(msg_type, data);

    // 2. 发送长度头（解决粘包）
    uint32_t json_len = strlen(json_str);
    uint32_t net_len = htonl(json_len);
    ret = send(socket_fd, &net_len, sizeof(net_len), 0);
    my_err(ERR_INT, &ret, "send_message/send: net_len");

    // 3. 发送 JSON 数据
    ret = send(socket_fd, json_str, json_len, 0);
    my_err(ERR_INT, &ret, "send_message/send: json_str");

    free(json_str);
}

// 接收并解析数据
MessageType receive_message(int socket_fd, void **parsed_data)
{
    // 接收函数返回值
    int ret = -1;

    // 1. 接收长度头
    uint32_t net_len;
    ret = recv(socket_fd, &net_len, sizeof(net_len), 0);
    my_err(ERR_INT, &ret, "receive_message/recv: net_len");

    uint32_t json_len = ntohl(net_len);

    // 2. 接收 JSON 数据
    char *json_str = malloc(json_len + 1);
    ret = recv(socket_fd, json_str, json_len, 0);
    my_err(ERR_INT, &ret, "receive_message/recv: json_str");

    json_str[json_len] = '\0';

    // 3. 解析 JSON
    cJSON *root = cJSON_Parse(json_str);
    if (!root)
    {
        fprintf(stderr, "receive_message/JSON 解析失败: %s\n", cJSON_GetErrorPtr());
        free(json_str);
        return -1;
    }

    // 4. 根据类型处理数据
    cJSON *type = cJSON_GetObjectItem(root, "type");
    MessageType msg_type = -1;                                    // 消息类型错误时返回 -1
    if (strcmp(type->valuestring, MSG_TYPE_STR_SOURCE_PATH) == 0) // 源文件路径
    {
        msg_type = MSG_SOURCE_PATH;
        char *path = strdup(cJSON_GetObjectItem(root, "path")->valuestring);
        *parsed_data = path;
    }
    else if (strcmp(type->valuestring, MSG_TYPE_STR_RESPONSE) == 0) // 响应消息
    {
        msg_type = MSG_RESPONSE;
        ResponseData *resp = malloc(sizeof(ResponseData));
        resp->status = strcmp(cJSON_GetObjectItem(root, "status")->valuestring, "success") == 0;
        resp->message = strdup(cJSON_GetObjectItem(root, "message")->valuestring);
        *parsed_data = resp;
    }
    else if (strcmp(type->valuestring, MSG_TYPE_STR_REQUEST) == 0) // 请求消息
    {
        msg_type = MSG_REQUEST;
        char *request = strdup(cJSON_GetObjectItem(root, "request")->valuestring);
        *parsed_data = request;
    }
    else if (strcmp(type->valuestring, MSG_TYPE_STR_SYNC_DATA) == 0) // 源文件索引/哈希/块信息
    {
        msg_type = MSG_SYNC_DATA;
        SyncBlockData *sync = malloc(sizeof(SyncBlockData));
        sync->index = cJSON_GetObjectItem(root, "index")->valueint;
        sync->hax = cJSON_GetObjectItem(root, "hax")->valueint;
        strcpy(sync->content, cJSON_GetObjectItem(root, "content")->valuestring);
        *parsed_data = sync;
    }
    else if (strcmp(type->valuestring, MSG_TYPE_STR_CLIENT_BLOCK_LIST) == 0) // 同步文件描述线性表
    {
        msg_type = MSG_CLIENT_LIST;
        BlockListData *list = (BlockListData *)malloc(sizeof(BlockListData));
        memset(list->items, -1, BLOCK_SIZE); // 初始化线性表以接收数据
        // 解析list对象
        cJSON *list_obj = cJSON_GetObjectItem(root, "list");
        if (list_obj && cJSON_IsObject(list_obj))
        {
            // 解析top字段
            cJSON *top_val = cJSON_GetObjectItem(list_obj, "top");
            if (top_val && cJSON_IsNumber(top_val))
            {
                list->top = top_val->valueint;
                // 解析items数组
                cJSON *items_obj = cJSON_GetObjectItem(list_obj, "items");
                if (items_obj && cJSON_IsArray(items_obj))
                {
                    for (int i = 0; i <= list->top; i++)
                    {
                        cJSON *item = cJSON_GetArrayItem(items_obj, i);
                        if (item && cJSON_IsNumber(item))
                        {
                            list->items[i] = item->valueint;
                        }
                    }
                }
            }
        }

        *parsed_data = list;
    }
    else if (strcmp(type->valuestring, MSG_TYPE_STR_SYNC_COMPLETE) == 0) // "同步完成"消息
    {
        msg_type = MSG_SYNC_COMPLETE;
        *parsed_data = NULL;
    }

    cJSON_Delete(root);
    free(json_str);
    return msg_type;
}
