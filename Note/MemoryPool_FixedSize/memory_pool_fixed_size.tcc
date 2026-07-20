/*===============================================
*   文件名称：memory_pool_fixed_size.tcc
*   创 建 者：Autumnker
*   创建日期：2026年07月06日
*   描    述：MemoryPool 类模板的实现代码
*
*   【关于 .tcc 后缀】
*   .tcc = Template Class Code，一种常见的文件组织约定，
*   用于存放 C++ 模板的成员函数实现。与 .cpp 不同，
*   .tcc 不作为独立的编译单元，而是由对应的 .hpp 包含。
*
*   【包含顺序】
*   memory_pool_fixed_size.hpp 末尾有：
*       #include "memory_pool_fixed_size.tcc"
*   因此本文件被包含时，MemoryPool 的类声明已经完整可见，
*   所有必要的头文件和 using namespace std 也已在 .hpp 中引入。
================================================*/

// ============================================================
//  构造函数：预分配 BlockCount 个 Slot，全部串联到空闲链表
//
//  时间复杂度：O(BlockCount)
//  说明：new[] 只调用 Slot 的默认构造（空操作），
//        不会构造任何 T 对象。
// ============================================================
template <typename T, size_t BlockCount>
MemoryPool<T, BlockCount>::MemoryPool()
    : allocated_(0)
{
    // 一次性向操作系统申请整块内存
    pool_ = new Slot[BlockCount];

    // 将所有 Slot 串联成单向链表（空闲链表）
    // 初始状态：全部空闲
    //  free_list_ → [Slot 0] → [Slot 1] → ... → [Slot N-1] → nullptr
    free_list_ = &pool_[0];
    for (size_t i = 0; i < BlockCount - 1; ++i) {
        pool_[i].next = &pool_[i + 1];
    }
    pool_[BlockCount - 1].next = nullptr;
}

// ============================================================
//  析构函数：归还整块内存
//
//  如果 allocated_ > 0，说明存在未归还的对象，打印警告。
//  注意：不会对未归还的 Slot 调用 T 的析构函数，
//  因为 union 无法追踪哪些 Slot 中的 T 是已构造的。
// ============================================================
template <typename T, size_t BlockCount>
MemoryPool<T, BlockCount>::~MemoryPool() {
    if (allocated_ > 0) {
        // 生产环境中可以打印警告后仍然释放内存
        // 但作为教学目的，这里输出信息提示问题
        cerr << "[MemoryPool] 警告：析构时仍有 "
             << allocated_ << " 个对象未释放！" << endl;
    }
    delete[] pool_;
    pool_       = nullptr;
    free_list_  = nullptr;
    allocated_  = 0;
}

// ============================================================
//  移动构造：转移整块内存的所有权
//
//  源对象被"掏空"后进入安全可析构状态（所有指针置 nullptr）。
//  这比拷贝整个池高效得多 —— O(1) 而非 O(BlockCount)。
// ============================================================
template <typename T, size_t BlockCount>
MemoryPool<T, BlockCount>::MemoryPool(MemoryPool&& other) noexcept
    : pool_(other.pool_)
    , free_list_(other.free_list_)
    , allocated_(other.allocated_)
{
    // 将源对象置为安全状态
    other.pool_       = nullptr;
    other.free_list_  = nullptr;
    other.allocated_  = 0;
}

// ============================================================
//  移动赋值：先释放自己的资源，再接管对方的
// ============================================================
template <typename T, size_t BlockCount>
MemoryPool<T, BlockCount>&
MemoryPool<T, BlockCount>::operator=(MemoryPool&& other) noexcept {
    if (this != &other) {
        // 先清理自己的旧资源
        delete[] pool_;

        // 接管对方资源
        pool_       = other.pool_;
        free_list_  = other.free_list_;
        allocated_  = other.allocated_;

        // 掏空对方
        other.pool_       = nullptr;
        other.free_list_  = nullptr;
        other.allocated_  = 0;
    }
    return *this;
}

// ============================================================
//  allocate()：从空闲链表头部取一个 Slot，返回指向其 data 的指针
//
//  时间复杂度：O(1)
//  返回值：指向未初始化内存的 T*（需要 placement new 构造对象）
//         池满时返回 nullptr（不会抛异常）
//
//  【为什么返回 T* 而不是 Slot*】
//  对外隐藏内部数据结构。使用者只需要知道"这是一块可以放 T 的内存"。
//  因为 Slot 是 union，data 在偏移 0 处，&slot->data == slot，
//  后续可通过 reinterpret_cast 从 T* 回推出 Slot*。
// ============================================================
template <typename T, size_t BlockCount>
T* MemoryPool<T, BlockCount>::allocate() {
    if (free_list_ == nullptr) {
        // 池已满，没有空闲槽位
        return nullptr;
    }

    // 从空闲链表头部取出一个 Slot
    Slot* slot = free_list_;
    free_list_ = slot->next;   // 头指针后移

    ++allocated_;

    // 返回 Slot 中 data 成员的地址
    // Slot 是 union，data 在偏移 0 处，因此 &slot->data == slot
    return &slot->data;
}

