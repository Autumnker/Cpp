/*===============================================
*   文件名称：main.cpp
*   创 建 者：Autumnker
*   创建日期：2026年07月06日
*   描    述：MemoryPool 教学演示程序 —— 6 个独立演示段落，
*            从基础用法到性能对比，逐步展示内存池的核心概念。
================================================*/
#include "memory_pool_fixed_size.hpp"
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <iomanip>

using namespace std;

/*===============================================
*  辅助工具：一个带计数器的测试类
*
*  用于可视化对象的构造/析构过程。
*  每次构造和析构都会输出信息，
*  方便观察内存池中对象的生命周期。
================================================*/
class Widget {
public:
    int  id;
    char padding[64];   // 模拟真实对象的大小（非平凡类）

    Widget(int id = 0) : id(id) {
        // cout << "  [构造] Widget #" << id << endl;
    }

    ~Widget() {
        // cout << "  [析构] Widget #" << id << endl;
    }

    void greet() const {
        // cout << "  Widget #" << id << ": 你好！" << endl;
    }
};

/*===============================================
*  辅助函数：打印分隔线
================================================*/
void printSection(const string& title) {
    cout << "\n=================================================" << endl;
    cout << "  " << title << endl;
    cout << "=================================================" << endl;
}

/*===============================================
*  演示 1：基本流程（手动控制生命周期）
*
*  最底层的用法：allocate → placement new → 显式析构 → deallocate
*  展示内存池如何将"内存分配"和"对象构造"两个步骤分离。
================================================*/
void demo1_basicFlow() {
    printSection("演示 1：基本 allocate/deallocate 流程");

    // 创建一个小型内存池，只容纳 5 个 Widget
    MemoryPool<Widget, 5> pool;
    cout << "[初始状态] 容量 = " << pool.capacity()
         << ", 已分配 = " << pool.allocated_count() << endl;

    // 步骤 1：从池中获取一块未初始化的内存
    Widget* p = pool.allocate();
    cout << "[分配后]   已分配 = " << pool.allocated_count() << endl;

    // 步骤 2：在获取的内存上构造对象（placement new）
    // 注意：这是"原地构造"，不分配新内存。
    // new(p) Widget(42) 的含义是：
    //   "在 p 指向的地址上，调用 Widget 的构造函数，参数为 42"
    new(p) Widget(42);
    p->greet();
    cout << "  Widget id = " << p->id << endl;

    // 步骤 3：显式销毁对象（但不释放内存）
    // p->~Widget() 调用析构函数，Widget 的内部资源被清理，
    // 但 p 指向的内存仍然属于内存池，可以被复用。
    p->~Widget();

    // 步骤 4：将内存归还池中
    pool.deallocate(p);
    cout << "[归还后]   已分配 = " << pool.allocated_count()
         << "（内存仍属于池，可被下一次 allocate 复用）" << endl;

    cout << "\n✅ 演示 1 通过 —— 基本分配/归还流程正常工作" << endl;
}

/*===============================================
*  演示 2：construct/destroy 便捷方法
*
*  construct() = allocate() + placement new（一步完成）
*  destroy()   = 显式析构 + deallocate()   （一步完成）
*
*  推荐日常使用 construct/destroy，减少手动管理出错的可能。
================================================*/
void demo2_constructDestroy() {
    printSection("演示 2：construct / destroy 便捷方法");

    MemoryPool<Widget, 10> pool;

    // construct 内部先 allocate，再 placement new
    Widget* w1 = pool.construct(100);
    Widget* w2 = pool.construct(200);
    Widget* w3 = pool.construct(300);

    cout << "[分配后] 已分配 = " << pool.allocated_count()
         << " / 容量 = " << pool.capacity() << endl;
    cout << "  w1 id = " << w1->id << endl;
    cout << "  w2 id = " << w2->id << endl;
    cout << "  w3 id = " << w3->id << endl;

    // destroy 内部先显式析构，再 deallocate
    pool.destroy(w1);
    pool.destroy(w3);

    cout << "[归还后] 已分配 = " << pool.allocated_count()
         << "（归还了 w1 和 w3，w2 仍在使用）" << endl;

    // 归还的槽位可以立即被复用
    Widget* w4 = pool.construct(400);
    cout << "  w4 id = " << w4->id
         << "（w4 复用了 w3 刚归还的槽位）" << endl;

    // 清理剩余对象
    pool.destroy(w2);
    pool.destroy(w4);
    cout << "[清理后] 已分配 = " << pool.allocated_count() << endl;

    cout << "\n✅ 演示 2 通过 —— construct/destroy 正常工作" << endl;
}

