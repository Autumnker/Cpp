/*===============================================
*   文件名称：threadPool.h
*   创 建 者：Autumnker
*   创建日期：2025年08月02日
*   描    述：
================================================*/
#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#define MANAGER_FREQUENCY 3  // 线程池管理线程的管理频率(单位：秒/次)

// 定义任务结构体
typedef struct Task {
    void (*fun)(void* arg);
    void* arg;
} Task;

// 定义线程池
typedef struct ThreadPool ThreadPool;

/**
 * @brief 初始化线程池
 * @param taskQueueSize 任务队列长度
 * @param threadMinNum 最小线程数
 * @param threadMaxNum 最大线程数
 * @return ThreadPool* 成功返回线程池指针, 失败返回NULL
 */
ThreadPool* initThreadPool(int taskQueueSize, int threadMinNum, int threadMaxNum);

/**
 * @brief 添加任务到线程池
 * @param pool 线程池指针
 * @param task 任务结构体(拷贝传参)
 * @return int 成功返回0, 失败返回-1
 */
int addTask(ThreadPool* pool, Task task);

/**
 * @brief 获取活动线程数量
 * @param pool 线程池指针
 * @return int 返回线程池pool中活动(正在工作)的线程数量
 */
int getActiveThreadNum(ThreadPool* pool);

/**
 * @brief 获取存活的线程数量
 * @param pool 线程池指针
 * @return int 返回线程池pool中存活的线程数量
 */
int getLiveThreadNum(ThreadPool* pool);

/**
 * @brief 销毁线程池
 * @param pool 线程池指针
 * @return int 成功返回0, 失败返回-1
 */
int destoryThreadPool(ThreadPool* pool);

/**
 * @brief 退出当前线程并将线程队列中自己的pid变为0
 * @param pool 线程池指针
 */
void exitThread(ThreadPool* pool);

/**
 * @brief 管理线程函数
 * @param pool 线程池指针
 */
void* threadManager(void* arg);

/**
 * @brief 工作线程函数
 * @param pool 线程池指针
 */
void* work(void* arg);

#endif
