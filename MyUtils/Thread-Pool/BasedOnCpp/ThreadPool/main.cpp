/*===============================================
*   文件名称：main.cpp
*   创 建 者：Autumnker
*   创建日期：2026年07月04日
*   描    述：
================================================*/
#include<chrono>
#include<random>
#include<iostream>
#include<string>
#include"thread_pool.h"
#include"utils.h"

bool stop = false;

void count_num(void) {
    int num = 0;
    srand(time(0));
    thread::id id = this_thread::get_id();
    while (num < 10) {
        {
            lock_guard<mutex> locker(mutex_io);
            cout << "[id] " << id << " [num] " << num << endl;
        }
        num++;
        this_thread::sleep_for(chrono::milliseconds(rand() % 500));
    }

    {
        lock_guard<mutex> locker(mutex_io);
        cout << "\n[exit] " << id << endl;
    }
}

#include<signal.h>
void stopHandler(int singal) {
    if (singal == SIGINT) {
        stop = true;
    }
    {
        lock_guard<mutex> locker(mutex_io);
        cout << "\nStopping..." << endl;
    }
}

int main(int argc, char const* argv[])
{
    signal(SIGINT, stopHandler);

    int thread_num = 1;
    if (argc > 1) {
        thread_num = stoi(argv[1]);
    }

    Thread_Pool th_pool;
    {
        lock_guard<mutex> io_locker(mutex_io);
        cout << "Starting..." << endl;
    }
    while (thread_num--) {
        th_pool.addTask(count_num);
    }
    th_pool.waitAll();

    return 0;
}