// ============================================================
//  deallocate(T* p)：将 p 指向的 Slot 归还到空闲链表头部
//
//  时间复杂度：O(1)
//
//  【关键：如何从 T* 找回 Slot*】
//  allocate() 返回的是 &slot->data（即 Slot 中 data 成员的地址）。
//  union 的所有成员共享起始地址，因此 data 的地址 == Slot 的地址。
//  通过 reinterpret_cast<Slot*>(p) 即可从 T* 回推出 Slot*。
//
//  【为什么不调用 T 的析构函数】
//  内存管理和对象生命周期是两个独立关注点。deallocate 只归还内存。
//  如果需要销毁对象，请先用 destroy() 或手动调用 p->~T()。
// ============================================================
template <typename T, size_t BlockCount>
void MemoryPool<T, BlockCount>::deallocate(T* p) {
    if (p == nullptr) {
        return;   // 防御性检查
    }

    // 指针回推：T* → Slot*
    // 因为 T 是 Slot 联合体的第一个成员（偏移 0），
    // reinterpret_cast 在此场景下是安全的
    Slot* slot = reinterpret_cast<Slot*>(p);

    // 将 slot 插入空闲链表头部（头插法，O(1)）
    slot->next = free_list_;
    free_list_ = slot;

    --allocated_;
}

// ============================================================
//  construct(Args&&... args)：分配内存 + 原地构造对象
//
//  等价于：T* p = allocate(); new(p) T(std::forward<Args>(args)...);
//  一步完成分配和构造，减少出错可能。
//
//  【什么是 placement new】
//  new(address) T(args...) 并不分配新内存 —— 它只在 address
//  指定的地址上调用 T 的构造函数。内存在此之前已由 allocate() 准备好。
//
//  【std::forward 的作用】
//  完美转发参数：左值保持左值，右值保持右值，避免多余的拷贝。
//
//  【异常安全】
// 如果 T 的构造函数抛异常，归还 Slot 并重新抛出，保证不泄漏。
// ============================================================
template <typename T, size_t BlockCount>
template <typename... Args>
T* MemoryPool<T, BlockCount>::construct(Args&&... args) {
    T* p = allocate();
    if (p == nullptr) {
        return nullptr;   // 池满，无法分配
    }

    // placement new：在 p 指向的内存上构造 T 对象
    try {
        new(p) T(std::forward<Args>(args)...);
    }
    catch (...) {
        // 构造失败，归还 Slot 并重新抛出异常
        deallocate(p);
        throw;
    }

    return p;
}

// ============================================================
//  destroy(T* p)：显式调用析构函数 + 归还内存
//
//  【为什么要显式调用析构函数】
//  使用 placement new 构造的对象必须显式调用其析构函数。
//  普通 delete p 会：1) 调用析构 2) 释放 p 指向的内存。
//  但在内存池中，第 2 步要改为"归还槽位"而非"释放给 OS"。
//
//  所以我们拆分两个操作：
//    p->~T();         // 销毁对象（调用析构函数）
//    deallocate(p);   // 归还槽位到空闲链表（不释放给 OS）
// ============================================================
template <typename T, size_t BlockCount>
void MemoryPool<T, BlockCount>::destroy(T* p) {
    if (p == nullptr) {
        return;
    }
    // 显式调用析构函数 —— 销毁对象但不释放内存
    p->~T();
    // 将槽位归还空闲链表
    deallocate(p);
}


/*===============================================
*  编译期约束检查
*
*  static_assert 在编译期就能捕获错误用法，
*  避免到运行期才发现问题。错误信息在编译时直接显示。
================================================*/

// 检查 T 是否是可析构的（必须有析构函数）
// 因为 destroy() 需要调用 p->~T()
static_assert(
    is_destructible<int>::value,
    "MemoryPool 管理的类型必须是可析构的"
);


/*===============================================
*  使用说明（教学参考）
*
*  典型用法 1 —— 手动控制生命周期：
*  ─────────────────────────────────
*     MemoryPool<MyClass, 100> pool;
*     MyClass* p = pool.allocate();      // 获取未初始化内存
*     new(p) MyClass(42);                // placement new 构造对象
*     p->someMethod();                   // 使用对象
*     p->~MyClass();                     // 显式析构
*     pool.deallocate(p);                // 归还槽位
*
*  典型用法 2 —— 使用便捷方法 construct/destroy：
*  ────────────────────────────────────────────────
*     MemoryPool<MyClass, 100> pool;
*     MyClass* p = pool.construct(42);   // 一步完成分配+构造
*     p->someMethod();                   // 使用对象
*     pool.destroy(p);                   // 一步完成析构+归还
*
*  典型用法 3 —— 模拟创建大量临时对象（游戏粒子/网络包等）：
*  ────────────────────────────────────────────────────────
*     MemoryPool<Particle, 10000> pool;
*     for (int frame = 0; frame < 1000; ++frame) {
*         // 每帧创建一批粒子
*         for (int i = 0; i < 100; ++i) {
*             Particle* p = pool.construct(x, y, vx, vy);
*             updateAndRender(p);
*         }
*         // 帧末批量归还（不归还给 OS，复用非常快）
*     }
*
*  性能对比思路：
*  ──────────────
*  内存池 vs new/delete 的关键区别：
*
*    new T(args):   1) malloc 找空闲块（系统调用）
*                   2) 调用构造函数
*
*    pool.construct: 1) 从链表头取 Slot（纯指针操作，无系统调用）
*                    2) placement new 调用构造函数
*
*    同理，delete vs destroy 的释放路径也是 O(1) 链表插入 vs free()
*    对于频繁创建/销毁小对象的场景，性能差距可达数倍到数十倍。
================================================*/
