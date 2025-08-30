#pragma once

/**
* 1. 管理者线程
*	1.0 检测线程池是否已经关闭
*	1.1 需要实时获取任务队列中<任务的数量>以及<活跃线程>、<存活线程>数量
*	1.2 增加/减少线程数量
*
* 2. 工作线程	(消费者)
*	2.0 检测线程池是否已经关闭
*	2.1 需要实时获取任务队列中<任务的数量>为0则当前线程阻塞
*	2.2 需要从任务队列中<获取任务>
*	2.3 取出任务则发送"任务队列未满"信号
*
* 3. 任务队列
*	3.1 STL -> Queue (FIFO)
*	3.2 需要用一个"条件变量"来反应任务队列是否为空 与<任务的数量>相映射
*
* 4. 添加任务的方法	(生产者)
*	4.0 检测线程池是否已经关闭
*	4.1 需要检测任务队列是否已满
*	4.2 满则阻塞添加任务操作,直到任务队列不为满
*/

#include <thread>
#include <vector>
#include <map>
#include <atomic>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

using namespace std;

class ThreadPool {
public:
	ThreadPool(int minThreadNum = 2, int maxThreadNum = thread::hardware_concurrency() * 2, int inspFrequency_sec = 1);
	~ThreadPool();

	void addTask(function<void(void)> task);		// 向任务队列中添加任务
	int getActiveThreadNum();						// 获取活动线程数量

private:
	void manager();									// 管理者函数
	void worker();									// 工作函数

private:
	thread* _manager_th;							// 管理者线程
	map<thread::id, thread> _workers_th;			// 工作线程容器
	vector<thread::id> _exitThreadIDs;				// 即将被销毁的线程id
	queue<function<void(void)>> _taskQueue;			// 任务队列(可调用对象)<互斥资源>

	mutex _taskQMutex;								// 锁任务队列,对任务队列的访问应该是互斥的
	mutex _exitIDsMutex;							// 锁即将被销毁的线程id的容器
	condition_variable _taskQCondition;				// 条件变量,任务队列为空时阻塞工作线程

	atomic<int> _minThreadNum;						// 最小线程数量
	atomic<int> _maxThreadNum;						// 最大线程数量
	atomic<int> _activeThreadNum;					// 活跃线程数量
	atomic<int> _liveThreadNum;						// 存活线程数量
	atomic<int> _exitThreadNum;						// 退出线程数量
	atomic<bool> _stopPool;							// 线程池是否关闭
	int _inspFrequency_sec;							// 管理者线程检测频率(秒/次)
};