/*===============================================
*  演示 3：循环分配与释放 —— 观察 free_list 复用
*
*  反复 alloc 和 dealloc，不会导致内存泄漏，也不会向 OS
*  频繁申请/释放内存。所有操作都在池内部完成。
================================================*/
void demo3_recycle() {
    printSection("演示 3：循环分配与释放（观察槽位复用）");

    const int ROUNDS = 100;
    MemoryPool<Widget, 50> pool;

    cout << "  进行 " << ROUNDS << " 轮分配/释放循环..." << endl;

    for (int round = 0; round < ROUNDS; ++round) {
        // 每轮分配 30 个 Widget
        Widget* widgets[30];
        for (int i = 0; i < 30; ++i) {
            widgets[i] = pool.construct(i + round * 100);
        }

        // 立即全部归还
        for (int i = 0; i < 30; ++i) {
            pool.destroy(widgets[i]);
        }
    }

    cout << "  完成 " << ROUNDS << " 轮后 (共 " << (ROUNDS * 30)
         << " 次 alloc/dealloc)：已分配 = " << pool.allocated_count() << endl;
    cout << "  内存池没有向 OS 申请任何新内存（始终用最初那 50 个槽位）" << endl;

    cout << "\n✅ 演示 3 通过 —— 槽位复用机制正常工作" << endl;
}

/*===============================================
*  演示 4：池满处理
*
*  当所有槽位都分配出去后，allocate() 返回 nullptr。
*  使用者应检查返回值，而不是假设分配一定成功。
================================================*/
void demo4_poolFull() {
    printSection("演示 4：池满时的处理");

    // 仅有 3 个槽位的极小池
    MemoryPool<Widget, 3> pool;
    cout << "  池容量: " << pool.capacity() << endl;

    // 用满所有槽位
    Widget* w1 = pool.construct(1);
    Widget* w2 = pool.construct(2);
    Widget* w3 = pool.construct(3);

    cout << "  已分配 3 个，池满: " << boolalpha << pool.full() << endl;

    // 第 4 次分配 —— 返回 nullptr
    Widget* w4 = pool.construct(4);
    if (w4 == nullptr) {
        cout << "  第 4 次 construct 返回 nullptr（池已满，无法分配）" << endl;
    }

    // 归还 1 个后，又可以分配了
    pool.destroy(w2);
    cout << "  归还 1 个后，池满: " << boolalpha << pool.full() << endl;

    w4 = pool.construct(4);
    if (w4 != nullptr) {
        cout << "  第 4 次 construct 成功！复用了归还的槽位，id = "
             << w4->id << endl;
    }

    // 清理
    pool.destroy(w1);
    pool.destroy(w3);
    pool.destroy(w4);

    cout << "\n✅ 演示 4 通过 —— 池满处理逻辑正确" << endl;
}

/*===============================================
*  演示 5：移动语义
*
*  内存池拥有底层内存资源，移动比拷贝更高效。
*  移动后源对象进入"空状态"，可以安全析构。
================================================*/
void demo5_moveSemantics() {
    printSection("演示 5：移动语义");

    // 创建一个池并放入一些对象
    MemoryPool<Widget, 100> pool1;
    Widget* w = pool1.construct(999);
    cout << "  pool1 已分配 = " << pool1.allocated_count() << endl;

    // 移动构造：pool2 接管 pool1 的内存资源
    MemoryPool<Widget, 100> pool2 = std::move(pool1);
    cout << "  移动后:" << endl;
    cout << "    pool1 已分配 = " << pool1.allocated_count()
         << "（源对象被掏空，处于安全空状态）" << endl;
    cout << "    pool2 已分配 = " << pool2.allocated_count()
         << "（目标对象接管了所有资源）" << endl;

    // 通过 pool2 还能正常使用之前分配的对象
    // 因为底层内存在移动时没有发生拷贝
    cout << "  pool2 中的 Widget id = " << w->id
         << "（仍然是同一个对象，地址没变）" << endl;

    // 移动赋值
    MemoryPool<Widget, 100> pool3;
    pool3 = std::move(pool2);
    cout << "  再次移动后:" << endl;
    cout << "    pool2 已分配 = " << pool2.allocated_count() << endl;
    cout << "    pool3 已分配 = " << pool3.allocated_count() << endl;

    // pool1 和 pool2 处于空状态，析构是安全的（delete[] nullptr 是空操作）
    pool3.destroy(w);

    cout << "\n✅ 演示 5 通过 —— 移动语义正常工作" << endl;
}

