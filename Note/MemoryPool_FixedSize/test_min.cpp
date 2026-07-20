#include "memory_pool_fixed_size.hpp"
#include <iostream>
using namespace std;

int main() {
    cout << "Step 1: creating pool..." << endl;
    MemoryPool<int, 10> pool;
    cout << "Step 2: pool created, capacity = " << pool.capacity() << endl;
    cout << "Step 3: done" << endl;
    return 0;
}
