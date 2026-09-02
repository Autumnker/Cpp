/*===============================================
*   文件名称：memory_pool_fixed_size.hpp
*   创 建 者：Autumnker
*   创建日期：2026年07月06日
*   描    述：固定大小内存池 —— 教学向实现（类模板声明）
*
*   【核心思想】
*   内存池预先申请一大块连续内存（一次 new[]），然后按需切分为
*   固定大小的 Block 分发给使用者。这样做的好处：
*     1. 减少系统调用：N 次 new 变成 1 次 new[] + N 次"从链表取块"
*     2. 更好的缓存局部性：所有 Block 物理上相邻
*     3. 避免外部碎片：所有 Block 大小相同，不会产生"夹缝"
*
*   【数据结构】
*   使用 联合体(union) + 嵌入式空闲链表(embedded free list)：
*
*     union Slot {
*         T data;      // 已分配时 → 存放真实对象
*         Slot* next;  // 空闲时   → 指向下一个空闲槽位
*     };
*
*   一块内存在两种状态之间切换，空闲链表指针"嵌入"在空闲块自己内部，
*   不需要额外的堆内存来管理链表节点 —— 零额外开销。
*
*   【关键知识点】
*   - placement new（在指定地址构造对象，不分配内存）
*   - 显式析构函数调用（销毁对象，不释放内存）
*   - reinterpret_cast 进行指针回推（T* → Slot*）
*   - RAII 管理池生命周期
*   - 移动语义转移大块内存所有权
*   - union 的类型安全的类型双关 (type punning)
*
*   【为什么模板声明和实现分离到 .hpp + .tcc】
*   C++ 模板采用"实例化时编译"策略。编译器在见到模板的每个
*   具体实例化时，都需要看到完整的模板定义才能生成代码。
*   因此将声明放在 .hpp、实现在 .tcc，并在 .hpp 末尾包含 .tcc，
*   既保持了代码组织的清晰，又满足编译器的要求。
================================================*/
#ifndef _MEMORY_POOL_FIXED_SIZE_H
#define _MEMORY_POOL_FIXED_SIZE_H

#include <cstddef>      // size_t
#include <new>          // placement new
#include <utility>      // std::forward, std::move
#include <cassert>      // assert
#include <iostream>     // cerr, endl
#include <type_traits>  // is_destructible (编译期检查)

using namespace std;

/*===============================================
*  类模板：MemoryPool<T, BlockCount>
*
*  T          — 池中管理的对象类型
*  BlockCount — 预分配的槽位数量（默认 1024）
*
*  该内存池为固定大小 —— 构造时一次性分配 BlockCount 个槽位，
*  之后不会再向操作系统申请内存。如果槽位用尽，allocate() 返回 nullptr
*  （不会抛出异常）。
================================================*/
template <typename T, size_t BlockCount = 1024>
class MemoryPool {
public:
    // ---- 类型别名 ----
    using value_type = T;
    using pointer    = T*;
    using size_type  = size_t;

private:
    /*===============================================
    *  Slot：内存池的最小管理单元
    *
    *  一个 Slot 占用的内存 = max(sizeof(T), sizeof(Slot*))
    *  由 union 保证对齐 = max(alignof(T), alignof(Slot*))
    *
    *  关键：union 的构造/析构函数是空的 —— 我们手动管理
    *  T 对象的生命周期，而不是让 union 自动构造/析构 T。
    ===============================================*/
    union Slot {
        T      data;   // 分配状态：存放真实对象
        Slot*  next;   // 空闲状态：指向空闲链表中的下一个 Slot

        Slot()  {}     // 不调用 T 的构造函数
        ~Slot() {}     // 不调用 T 的析构函数
    };

    // ---- 成员变量 ----
    Slot*  pool_;         // 指向预分配的 Slot 数组（整块内存）
    Slot*  free_list_;    // 空闲链表头指针（指向第一个可用的 Slot）
    size_t allocated_;    // 当前已分配（正在使用）的槽位数量

public:
    // ============================================================
    //  构造 / 析构
    // ============================================================
    MemoryPool();
    ~MemoryPool();

    // ============================================================
    //  禁止拷贝 —— 池拥有底层内存资源，拷贝会导致双重释放
    // ============================================================
    MemoryPool(const MemoryPool&)            = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;

    // ============================================================
    //  移动语义 —— O(1) 转移整块内存所有权
    // ============================================================
    MemoryPool(MemoryPool&& other) noexcept;
    MemoryPool& operator=(MemoryPool&& other) noexcept;

    // ============================================================
    //  内存管理接口
    // ============================================================
    T*   allocate();                          // 从空闲链表取一个 Slot
    void deallocate(T* p);                    // 归还 Slot 到空闲链表

    template <typename... Args>
    T*   construct(Args&&... args);           // allocate + placement new
    void destroy(T* p);                       // 显式析构 + deallocate

    // ============================================================
    //  查询接口（在类内实现，保持简洁）
    // ============================================================
    size_t allocated_count() const { return allocated_; }
    size_t capacity()        const { return BlockCount;  }
    bool   empty()           const { return allocated_ == 0; }
    bool   full()            const { return free_list_ == nullptr; }
};

// 模板实现在 .tcc 中，由 .hpp 末尾包含
// 这样编译器在实例化模板时能看到完整定义
#include "memory_pool_fixed_size.tcc"

#endif //_MEMORY_POOL_FIXED_SIZE_H
