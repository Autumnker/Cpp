/*===============================================
*   文件名称：threadPool.c
*   创 建 者：Autumnker
*   创建日期：2025年08月04日
*   描    述：
================================================*/
#include "./threadPool.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct ThreadPool {
    // 任务队列相关
    Task* _taskQueue;
    int _taskQSize;
    int _taskNum;
    int _taskFront;
    int _taskTail;

    // 线程队列相关
    pthread_t _managerThread;
    pthread_t* _threadQueue;
    int _threadMaxNum;
    int _threadMinNum;
    int _liveThread;
    int _activeThread;
    int _exitNum;

    // 线程锁/全局变量
    pthread_mutex_t _mutexPool;
    pthread_mutex_t _mutexActiveThread;
    pthread_cond_t _notEmpty;
    pthread_cond_t _notFull;

    // 线程池状态
    int _isShutDown;
};

ThreadPool* initThreadPool(int taskQSize, int threadMinNum, int threadMaxNum) {
    // 参数校验
    if (taskQSize <= 0 || threadMinNum <= 0 || threadMinNum > threadMaxNum) {
        printf("initThreadPool: args error\n");
        return NULL;
    }

    ThreadPool* pool = (ThreadPool*)malloc(sizeof(ThreadPool));

    // 初始化
    do {
        if (pool == NULL) {
            printf("initThreadPool: malloc ThreadPool error\n");
            break;
        }

        // 开辟任务队列空间
        pool->_taskQueue = (Task*)malloc(sizeof(Task) * taskQSize);
        if (pool->_taskQueue == NULL) {
            printf("initThreadPool: malloc _taskQueue error\n");
            break;
        }
        memset(pool->_taskQueue, 0, sizeof(Task) * taskQSize);
        pool->_taskQSize = taskQSize;
        pool->_taskNum = 0;
        pool->_taskFront = 0;
        pool->_taskTail = 0;

        // 开辟线程队列空间
        pool->_threadQueue = (pthread_t*)malloc(sizeof(pthread_t*) * threadMaxNum);
        if (pool->_threadQueue == NULL) {
            printf("initThreadPool: malloc _threadQueue error\n");
            break;
        }
        pool->_threadMaxNum = threadMaxNum;
        pool->_threadMinNum = threadMinNum;
        pool->_liveThread = 0;
        pool->_activeThread = 0;

        // 开启管理线程
        if (0 != pthread_create(&pool->_managerThread, NULL, manager, (void*)pool)) {
            printf("initThreadPool: pthread_create manager error\n");
            break;
        }

        // 开启最小数量工作线程
        for (int i = 0; i < pool->_threadMinNum; ++i) {
            if (0 != pthread_create(&pool->_threadQueue[i], NULL, worker, (void*)pool)) {
                printf("initThreadPool: pthread_create worker error\n");
                break;
            }
            pool->_liveThread++;
        }

        // 初始化锁和全局变量
        if (pthread_mutex_init(&pool->_mutexPool, NULL) != 0 ||
            pthread_mutex_init(&pool->_mutexActiveThread, NULL) != 0 ||
            pthread_cond_init(&pool->_notEmpty, NULL) != 0 || pthread_cond_init(&pool->_notFull, NULL) != 0) {
            printf("initThreadPool: init mutex or cond error\n");
            break;
        }

        // 开启线程池
        pool->_isShutDown = 0;

        return pool;

    } while (0);

    // 错误处理
    if (pool && pool->_taskQueue) { free(pool->_taskQueue); }
    if (pool && pool->_threadQueue) { free(pool->_threadQueue); }
    if (pool) { free(pool); }
    return NULL;
}