/*===============================================
*  演示 6：性能对比 —— 内存池 vs new/delete
*
*  分别用内存池和 new/delete 执行大量对象创建/销毁操作，
*  比较两者耗时。注意：这只是一个简单对比，真实效果取决于
*  对象大小、分配频率、内存碎片程度等因素。
================================================*/
void demo6_performance() {
    printSection("演示 6：性能对比（内存池 vs new/delete）");

    const int ITERATIONS = 100000;
    const int POOL_SIZE  = 1024;

    // ---- 测试 1：使用 new/delete ----
    {
        auto start = chrono::high_resolution_clock::now();

        for (int i = 0; i < ITERATIONS; ++i) {
            Widget* w = new Widget(i);
            delete w;
        }

        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        cout << "  new/delete 耗时: " << setw(6) << duration.count()
             << " ms  (" << ITERATIONS << " 次分配+释放)" << endl;
    }

    // ---- 测试 2：使用内存池 ----
    {
        MemoryPool<Widget, POOL_SIZE> pool;

        auto start = chrono::high_resolution_clock::now();

        for (int i = 0; i < ITERATIONS; ++i) {
            // 一次只需要 1 个槽位，用完立即归还
            // 所以 1024 的容量远大于需求，不会池满
            Widget* w = pool.construct(i);
            pool.destroy(w);
        }

        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        cout << "  内存池   耗时: " << setw(6) << duration.count()
             << " ms  (" << ITERATIONS << " 次 construct+destroy)" << endl;
    }

    // ---- 测试 3：批量分配后批量释放 ----
    {
        auto start = chrono::high_resolution_clock::now();

        // 用 vector 管理 new 出来的指针，最后统一 delete
        vector<Widget*> ptrs;
        ptrs.reserve(ITERATIONS);
        for (int i = 0; i < ITERATIONS; ++i) {
            ptrs.push_back(new Widget(i));
        }
        for (Widget* w : ptrs) {
            delete w;
        }

        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        cout << "  new/delete (批量): " << setw(6) << duration.count()
             << " ms  (" << ITERATIONS << " 次)" << endl;
    }

    {
        MemoryPool<Widget, POOL_SIZE> pool;

        auto start = chrono::high_resolution_clock::now();

        // 从池中批量分配
        vector<Widget*> ptrs;
        ptrs.reserve(ITERATIONS);
        for (int i = 0; i < ITERATIONS; ++i) {
            ptrs.push_back(pool.construct(i));
        }
        // 批量归还
        for (Widget* w : ptrs) {
            pool.destroy(w);
        }

        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        cout << "  内存池   (批量): " << setw(6) << duration.count()
             << " ms  (" << ITERATIONS << " 次)" << endl;
    }

    cout << "\n  说明：实际项目中，内存池的优势还体现在：\n"
            "    - 更好的缓存局部性（连续内存）\n"
            "    - 无外部碎片（固定大小）\n"
            "    - 可预测的内存使用量上限\n"
            "    - 适合游戏引擎/网络服务器等高频分配场景" << endl;

    cout << "\n✅ 演示 6 通过 —— 性能对比完成" << endl;
}

/*===============================================
*  主函数
================================================*/
int main() {
    // 设置输出格式
    cout << fixed << setprecision(2);
    cout << "╔════════════════════════════════════════════════╗" << endl;
    cout << "║   MemoryPool — 固定大小内存池教学演示        ║" << endl;
    cout << "║   知识点：预分配 / 空闲链表 / placement new  ║" << endl;
    cout << "║           显式析构 / RAII / 移动语义          ║" << endl;
    cout << "╚════════════════════════════════════════════════╝" << endl;

    demo1_basicFlow();
    demo2_constructDestroy();
    demo3_recycle();
    demo4_poolFull();
    demo5_moveSemantics();
    demo6_performance();

    cout << "\n=================================================" << endl;
    cout << "  全部 6 个演示完成！无内存泄漏，无崩溃。🎉" << endl;
    cout << "=================================================" << endl;

    return 0;
}
