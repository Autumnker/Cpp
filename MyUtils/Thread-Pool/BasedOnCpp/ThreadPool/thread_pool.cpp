/*===============================================
*   文件名称：thread_pool.cpp
*   创 建 者：Autumnker
*   创建日期：2026年07月04日
*   描    述：
================================================*/
#include<iostream>
#include"thread_pool.h"
#include"utils.h"

using namespace std;

/**
 * 创建管理者线程
 * 根据最小线程数量，创建对应数量的线程
 */
Thread_Pool::Thread_Pool(int thread_num_min, int thread_num_max, int detection_frequency_ms)
    :thread_num_min(thread_num_min),
    thread_num_max(thread_num_max),
    detection_frequency_ms(detection_frequency_ms),
    thread_num_exit(0),
    stop_thread_pool(false) {

    // 创建最小空闲线程
    for (int i = 0; i < thread_num_min; i++) {
        thread th = move(thread(&Thread_Pool::work, this));
        thread_map[th.get_id()] = move(th);
    }
    // 更新线程状态变量
    thread_num_alive.store(thread_num_min);
    thread_num_active.store(0);

    // 创建管理者线程
    manager_ptr = make_shared<thread>(thread(&Thread_Pool::manager, this));
}

/**
 * 关闭线程池
 * 唤醒所有被阻塞的线程
 * 退出管理者线程
 * 退出所有工作线程
 */
Thread_Pool::~Thread_Pool() {
    // 关闭线程池
    stop_thread_pool = true;
    // 唤醒所有被阻塞的线程
    task_condition.notify_all();
    // 退出管理者线程
    if (manager_ptr && manager_ptr->joinable()) {
        manager_ptr->join();
    }
    // 退出所有存活线程
    for (auto& [_, th] : thread_map) {
        if (th.joinable()) {
            th.join();
        }
    }
    thread_map.clear();
#if DEBUG_LOG
    lock_guard<mutex> io_locker(mutex_io);
    cout << "Exiting..." << endl;
#endif
}

/**
 * 锁住任务队列
 * 添加任务
 * 唤醒阻塞线程
 */
void Thread_Pool::addTask(function<void(void)> task) {
    {
        lock_guard<mutex> lock(task_queue_mutex);
        task_queue.push(move(task));
    }
    task_condition.notify_one();
}

/**
 * 当任务队列为空且活跃线程数为0且存活线程数为最小值时退出
 */
void Thread_Pool::waitAll() {
    while (true) {
        {
            lock_guard<mutex> task_queue_locker(task_queue_mutex);
            if (task_queue.empty() &&
                thread_num_active == 0 &&
                thread_num_alive == thread_num_min) {
                break;
            }
        }
        this_thread::sleep_for(chrono::milliseconds(10));
    }
#if DEBUG_LOG
    lock_guard<mutex> io_locker(mutex_io);
    cout << "Waiting Done" << endl;
#endif
}

int Thread_Pool::getActiveThreadNum() {
    return thread_num_active;
}

int Thread_Pool::getAliveThreadNum() {
    return thread_num_alive;
}

int Thread_Pool::getDetectionFrequency() {
    return detection_frequency_ms;
}

/**
 * 启动内部定时器
 * detection_frequency_ms后开始检测
 * 获取任务队列中的任务数量
 * 根据任务数、存活线程数、活跃线程数计算将要退出的线程数量
 * 如果将要退出的线程数量为正，唤醒所有被阻塞的线程，让它们自己进入待销毁容器
 * 在manager线程执行销毁操作并更新存活线程数量
 * 如果将要退出的线程数量为负，则创建新线程并更新存活线程数量
 * 清空exit_threads等待下一轮计时
 */