int addTask(Task task, ThreadPool* pool) {
    // 参数校验
    if (task.fun == NULL || pool == NULL) {
        printf("addTask: args error\n");
        return -1;
    }

    pthread_mutex_lock(&pool->_mutexPool);

    // 线程池是否关闭
    if (pool->_isShutDown) {
        printf("addTask: thread pool is shut down\n");
        pthread_mutex_unlock(&pool->_mutexPool);
        return -1;
    }

    // 任务队列是否已满
    while (pool->_taskNum == pool->_taskQSize && !pool->_isShutDown) {
        printf("addTask: task queue full, please wait……\n");
        pthread_cond_wait(&pool->_notFull, &pool->_mutexPool);
    }

    // 添加任务
    pool->_taskQueue[pool->_taskTail].fun = task.fun;
    pool->_taskQueue[pool->_taskTail].arg = task.arg;

    // 任务数量增加，队尾后移
    pool->_taskTail = (pool->_taskTail + 1) % pool->_taskQSize;
    pool->_taskNum++;

    // 发送notEmpty信号
    pthread_cond_signal(&pool->_notEmpty);

    pthread_mutex_unlock(&pool->_mutexPool);

    return 0;
}

void* worker(void* arg) {
    // 参数校验
    if (arg == NULL) {
        printf("worker: arg is NULL\n");
        return NULL;
    }
    ThreadPool* pool = (ThreadPool*)arg;
    Task task;

    while (1) {
        pthread_mutex_lock(&pool->_mutexPool);

        // 任务队列是否为空(阻塞)
        while (pool->_taskNum == 0) {
            // 线程池是否关闭
            if (pool->_isShutDown) {
                pthread_mutex_unlock(&pool->_mutexPool);
                exitThread(pool);
            }
            pthread_cond_wait(&pool->_notEmpty, &pool->_mutexPool);
        }

        // 是否有线程要退出
        if (pool->_exitNum > 0) {
            pool->_exitNum--;
            pthread_mutex_unlock(&pool->_mutexPool);
            exitThread(pool);
        }

        // 线程池是否关闭
        if (pool->_isShutDown) {
            pthread_mutex_unlock(&pool->_mutexPool);
            exitThread(pool);
        }

        // 取一个任务
        task.arg = pool->_taskQueue[pool->_taskFront].arg;
        task.fun = pool->_taskQueue[pool->_taskFront].fun;

        // 队头后移，任务数量减少
        pool->_taskFront = (pool->_taskFront + 1) % pool->_taskQSize;
        pool->_taskNum--;

        printf("======工作线程刚刚取到任务 current taskNum: %d, liveNum: %d, activateNum: %d\n", pool->_taskNum, pool->_liveThread,
               pool->_activeThread);

        // 发送notFull信号
        pthread_cond_signal(&pool->_notFull);
        pthread_mutex_unlock(&pool->_mutexPool);

        // 活跃线程数量+1
        pthread_mutex_lock(&pool->_mutexActiveThread);
        pool->_activeThread++;
        pthread_mutex_unlock(&pool->_mutexActiveThread);

        // 执行任务
        task.fun(task.arg);

        // 活跃线程数量-1
        pthread_mutex_lock(&pool->_mutexActiveThread);
        pool->_activeThread--;
        pthread_mutex_unlock(&pool->_mutexActiveThread);
    }
    return NULL;
}

