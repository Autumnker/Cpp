#include "memory_pool_fixed_size.hpp"
#include <iostream>
using namespace std;

int main() {
    cout << "start" << endl;
    MemoryPool<int, 10> pool;
    cout << "pool created, capacity=" << pool.capacity() << endl;
    return 0;
}
