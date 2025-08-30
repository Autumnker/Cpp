#include "ThreadPool.h"
#include <iostream>

ThreadPool::ThreadPool(int minThreadNum, int maxThreadNum, int inspFrequency_sec) : _minThreadNum(minThreadNum), _maxThreadNum(maxThreadNum),
_inspFrequency_sec(inspFrequency_sec), _activeThreadNum(0), _liveThreadNum(0), _stopPool(false) {
	// 创建管理者线程
	_manager_th = new thread(&ThreadPool::manager, this);
	//cout << "构造函数-添加manager线程：id = " << _manager_th->get_id() << endl;

	// 工作线程
	for (int i = 0; i < _minThreadNum; ++i) {
		thread th = thread(&ThreadPool::worker, this);
		//cout << "构造函数-添加线程：id = " << th.get_id() << endl;
		_workers_th.insert(make_pair(th.get_id(), move(th)));	// 转移局部对象内存空间的所有权
	}

	// 初始化线程数量
	_liveThreadNum.store(_minThreadNum);
	_activeThreadNum.store(0);
}

ThreadPool::~ThreadPool() {
	_stopPool = true;
	_taskQCondition.notify_all();
	for (auto& it : _workers_th) {
		thread& t = it.second;
		if (t.joinable()) {
			//cout << "=-=<析构函数>线程：" << it.first << "已经退出\n";
			t.join();
		}
	}
	if (_manager_th->joinable()) {
		//cout << "=*=<析构函数>管理者线程：" << _manager_th->get_id() << "已经退出\n";
		_manager_th->join();
	}
	delete _manager_th;
}

void ThreadPool::addTask(function<void(void)> task) {
	{
		unique_lock<mutex> locker(_taskQMutex);	// 使用互斥锁管理对象管理互斥锁状态(对象析构则自动解锁)
		_taskQueue.push(task);
	}
	_taskQCondition.notify_one();				// 唤醒一个因为任务队列为空而阻塞的线程
}

int ThreadPool::getActiveThreadNum() {
	return _activeThreadNum;
}

void ThreadPool::manager() {
	while (!_stopPool.load()) {
		this_thread::sleep_for(chrono::seconds(_inspFrequency_sec));
		int liveNum_th = _liveThreadNum.load();
		int activeNum_th = _activeThreadNum.load();

		if (liveNum_th - activeNum_th >= liveNum_th / 2) {	// 如果空闲线程数量 >= 总线程数量/2
			_exitThreadNum = liveNum_th / 3;				// 退出 1/3 的线程
			_taskQCondition.notify_all();					// 唤醒所有阻塞在条件变量上的线程(执行退出)

			unique_lock<mutex> locker_ids(_exitIDsMutex);
			for (auto id : _exitThreadIDs) {				// 在释放空余线程的同时扫描已经被确定要退出的线程
				auto it = _workers_th.find(id);

				if (it != _workers_th.end()) {
					(*it).second.join();					// 释放线程资源
					_workers_th.erase(it);
					//cout << "===<manager>线程：" << id << "已经退出\n";
				}
			}
			_exitThreadIDs.clear();
		}
		else if (liveNum_th - activeNum_th == 0 &&
			liveNum_th < _maxThreadNum) {				// 空闲线程数量为0则添加一个线程
			thread th = thread(&ThreadPool::worker, this);
			//cout << "+++<manager>添加线程：id = " << th.get_id() << endl;
			_workers_th.insert(make_pair(th.get_id(), move(th)));
			_liveThreadNum++;
		}
	}
}

void ThreadPool::worker() {
	function<void(void)> task = nullptr;		// 用于接收任务队列中可调用对象的实际地址

	while (!_stopPool.load()) {					// 线程池没被关闭则处理任务
		{
			unique_lock<mutex> locker(_taskQMutex);

			while (_taskQueue.empty() && !_stopPool.load()) {
				_taskQCondition.wait(locker);		// 条件变量的wait方法的解锁/枷锁操作

				if (_exitThreadNum.load() > 0) {	// 如果有线程需要被退出,则自己退出
					_liveThreadNum--;
					_exitThreadNum--;
					//cout << "---<worker>线程：" << this_thread::get_id() << "即将退出\n";

					unique_lock<mutex> locker_ids(_exitIDsMutex);
					_exitThreadIDs.push_back(this_thread::get_id());
					return;
				}
			}
			if (!_taskQueue.empty()) {
				task = move(_taskQueue.front());
				_taskQueue.pop();
			}
		}
		if (task != nullptr) {
			_activeThreadNum++;
			//cout << "开始执行任务" << this_thread::get_id() << endl;
			task();

			// 调试时用
			static mutex _outStream;
			unique_lock<mutex> out_locker(_outStream);
			cout << "_liveThreadNum: " << _liveThreadNum << " | " << "_activeThreadNum: " << _activeThreadNum << endl;
			
			_activeThreadNum--;

		}
	}
}