void* manager(void* arg) {
    // 参数校验
    if (arg == NULL) {
        printf("manager: arg is NULL\n");
        return NULL;
    }
    ThreadPool* pool = (ThreadPool*)arg;

    while (1) {
        sleep(MANAGER_FRQ);

        pthread_mutex_lock(&pool->_mutexPool);
        // 线程池是否关闭
        if (pool->_isShutDown) {
            pthread_mutex_unlock(&pool->_mutexPool);
            printf("manager: manager pthread exit……\n");
            pthread_exit(NULL);
        }
        pthread_mutex_unlock(&pool->_mutexPool);

        // 获取线程池参数
        pthread_mutex_lock(&pool->_mutexPool);
        int taskNum = pool->_taskNum;
        int liveNum = pool->_liveThread;
        pthread_mutex_unlock(&pool->_mutexPool);

        pthread_mutex_lock(&pool->_mutexActiveThread);
        int activateNum = pool->_activeThread;
        pthread_mutex_unlock(&pool->_mutexActiveThread);

        pthread_mutex_lock(&pool->_mutexPool);
        // 是否需要添加线程
        if (taskNum / 2 > (liveNum - activateNum) && liveNum < pool->_threadMaxNum) {
            int addNum = pool->_threadMaxNum - liveNum > taskNum ? taskNum : pool->_threadMaxNum - liveNum;

            printf("+++++增加线程数量 taskNum: %d, liveThreadNum: %d, activeThreadNum: %d, addNum: %d\n", taskNum, liveNum,
                   activateNum, addNum);
            sleep(3);

            for (int i = 0, count = 0; i < pool->_threadMaxNum && count < addNum; ++i) {
                if (pool->_threadQueue[i] == 0) {
                    if (0 != pthread_create(&pool->_threadQueue[i], NULL, worker, pool)) {
                        printf("manager: pthread_create error\n");
                        break;
                    }
                    pool->_liveThread++;
                    count++;
                }
            }
        }
        // 是否需要销毁线程
        else if (pool->_exitNum == 0 && pool->_taskNum * 2 < (liveNum - activateNum) && liveNum > pool->_threadMinNum) {
            int subNum = subNum == 1 ? 1 : (liveNum - activateNum) / 2;
            subNum = liveNum - subNum > pool->_threadMinNum ? subNum : liveNum - pool->_threadMinNum;
            pool->_exitNum = subNum;

            // pthread_cond_broadcast(&pool->_notEmpty);
            printf("-----减少线程数量 taskNum: %d, liveThreadNum: %d, activeThreadNum: %d, subNum: %d\n", taskNum, liveNum,
                   activateNum, subNum);
            sleep(3);
        }

        pthread_mutex_unlock(&pool->_mutexPool);
    }
    return NULL;
}

int destoryThreadPool(ThreadPool* pool) {
    // 参数校验
    if (pool == NULL) {
        printf("destoryThreadPool: arg is NULL\n");
        return -1;
    }

    // 关闭线程池
    pool->_isShutDown = 1;

    // 唤醒所有阻塞的线程
    pthread_cond_broadcast(&pool->_notEmpty);

    // 等待所有工作线程退出
    for (int i = 0; i < pool->_threadMaxNum; ++i) {
        if (pool->_threadQueue[i] != 0) {
            pthread_join(pool->_threadQueue[i], NULL);
            // printf("*****回收到工作线程 %ld \n", pool->_threadQueue[i]);
        }
    }

    // 等待管理线程退出
    pthread_join(pool->_managerThread, NULL);
    // printf("*****回收到管理线程资源\n");

    // 销毁互斥锁、全局变量
    pthread_mutex_destroy(&pool->_mutexPool);
    pthread_mutex_destroy(&pool->_mutexActiveThread);
    pthread_cond_destroy(&pool->_notEmpty);
    pthread_cond_destroy(&pool->_notFull);

    // 释放堆内存
    free(pool->_taskQueue);
    free(pool->_threadQueue);
    free(pool);

    return 0;
}

void exitThread(ThreadPool* pool) {
    pthread_t currId = pthread_self();
    pthread_mutex_lock(&pool->_mutexPool);

    printf(".....线程正在退出 current taskNum: %d, liveNum: %d, activateNum: %d, exitNum: %d\n", pool->_taskNum, pool->_liveThread,
           pool->_activeThread, pool->_exitNum);

    for (int i = 0; i < pool->_threadMaxNum; ++i) {
        if (pthread_equal(currId, pool->_threadQueue[i])) {
            printf("exitThread: thread %ld is exiting…… ", currId);
            pool->_threadQueue[i] = 0;
            pool->_liveThread--;
            break;
        }
    }
    pthread_mutex_unlock(&pool->_mutexPool);
    printf("exit completed\n");
    pthread_exit(NULL);
}

int getTaskNum(ThreadPool* pool) {
    pthread_mutex_lock(&pool->_mutexPool);
    int taskNum = pool->_taskNum;
    pthread_mutex_unlock(&pool->_mutexPool);
    return taskNum;
}

int getActiveNum(ThreadPool* pool) {
    pthread_mutex_lock(&pool->_mutexPool);
    int activateNum = pool->_activeThread;
    pthread_mutex_unlock(&pool->_mutexPool);
    return activateNum;
}

int getLiveNum(ThreadPool* pool) {
    pthread_mutex_lock(&pool->_mutexPool);
    int liveNum = pool->_liveThread;
    pthread_mutex_unlock(&pool->_mutexPool);
    return liveNum;
}
