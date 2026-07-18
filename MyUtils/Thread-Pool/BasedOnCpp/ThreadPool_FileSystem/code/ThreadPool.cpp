#include "ThreadPool.h"
#include <iostream>

ThreadPool::ThreadPool(int minThreadNum, int maxThreadNum, int inspFrequency_ms) :
	_minThreadNum(minThreadNum),
	_maxThreadNum(maxThreadNum),
	_inspFrequency_ms(inspFrequency_ms),
	_exitThreadNum(0),
	_stopPool(false) {
	// 工作线程
	for (int i = 0; i < _minThreadNum; ++i) {
		thread th = thread(&ThreadPool::worker, this);
		//cout << "构造函数-添加线程：id = " << th.get_id() << endl;
		_workers_th.insert(make_pair(th.get_id(), move(th)));	// 转移局部对象内存空间的所有权
	}

	_liveThreadNum.store(_minThreadNum);
	_activeThreadNum.store(0);

	// 创建管理者线程
	_manager_th = make_shared<thread>(thread(&ThreadPool::manager, this));
	//cout << "构造函数-添加manager线程：id = " << _manager_th->get_id() << endl;
}

ThreadPool::~ThreadPool() {
	_stopPool = true;
	_taskQCondition.notify_all();

	// 销毁管理者线程
	if (_manager_th != nullptr && _manager_th->joinable()) {
		_manager_th->join();
	}
	// 销毁工作线程，由于管理者不会修改_workers_th，故不加锁
	for (auto& it : _workers_th) {
		thread& t = it.second;
		if (t.joinable()) {
			//cout << "=-=<析构函数>线程：" << it.first << "已经退出\n";
			t.join();
		}
	}
}

void ThreadPool::addTask(function<void(void)> task) {
	{
		if (_stopPool.load()) {	// 线程池关闭则无法插入任务
			return;
		}
		unique_lock<mutex> locker(_taskQMutex);	// 使用互斥锁管理对象管理互斥锁状态(对象析构则自动解锁)
		_taskQueue.push(task);
	}
	_taskQCondition.notify_one();				// 唤醒一个因为任务队列为空而阻塞的线程
}

int ThreadPool::getActiveThreadNum() {
	return _activeThreadNum;
}

void ThreadPool::waitAll() {
	while (true) {
		{
			lock_guard<mutex> taskQ_locker(_taskQMutex);
			if (_taskQueue.empty() &&
				_activeThreadNum.load() == 0 &&
				_liveThreadNum.load() <= _minThreadNum) {
				break;
			}
		}
		this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void ThreadPool::manager() {
	while (!_stopPool.load()) {
		int liveNum_th = _liveThreadNum.load();
		int activeNum_th = _activeThreadNum.load();
		int freeNum_th = liveNum_th - activeNum_th;

		if (freeNum_th > liveNum_th / 2) {	// 如果空闲线程数量大于存活线程数的1/2，则退出1/2的空闲线程
			int exitNum_th = freeNum_th / 2;
			if (liveNum_th - exitNum_th < _minThreadNum) {	// 存活线程数量必须≥最小线程数量
				exitNum_th = std::max(liveNum_th - _minThreadNum, 0);
			}
			_exitThreadNum = exitNum_th;

			// 日志
			if (exitNum_th > 0) {
				std::cout << "aliveNum_th: " << liveNum_th << " | "
					<< "activeNum_th: " << activeNum_th << " | "
					<< "exitNum_th: " << exitNum_th << std::endl;
			}

			while (exitNum_th--) { _taskQCondition.notify_one(); }

			{
				lock_guard<mutex> locker_ids(_exitIDsMutex);
				for (auto id : _exitThreadIDs) {				// 在释放空余线程的同时扫描已经被确定要退出的线程
					auto it = _workers_th.find(id);

					if (it != _workers_th.end() && it->second.joinable()) {
						it->second.join();					// 释放线程资源
						_workers_th.erase(it);
						//cout << "===<manager>线程：" << id << "已经退出\n";
					}
				}
				_exitThreadIDs.clear();
			}
		}
		else if (freeNum_th == 0 && liveNum_th < _maxThreadNum) {	// 空闲线程数量为0则添加一个线程
			thread th = thread(&ThreadPool::worker, this);
			//cout << "+++<manager>添加线程：id = " << th.get_id() << endl;
			lock_guard<mutex> locker_ids(_exitIDsMutex);
			_workers_th.insert(make_pair(th.get_id(), move(th)));
			_liveThreadNum++;
		}
#if DEBUG_LIGHT

		try
		{
			// 打印日志
			std::cout << "_liveThreadNum: " << _liveThreadNum.load() << " | "
				<< "_activeThreadNum: " << _activeThreadNum.load() << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
		catch (...)
		{
			std::cerr << "[INFO] work thread: unknown exception\n";
		}
#endif
		this_thread::sleep_for(chrono::milliseconds(_inspFrequency_ms));
	}
}

void ThreadPool::worker() {
	function<void(void)> task = nullptr;	// 用于接收任务队列中可调用对象的实际地址

	// 线程池没被关闭则处理任务
	while (!_stopPool.load()) {
		{
			unique_lock<mutex> locker(_taskQMutex);
			while (_taskQueue.empty()) {
				_taskQCondition.wait(locker);		// 条件变量的wait方法的解锁/枷锁操作
				if (_stopPool.load()) {	// 退出当前线程
					return;
				}
				if (_exitThreadNum.load() > 0) {	// 如果有线程需要被退出,则自己退出
					_exitThreadNum--;
					_liveThreadNum--;
					//cout << "---<worker>线程：" << this_thread::get_id() << "即将退出\n";
					unique_lock<mutex> locker_ids(_exitIDsMutex);
					_exitThreadIDs.push_back(this_thread::get_id());
					return;
				}
			}
			// 获取任务
			if (!_taskQueue.empty()) {
				task = move(_taskQueue.front());
				_taskQueue.pop();
			}
		}
		if (task != nullptr) {
			_activeThreadNum++;
			//cout << "开始执行任务" << this_thread::get_id() << endl;
			try
			{
				task();
			}
			catch (const std::exception& e)
			{
				std::cerr << "[INFO] " << "work thread: "
					<< std::this_thread::get_id() << e.what() << '\n';
			}
			catch (...) {
				std::cerr << "[INFO] work thread: unknown exception\n";
			}
			task = nullptr;
			_activeThreadNum--;
		}
	}
}
