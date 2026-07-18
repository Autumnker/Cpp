/*===============================================
*   文件名称：thread_pool.h
*   创 建 者：Autumnker
*   创建日期：2026年07月04日
*   描    述：
================================================*/
#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

/**
 * 一、线程池分为三个角色：生产者、消费者、管理者
 *
 * 1.生产者
 *  1.1 对外提供接收任务的接口
 *  1.2 对内将任务放进任务队列中
 *
 * 2.消费者
 *  2.1 从任务队列中取任务
 *  2.2 在自己的线程内执行任务
 *
 * 3.管理者
 *  3.1 定时监控任务队列的任务数量
 *  3.2 监控空闲线程数量
 *  3.3 根据空闲线程数量和任务数量创建、销毁线程
 *
 * 二、数据结构：任务队列queue、线程id管理容器map
 */

#include<map>
#include<queue>
#include<mutex>
#include<vector>
#include<thread>
#include<atomic>
#include<memory>
#include<functional>
#include<condition_variable>

using namespace std;

class Thread_Pool {

public:
    Thread_Pool(int thread_num_min = 2,                 // 最小线程数量默认 = 2
        int thread_num_max = thread::hardware_concurrency() * 2,  // 最大线程数量 = 硬件逻辑并发单元数量 * 2
        int detection_frequency_ms = 10);
    ~Thread_Pool();

    void addTask(function<void(void)> task);        // 添加任务
    void waitAll();                     // 等待所有任务执行完成
    int getActiveThreadNum();                // 获取活跃线程数量
    int getAliveThreadNum();                 // 获取存活线程数量
    int getDetectionFrequency();               // 获取检测频率

private:
    void manager();                     // 管理者线程函数
    void work();                       // 工作线程函数

private:
    queue<function<void(void)>> task_queue;          // 任务队列
    map<thread::id, thread>   thread_map;        // 线程池
    vector<thread::id>      exit_threads;        // 将要退出的线程id集合
    shared_ptr<thread>      manager_ptr;       // 管理者线程指针

    condition_variable      task_condition;       // 任务条件变量
    condition_variable      exit_condition;       // 将要退出的线程准备完毕
    mutex            task_queue_mutex;      // 任务队列锁
    mutex            thread_exit_mutex;      // 将要退出的线程容器锁

    int             thread_num_min;        // 最小线程数量
    int             thread_num_max;        // 最大线程数量
    int             detection_frequency_ms;     // 管理者线程对线程池的检测频率(ms)
    atomic<int>         thread_num_alive;       // 存活线程数量
    atomic<int>         thread_num_active;       // 活跃线程数量
    atomic<int>         thread_num_exit;        // 将要退出的线程数量
    atomic<bool>         stop_thread_pool;       // 是否关闭线程池
};

#endif
