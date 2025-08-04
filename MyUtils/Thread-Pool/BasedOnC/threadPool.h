/*===============================================
*   文件名称：threadPool.h
*   创 建 者：Autumnker
*   创建日期：2025年08月04日
*   描    述：
================================================*/
#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#define MANAGER_FRQ 3  // 管理线程工作频率 单位：秒/次

// 任务结构体
typedef struct {
    void (*fun)(void* arg);
    void* arg;
} Task;

// 线程池结构体
typedef struct ThreadPool ThreadPool;

// 初始化线程池
ThreadPool* initThreadPool(int taskQSize, int threadMinNum, int threadMaxNum);

// 添加任务
int addTask(Task task, ThreadPool* pool);

// 工作线程相关函数
void* worker(void* arg);

// 管理线程相关函数
void* manager(void* arg);

// 销毁线程池
int destoryThreadPool(ThreadPool* pool);

// 退出工作线程
void exitThread(ThreadPool* pool);

// 获取任务数量
int getTaskNum(ThreadPool* pool);

// 获取活动线程数量
int getActiveNum(ThreadPool* pool);

// 获取存活线程数量
int getLiveNum(ThreadPool* pool);

#endif