void Thread_Pool::manager() {
    while (!stop_thread_pool) {
#if DEBUG_LOG
        {
            lock_guard<mutex> io_locker(mutex_io);
            cout << "--------------------------------------------------" << endl;
        }
#endif
        int num_task = 0;
        int num_alive = 0;
        int num_active = 0;
        int num_exit = 0;
        // 获取任务数量
        {
            lock_guard<mutex> task_queue_locker(task_queue_mutex);
            num_task = task_queue.size();
#if DEBUG_LOG
            lock_guard<mutex> io_locker(mutex_io);
            cout << "num_task = " << num_task << endl;
#endif
        }
        // 获取存活线程数量和活动线程数量
        {
            lock_guard<mutex> thread_exit_locker(thread_exit_mutex);
            num_alive = thread_num_alive;
            num_active = thread_num_active;
#if DEBUG_LOG
            lock_guard<mutex> io_locker(mutex_io);
            cout << "num_alive | num_active = " << num_alive << " | " << num_active << endl;
#endif
        }
        // 计算需要退出的任务数量
        num_exit = (num_alive - num_active - num_task) / 2;
        if (num_exit > 0 && num_exit > num_alive - thread_num_min) {
            num_exit = num_alive - thread_num_min;
        }
        else if (num_exit < 0 && -num_exit > thread_num_max - num_alive) {
            num_exit = -(thread_num_max - num_alive);
        }
        // 得到需要退出的线程数量
        {
            lock_guard<mutex> thread_exit_locker(thread_exit_mutex);
            thread_num_exit = num_exit;
#if DEBUG_LOG
            lock_guard<mutex> io_locker(mutex_io);
            cout << "thread_num_exit = " << thread_num_exit << endl;
#endif
        }
#if DEBUG_LOG
        {
            lock_guard<mutex> io_locker(mutex_io);
            cout << "[thread ids begin before change]" << endl;
            int count = 0;
            for (auto& [id, _] : thread_map) {
                cout << id << endl;
                count++;
            }
            cout << "[thread ids end before change count = " << count << "]" << endl;
        }
#endif
        // 判断增加或是销毁线程
        if (num_exit > 0) { // 销毁线程
            // 唤醒所有线程
            task_condition.notify_all();
            {
                unique_lock<mutex> thread_exit_locker(thread_exit_mutex);
                // 等所有待退出线程准备好才能开始销毁，但最多等待一次检测间隔的时间
                exit_condition.wait_for(thread_exit_locker, chrono::milliseconds(detection_frequency_ms));
                for (thread::id& id : exit_threads) {
                    auto it = thread_map.find(id);
                    if (it != thread_map.end() && (*it).second.joinable()) {
                        (*it).second.join();
                        thread_map.erase(id);
                        // 更新存活线程数量
                        thread_num_alive--;
                    }
                }
                exit_threads.clear();
            }
        }
        else if (num_exit < 0) { // 增加线程
            int add_num = -num_exit;
            lock_guard<mutex> thread_exit_locker(thread_exit_mutex);
            while (add_num--) {
                thread th = move(thread(&Thread_Pool::work, this));
                thread_map[th.get_id()] = move(th);
                // 更新存活线程数量
                thread_num_alive++;
            }
        }
#if DEBUG_LOG
        {
            lock_guard<mutex> io_locker(mutex_io);
            cout << "[thread ids begin after change]" << endl;
            int count = 0;
            for (auto& [id, _] : thread_map) {
                cout << id << endl;
                count++;
            }
            cout << "[thread ids end after change count = " << count << "]" << endl;

            cout << "--------------------------------------------------" << endl;
        }
#endif
        // 延时
        this_thread::sleep_for(chrono::milliseconds(detection_frequency_ms));
    }
}

/**
 * 如果任务队列为空，则阻塞
 * 被唤醒时，如果退出线程数量大于0，则将自己的线程id放进exit_threads中，更新将要退出的线程数量，并退出循环
 * 被唤醒时，如果退出线程数量等于0，则从任务队列中取任务，更新活跃线程数量，执行任务，任务执行完毕后更新活跃线程数量
 * 如果任务队列不为空，则从任务队列中取任务，更新活跃线程数量，执行任务，任务执行完毕后更新活跃线程数量
 */
void Thread_Pool::work() {
    function<void(void)> task = nullptr;
    while (!stop_thread_pool) {
        unique_lock<mutex> task_queue_locker(task_queue_mutex);
        if (task_queue.empty()) {  // 任务队列为空
            task_condition.wait(task_queue_locker);
            if (!stop_thread_pool) {    // 可能是关闭线程池的通知
                lock_guard<mutex> thraed_exit_locker(thread_exit_mutex);
                if (thread_num_exit > 0) {
                    exit_threads.emplace_back(this_thread::get_id());
                    thread_num_exit--;
                    if (thread_num_exit == 0) {
                        exit_condition.notify_all();
                    }
                    return;
                }
            }
            else {
                exit_condition.notify_all();
                return;
            }
            // 如果不需要退出，则执行任务
            if (task_queue.empty()) {
                continue;
            }
            // 获取任务
            task = task_queue.front();
            task_queue.pop();
        }
        else {  // 任务队列不为空
            // 获取任务
            task = task_queue.front();
            task_queue.pop();
        }
        task_queue_locker.unlock();

        // 执行任务
        thread_num_active++;
        try {
            task();
        }
        catch (exception& e) {
#if DEBUG_LOG
            lock_guard<mutex> io_locker(mutex_io);
#endif
            cout << e.what() << endl;
        }
        thread_num_active--;
        task = nullptr;
    }
}
