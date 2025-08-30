#include "ThreadPool.h"
#include <iostream>

ThreadPool::ThreadPool(int minThreadNum, int maxThreadNum, int inspFrequency_sec) : _minThreadNum(minThreadNum), _maxThreadNum(maxThreadNum),
_inspFrequency_sec(inspFrequency_sec), _activeThreadNum(0), _liveThreadNum(0), _stopPool(false) {
	// �����������߳�
	_manager_th = new thread(&ThreadPool::manager, this);
	//cout << "���캯��-���manager�̣߳�id = " << _manager_th->get_id() << endl;

	// �����߳�
	for (int i = 0; i < _minThreadNum; ++i) {
		thread th = thread(&ThreadPool::worker, this);
		//cout << "���캯��-����̣߳�id = " << th.get_id() << endl;
		_workers_th.insert(make_pair(th.get_id(), move(th)));	// ת�ƾֲ������ڴ�ռ������Ȩ
	}

	// ��ʼ���߳�����
	_liveThreadNum.store(_minThreadNum);
	_activeThreadNum.store(0);
}

ThreadPool::~ThreadPool() {
	_stopPool = true;
	_taskQCondition.notify_all();
	for (auto& it : _workers_th) {
		thread& t = it.second;
		if (t.joinable()) {
			//cout << "=-=<��������>�̣߳�" << it.first << "�Ѿ��˳�\n";
			t.join();
		}
	}
	if (_manager_th->joinable()) {
		//cout << "=*=<��������>�������̣߳�" << _manager_th->get_id() << "�Ѿ��˳�\n";
		_manager_th->join();
	}
	delete _manager_th;
}

void ThreadPool::addTask(function<void(void)> task) {
	{
		unique_lock<mutex> locker(_taskQMutex);	// ʹ�û�������������������״̬(�����������Զ�����)
		_taskQueue.push(task);
	}
	_taskQCondition.notify_one();				// ����һ����Ϊ�������Ϊ�ն��������߳�
}

int ThreadPool::getActiveThreadNum() {
	return _activeThreadNum;
}

void ThreadPool::manager() {
	while (!_stopPool.load()) {
		this_thread::sleep_for(chrono::seconds(_inspFrequency_sec));
		int liveNum_th = _liveThreadNum.load();
		int activeNum_th = _activeThreadNum.load();

		if (liveNum_th - activeNum_th >= liveNum_th / 2) {	// ��������߳����� >= ���߳�����/2
			_exitThreadNum = liveNum_th / 3;				// �˳� 1/3 ���߳�
			_taskQCondition.notify_all();					// �����������������������ϵ��߳�(ִ���˳�)

			unique_lock<mutex> locker_ids(_exitIDsMutex);
			for (auto id : _exitThreadIDs) {				// ���ͷſ����̵߳�ͬʱɨ���Ѿ���ȷ��Ҫ�˳����߳�
				auto it = _workers_th.find(id);

				if (it != _workers_th.end()) {
					(*it).second.join();					// �ͷ��߳���Դ
					_workers_th.erase(it);
					//cout << "===<manager>�̣߳�" << id << "�Ѿ��˳�\n";
				}
			}
			_exitThreadIDs.clear();
		}
		else if (liveNum_th - activeNum_th == 0 &&
			liveNum_th < _maxThreadNum) {				// �����߳�����Ϊ0�����һ���߳�
			thread th = thread(&ThreadPool::worker, this);
			//cout << "+++<manager>����̣߳�id = " << th.get_id() << endl;
			_workers_th.insert(make_pair(th.get_id(), move(th)));
			_liveThreadNum++;
		}
	}
}

void ThreadPool::worker() {
	function<void(void)> task = nullptr;		// ���ڽ�����������пɵ��ö����ʵ�ʵ�ַ

	while (!_stopPool.load()) {					// �̳߳�û���ر���������
		{
			unique_lock<mutex> locker(_taskQMutex);

			while (_taskQueue.empty() && !_stopPool.load()) {
				_taskQCondition.wait(locker);		// ����������wait�����Ľ���/��������

				if (_exitThreadNum.load() > 0) {	// ������߳���Ҫ���˳�,���Լ��˳�
					_liveThreadNum--;
					_exitThreadNum--;
					//cout << "---<worker>�̣߳�" << this_thread::get_id() << "�����˳�\n";

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
			//cout << "��ʼִ������" << this_thread::get_id() << endl;
			task();

			// ����ʱ��
			static mutex _outStream;
			unique_lock<mutex> out_locker(_outStream);
			cout << "_liveThreadNum: " << _liveThreadNum << " | " << "_activeThreadNum: " << _activeThreadNum << endl;
			
			_activeThreadNum--;

		}
	}
}
