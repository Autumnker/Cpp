#include <iostream>
#include <vector>
using namespace std;

#define NODE_NUM 7
enum NodeIndex { A, B, C, D, E, F, G };

// 采用"无向有环无权重图2"
int global_map[NODE_NUM][NODE_NUM] = {
{0,1,1,0,1,0,0},
{1,0,1,1,1,0,0},
{1,1,0,1,0,0,0},
{0,1,1,0,0,0,1},
{1,1,0,0,0,1,0},
{0,0,0,0,1,0,1},
{0,0,0,1,0,1,0}
};

// 打印vector
template<typename T>
void print_vector(const vector<T>& v) {
    cout << "==================================================" << endl;
    for (auto n : v) {
        cout << n << endl;
    }
    cout << "==================================================" << endl;
}
