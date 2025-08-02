/*===============================================
*   文件名称：threadPool.c
*   创 建 者：Autumnker
*   创建日期：2025年08月02日
*   描    述：
================================================*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "./threadPool.h"

struct ThreadPool {
    // 任务队列相关
    Task* _taskQueue;    // 任务队列头指针
    int _taskQueueSize;  // 任务队列最大容量
    int _taskQFront;     // 任务队列头索引
    int _taskQTail;      // 任务队列尾索引
    int _taskNum;        // 任务队列中的任务个数

    // 线程池相关
    pthread_t _threadManager;  // 管理者线程id
    pthread_t* _threadQueue;   // 线程队列头指针
    int _threadMaxNum;         // 最大线程数量
    int _threadMinNum;         // 最小线程数量
    int _activeThreadNum;      // 活动线程数量
    int _liveThreadNum;        // 存活线程数量
    int _exitThreadNum;        // 要销毁的线程数量

    pthread_mutex_t _mutexPool;       // 线程池锁
    pthread_mutex_t _mutexActiveNum;  // 记录活动线程数量的变量的锁
    pthread_cond_t _notEmpty;         // 线程池非空变量
    pthread_cond_t _notFull;          // 线程池非满变量

    int _shutDown;  // 线程池已经关闭
};

ThreadPool* initThreadPool(int taskQueueSize, int threadMinNum, int threadMaxNum) {
    // 参数校验
    if (taskQueueSize <= 0 || threadMinNum <= 0 || threadMaxNum < threadMinNum) {
        printf("initThreadPool: args error\n");
        return NULL;
    }

    // 开辟线程池空间
    ThreadPool* pool = (ThreadPool*)malloc(sizeof(ThreadPool));
    if (pool == NULL) {
        printf("initThreadPool: malloc pool\n");
        return NULL;
    }

    // 开辟空间并初始化空间/成员
    do {
        // 开辟任务队列空间
        pool->_taskQueue = (Task*)malloc(sizeof(Task) * taskQueueSize);
        if (pool->_taskQueue == NULL) {
            printf("initThreadPool: malloc _taskQueue\n");
            break;
        }
        memset(pool->_taskQueue, 0, sizeof(Task) * taskQueueSize);
        pool->_taskQueueSize = taskQueueSize;
        pool->_taskNum = 0;
        pool->_taskQFront = 0;
        pool->_taskQTail = 0;

        // // 调试
        // printf("任务队列开辟完成, size = %d\n", pool->_taskQueueSize);

        // 开辟线程队列空间
        pool->_threadQueue = (pthread_t*)malloc(sizeof(pthread_t) * threadMaxNum);
        if (pool->_threadQueue == NULL) {
            printf("initThreadPool: malloc _threadQueue\n");
            break;
        }
        memset(pool->_threadQueue, 0, sizeof(pthread_t) * threadMaxNum);
        pool->_threadMaxNum = threadMaxNum;
        pool->_threadMinNum = threadMinNum;
        pool->_activeThreadNum = 0;
        pool->_liveThreadNum = 0;
        pool->_exitThreadNum = 0;

        // // 调试
        // printf("线程队列开辟成功, maxNum = %d, minNum = %d\n", pool->_threadMaxNum, pool->_threadMinNum);

        // 开启管理线程
        if (0 != pthread_create(&pool->_threadManager, NULL, threadManager, (void*)pool)) {
            printf("initThreadPool: pthread_create _threadManager\n");
            break;
        }

        // // 调试
        // printf("管理线程开辟完毕, tid = %ld\n", pool->_threadManager);

        // 创建允许的最小数量线程
        for (int i = 0; i < threadMinNum; ++i) {
            if (0 != pthread_create(&pool->_threadQueue[i], NULL, work, (void*)pool)) {
                printf("initThreadPool: pthread_create _threadQueue, current active thread num: %d\n", i + 1);
                break;
            }
            pool->_liveThreadNum++;

            // // 调试
            // printf("工作线程 %ld 创建完毕\n", pool->_threadQueue[i]);
        }

        // 初始化锁
        if (pthread_mutex_init(&pool->_mutexPool, NULL) != 0 || pthread_mutex_init(&pool->_mutexActiveNum, NULL) != 0 ||
            pthread_cond_init(&pool->_notEmpty, NULL) != 0 || pthread_cond_init(&pool->_notFull, NULL) != 0) {
            printf("initThreadPool: mutex or cond init error\n");
            break;
        }

        // 线程池开启
        pool->_shutDown = 0;

        return pool;

    } while (0);

    // 错误处理
    if (pool->_threadQueue != NULL) { free(pool->_threadQueue); }
    if (pool->_taskQueue != NULL) { free(pool->_taskQueue); }
    if (pool != NULL) { free(pool); }
    return NULL;
}

void* work(void* arg) {
    ThreadPool* pool = (ThreadPool*)arg;
    Task task;

    while (1) {
        pthread_mutex_lock(&pool->_mutexPool);

        // 检查是否需要退出线程
        if (pool->_exitThreadNum > 0 && pool->_liveThreadNum > pool->_threadMinNum) {
            pool->_exitThreadNum--;
            pthread_mutex_unlock(&pool->_mutexPool);
            exitThread(pool);
        }

        // 线程池关闭处理
        if (pool->_shutDown && pool->_taskNum == 0) {
            pthread_mutex_unlock(&pool->_mutexPool);
            exitThread(pool);
        }

        // 检查任务队列是否为空
        while (pool->_taskNum == 0 && !pool->_shutDown) { pthread_cond_wait(&pool->_notEmpty, &pool->_mutexPool); }

        if (pool->_taskNum > 0) {
            // 从任务队列中拿出一个线程并处理
            task = pool->_taskQueue[pool->_taskQFront];
            // 任务队列头后移
            pool->_taskQFront = (pool->_taskQFront + 1) % pool->_taskQueueSize;  // 对头后移
            pool->_taskNum--;                                                    // 任务数量减少

            pthread_cond_signal(&pool->_notFull);  // 发送任务队列未满信号
            pthread_mutex_unlock(&pool->_mutexPool);

            pthread_mutex_lock(&pool->_mutexActiveNum);
            pool->_activeThreadNum++;  // 活跃线程数增加
            pthread_mutex_unlock(&pool->_mutexActiveNum);

            // 处理任务
            task.fun(task.arg);

            // // 调试
            // printf("work: end work %ld ……\n", pthread_self());

            pthread_mutex_lock(&pool->_mutexActiveNum);
            pool->_activeThreadNum--;
            pthread_mutex_unlock(&pool->_mutexActiveNum);
        }
        pthread_mutex_unlock(&pool->_mutexPool);
    }
    exitThread(pool);

    return NULL;
}

void* threadManager(void* arg) {
    ThreadPool* pool = (ThreadPool*)arg;

    while (!pool->_shutDown) {
        // 每隔一段时间检查一次
        sleep(MANAGER_FREQUENCY);

        // 获取活动线程数量、存活线程数量、当前任务数量
        pthread_mutex_lock(&pool->_mutexActiveNum);
        int activateNum = pool->_activeThreadNum;
        pthread_mutex_unlock(&pool->_mutexActiveNum);

        pthread_mutex_lock(&pool->_mutexPool);
        int liveNum = pool->_liveThreadNum;
        int taskNum = pool->_taskNum;
        pthread_mutex_unlock(&pool->_mutexPool);

        // 看看是否需要增加工作线程数量
        if (liveNum < taskNum && liveNum < pool->_threadMaxNum) {
            // 计算空闲线程数量
            int idleNum = liveNum - activateNum;
            // 计算不能分配到线程的任务数量
            int surplusTaskdNum = taskNum - idleNum > 0 ? (taskNum - idleNum) : 0;
            // 计算剩余可开辟线程的数量
            int surplusThreadNum = pool->_threadMaxNum - liveNum;
            // 得出最后需要开辟的线程数量
            int createThreadNum = surplusThreadNum > surplusTaskdNum ? surplusTaskdNum : surplusThreadNum;

            // // 调试
            // printf("需要开辟的新线程数量为: %d\n", createThreadNum);

            pthread_mutex_lock(&pool->_mutexPool);
            for (int i = 0, count = 0; i < pool->_threadMaxNum && count <= createThreadNum; ++i) {
                if (pool->_threadQueue[i] == 0) {
                    if (0 != pthread_create(&pool->_threadQueue[i], NULL, work, pool)) {
                        printf("threadManager: pthread_create error\n");
                        break;
                    }
                    count++;
                    pool->_liveThreadNum++;
                }
            }
            pthread_mutex_unlock(&pool->_mutexPool);
        }

        // 看看是否需要减少工作线程数量
        // 当 任务数量 = 0 且 活动线程数量 = 存活线程数量 / 2 时, 线程的销毁数量 = 存活线程数量 / 2
        if (taskNum == 0 && activateNum < (liveNum / 2) && (liveNum / 2) > pool->_threadMinNum) {
            pthread_mutex_lock(&pool->_mutexPool);
            pool->_exitThreadNum = liveNum / 2 > 0 ? liveNum / 2 : 1;
            pthread_mutex_unlock(&pool->_mutexPool);
        }
    }

    return NULL;
}

int addTask(ThreadPool* pool, Task task) {
    // 参数校验
    if (pool == NULL || task.fun == NULL) {
        printf("addTask: args error\n");
        return -1;
    }

    // 验证任务队列是否已满
    pthread_mutex_lock(&pool->_mutexPool);

    while (pool->_taskNum == pool->_taskQueueSize && !pool->_shutDown) {
        // // 调试
        // printf("addTask: task queue full, wait……\n");
        pthread_cond_wait(&pool->_notFull, &pool->_mutexPool);
    }

    // 验证线程池是否已关闭
    if (pool->_shutDown) {
        // // 调试
        // printf("addTask: pool shut down, return……\n");
        pthread_mutex_unlock(&pool->_mutexPool);
        return 0;
    }

    // 添加任务并移动任务队列尾索引
    pool->_taskQueue[pool->_taskQTail].fun = task.fun;
    pool->_taskQueue[pool->_taskQTail].arg = task.arg;
    pool->_taskQTail = (pool->_taskQTail + 1) % pool->_taskQueueSize;
    pool->_taskNum++;

    // // 调试
    // printf("addTask: 任务添加成功,pool.taskQTail = %d, pool.taskNum = %d, pool.liveThreadNum = %d\n", pool->_taskQTail,
        //    pool->_taskNum, pool->_liveThreadNum);

    // 发送notEmpty信号
    pthread_cond_broadcast(&pool->_notEmpty);
    
    pthread_mutex_unlock(&pool->_mutexPool);

    return 0;
}

void exitThread(ThreadPool* pool) {
    // 获取自己的线程id
    pthread_t id = pthread_self();

    // 修改线程队列
    pthread_mutex_lock(&pool->_mutexPool);

    for (int i = 0; i < pool->_threadMaxNum; ++i) {
        if (pthread_equal(pool->_threadQueue[i], id)) {
            pool->_threadQueue[i] = 0;
            pool->_liveThreadNum--;
            break;
        }
    }

    pthread_mutex_unlock(&pool->_mutexPool);
    // 退出本线程
    printf("----------exitThread: %ld exit……\n", id);
    pthread_exit(NULL);
}

int destoryThreadPool(ThreadPool* pool) {
    if (pool == NULL) {
        printf("getActiveThreadNum: arg error\n");
        return -1;
    }

    // 关闭线程池
    pool->_shutDown = 1;

    // 唤醒所有阻阻塞的消费者线程
    pthread_cond_broadcast(&pool->_notEmpty);

    // 阻塞回收工作线程
    for (int i = 0; i < pool->_threadMaxNum; ++i) {
        if (pool->_threadQueue[i] != 0) { pthread_join(pool->_threadQueue[i], NULL); }
    }

    // 阻塞回收管理者线程
    pthread_join(pool->_threadManager, NULL);

    // 回收堆内存
    free(pool->_taskQueue);
    free(pool->_threadQueue);

    // 销毁互斥锁和条件变量
    pthread_mutex_destroy(&pool->_mutexActiveNum);
    pthread_mutex_destroy(&pool->_mutexPool);
    pthread_cond_destroy(&pool->_notEmpty);
    pthread_cond_destroy(&pool->_notFull);

    free(pool);
    return 0;
}

int getActiveThreadNum(ThreadPool* pool) {
    if (pool == NULL) {
        printf("getActiveThreadNum: arg error\n");
        return -1;
    }
    int activateThreadNum = -1;
    pthread_mutex_lock(&pool->_mutexActiveNum);
    activateThreadNum = pool->_activeThreadNum;
    pthread_mutex_unlock(&pool->_mutexActiveNum);
    return activateThreadNum;
}

int getLiveThreadNum(ThreadPool* pool) {
    if (pool == NULL) {
        printf("getActiveThreadNum: arg error\n");
        return -1;
    }
    int liveThreadNum = -1;
    pthread_mutex_lock(&pool->_mutexPool);
    liveThreadNum = pool->_liveThreadNum;
    pthread_mutex_unlock(&pool->_mutexPool);
    return liveThreadNum;
}
