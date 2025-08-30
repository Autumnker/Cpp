#pragma once

/**
* 1. �������߳�
*	1.0 ����̳߳��Ƿ��Ѿ��ر�
*	1.1 ��Ҫʵʱ��ȡ���������<���������>�Լ�<��Ծ�߳�>��<����߳�>����
*	1.2 ����/�����߳�����
*
* 2. �����߳�	(������)
*	2.0 ����̳߳��Ƿ��Ѿ��ر�
*	2.1 ��Ҫʵʱ��ȡ���������<���������>Ϊ0��ǰ�߳�����
*	2.2 ��Ҫ�����������<��ȡ����>
*	2.3 ȡ����������"�������δ��"�ź�
*
* 3. �������
*	3.1 STL -> Queue (FIFO)
*	3.2 ��Ҫ��һ��"��������"����Ӧ��������Ƿ�Ϊ�� ��<���������>��ӳ��
*
* 4. �������ķ���	(������)
*	4.0 ����̳߳��Ƿ��Ѿ��ر�
*	4.1 ��Ҫ�����������Ƿ�����
*	4.2 ������������������,ֱ��������в�Ϊ��
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

	void addTask(function<void(void)> task);		// ������������������
	int getActiveThreadNum();						// ��ȡ��߳�����

private:
	void manager();									// �����ߺ���
	void worker();									// ��������

private:
	thread* _manager_th;							// �������߳�
	map<thread::id, thread> _workers_th;			// �����߳�����
	vector<thread::id> _exitThreadIDs;				// ���������ٵ��߳�id
	queue<function<void(void)>> _taskQueue;			// �������(�ɵ��ö���)<������Դ>

	mutex _taskQMutex;								// ���������,��������еķ���Ӧ���ǻ����
	mutex _exitIDsMutex;							// �����������ٵ��߳�id������
	condition_variable _taskQCondition;				// ��������,�������Ϊ��ʱ���������߳�

	atomic<int> _minThreadNum;						// ��С�߳�����
	atomic<int> _maxThreadNum;						// ����߳�����
	atomic<int> _activeThreadNum;					// ��Ծ�߳�����
	atomic<int> _liveThreadNum;						// ����߳�����
	atomic<int> _exitThreadNum;						// �˳��߳�����
	atomic<bool> _stopPool;							// �̳߳��Ƿ�ر�
	int _inspFrequency_sec;							// �������̼߳��Ƶ��(��/��)
};
