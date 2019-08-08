# 单生产者/单消费者 的 FIFO 无锁队列

&emsp;&emsp;发现 zeromq 的 yqueue_t 模板类，其数据存储理念设计得非常妙。借这一理念，按照 STL 的泛型类 queue 的接口标准，我设计了一个线程安全的 单生产者/单消费者（单线程push/单线程pop） FIFO 队列，以此满足更为广泛的应用。

## 1. 数据存储理念的结构图

![type_index](https://raw.githubusercontent.com/Gaaagaa/repository/master/images/xspsc_queue.png)

> - 队列的整体结构上，使用链表的方式，将多个固定长度的 chunk 串联起来；
> - 每个 chunk 则可用于存储队列所需要的元素；
> - 增加一个可交换的 chunk 单元，利于内存复用；
> - 队列使用时，支持 单个线程的 push（生产） 和 单个线程 pop（消费）的并发操作（内部并未加锁）。

## 2. 源码 （xspsc_queue.h）

```
/**
 * @file    xspsc_queue.h
 * <pre>
 * Copyright (c) 2018, Gaaagaa All rights reserved.
 * 
 * 文件名称：xspsc_queue.h
 * 创建日期：2018年12月17日
 * 文件标识：
 * 文件摘要：实现线程安全的 单生产者/单消费者（single producer/single consumer） FIFO 队列。
 * 
 * 特别声明：x_spsc_queue_t 的设计，主要参考了 zeromq 的 yqueue_t 模板类的数据存储理念。
 * 特别鸣谢：zeromq 开源项目，Lee哥 。
 * 
 * 当前版本：1.0.0.0
 * 作    者：
 * 完成日期：2018年12月17日
 * 版本摘要：
 * 
 * 历史版本：
 * 原作者  ：
 * 完成日期：
 * 版本摘要：
 * </pre>
 */

#ifndef __XSPSC_QUEUE_H__
#define __XSPSC_QUEUE_H__

#include <memory>
#include <atomic>

////////////////////////////////////////////////////////////////////////////////

#ifndef ENABLE_XASSERT
#if ((defined _DEBUG) || (defined DEBUG))
#define ENABLE_XASSERT 1
#else // !((defined _DEBUG) || (defined DEBUG))
#define ENABLE_XASSERT 0
#endif // ((defined _DEBUG) || (defined DEBUG))
#endif // ENABLE_XASSERT

#ifndef XASSERT
#if ENABLE_XASSERT
#include <cassert>
#define XASSERT(xptr)    assert(xptr)
#else // !ENABLE_XASSERT
#define XASSERT(xptr)
#endif // ENABLE_XASSERT
#endif // XASSERT

////////////////////////////////////////////////////////////////////////////////
// x_spsc_queue_t

/**
 * @class x_spsc_queue_t
 * @brief 实现线程安全的 单生产者/单消费者（single producer/single consumer） FIFO 队列。
 * 
 * @param [in ] _Ty    : 队列存储的元素类型。
 * @param [in ] _En    : 队列中的存储块可容纳元素的数量。
 * @param [in ] _Alloc : 元素分配器。
 */
template< typename _Ty, size_t _En, typename _Alloc = std::allocator< _Ty > >
class x_spsc_queue_t : protected _Alloc
{
    static_assert(_En >= 4, "_En size value must be greater than or equal to 4!");

    // common data types
public:
    using x_element_t = _Ty;

private:
    /**
     * @struct x_chunk_t
     * @brief  存储元素节点的连续内存块结构体。
     */
    typedef struct x_chunk_t
    {
        x_element_t * xet_array;   ///< 当前内存块中的元素节点数组
        x_chunk_t   * xprev_ptr;   ///< 指向前一内存块节点
        x_chunk_t   * xnext_ptr;   ///< 指向后一内存块节点
    } x_chunk_t;

#ifdef _MSC_VER
    using ssize_t = std::intptr_t;
#endif // _MSC_VER

    using x_chunk_ptr_t   = x_chunk_t *;
    using x_atomic_ptr_t  = std::atomic< x_chunk_ptr_t >;
    using x_atomic_size_t = std::atomic< size_t >;
    using x_allocator_t   = _Alloc; 
    using x_chunk_alloc_t = typename std::allocator_traits< x_allocator_t >::template rebind_alloc< x_chunk_t >;

    // constructor/destructor
public:
    explicit x_spsc_queue_t(void)
        : m_chk_begin_ptr(nullptr)
        , m_xst_begin_pos(0)
        , m_chk_end_ptr(nullptr)
        , m_xst_end_pos(0)
        , m_chk_back_ptr(nullptr)
        , m_xst_back_pos(0)
        , m_xst_queue_size(0)
        , m_chk_swap_ptr(nullptr)
    {
        m_chk_begin_ptr = m_chk_end_ptr = alloc_chunk();
    }

    ~x_spsc_queue_t(void)
    {
        x_chunk_t * xchunk_ptr = nullptr;

        while (size() > 0)
            pop();

        while (true)
        {
            if (m_chk_begin_ptr == m_chk_end_ptr)
            {
                free_chunk(m_chk_begin_ptr);
                break;
            }

            xchunk_ptr = m_chk_begin_ptr;
            m_chk_begin_ptr = m_chk_begin_ptr->xnext_ptr;
            if (nullptr != xchunk_ptr)
                free_chunk(xchunk_ptr);
        }

        xchunk_ptr = m_chk_swap_ptr.exchange(nullptr);
        if (nullptr != xchunk_ptr)
            free_chunk(xchunk_ptr);
    }

    x_spsc_queue_t(x_spsc_queue_t && xobject) = delete;
    x_spsc_queue_t(const x_spsc_queue_t & xobject) = delete;
    x_spsc_queue_t & operator=(const x_spsc_queue_t & xobject) = delete;

    // public interfaces
public:
    /**********************************************************/
    /**
     * @brief 当前队列中的元素数量。
     */
    inline size_t size(void) const
    {
        return m_xst_queue_size;
    }

    /**********************************************************/
    /**
     * @brief 判断队列是否为空。
     */
    inline bool empty(void) const
    {
        return (0 == size());
    }

    /**********************************************************/
    /**
     * @brief 向队列尾端压入一个元素。
     */
    void push(const x_element_t & xemt_value)
    {
        x_allocator_t::construct(&m_chk_end_ptr->xet_array[m_xst_end_pos], xemt_value);

        m_chk_back_ptr = m_chk_end_ptr;
        m_xst_back_pos = m_xst_end_pos;

        m_xst_queue_size.fetch_add(1);
        move_end_pos();
    }

    /**********************************************************/
    /**
     * @brief 向队列尾端压入一个元素。
     */
    void push(x_element_t && xemt_value)
    {
        x_allocator_t::construct(&m_chk_end_ptr->xet_array[m_xst_end_pos],
                                 std::forward< x_element_t >(xemt_value));

        m_chk_back_ptr = m_chk_end_ptr;
        m_xst_back_pos = m_xst_end_pos;

        m_xst_queue_size.fetch_add(1);
        move_end_pos();
    }

    /**********************************************************/
    /**
     * @brief 从队列前端弹出一个元素。
     */
    void pop(void)
    {
        if (empty())
            return;
        m_xst_queue_size.fetch_sub(1);
        x_allocator_t::destroy(&m_chk_begin_ptr->xet_array[m_xst_begin_pos]);
        move_begin_pos();
    }

    /**********************************************************/
    /**
     * @brief 返回队列首个元素。
     */
    inline x_element_t & front(void)
    {
        XASSERT(!empty());
        return m_chk_begin_ptr->xet_array[m_xst_begin_pos];
    }

    /**********************************************************/
    /**
     * @brief 返回队列首个元素。
     */
    inline const x_element_t & front(void) const
    {
        XASSERT(!empty());
        return m_chk_begin_ptr->xet_array[m_xst_begin_pos];
    }

    /**********************************************************/
    /**
     * @brief 返回队列末端元素。
     */
    inline x_element_t & back(void)
    {
        XASSERT(!empty());
        return m_chk_back_ptr->xet_array[m_xst_back_pos];
    }

    /**********************************************************/
    /**
     * @brief 返回队列末端元素。
     */
    inline const x_element_t & back(void) const
    {
        XASSERT(!empty());
        return m_chk_back_ptr->xet_array[m_xst_back_pos];
    }

    // internal invoking
private:
    /**********************************************************/
    /**
     * @brief 申请一个存储元素节点的内存块。
     */
    x_chunk_ptr_t alloc_chunk(void)
    {
        x_chunk_alloc_t xchunk_allocator(*(x_allocator_t *)this);

        x_chunk_ptr_t xchunk_ptr = xchunk_allocator.allocate(1);
        XASSERT(nullptr != xchunk_ptr);

        if (nullptr != xchunk_ptr)
        {
            xchunk_ptr->xet_array = x_allocator_t::allocate(_En);
            XASSERT(nullptr != xchunk_ptr->xet_array);

            if (nullptr != xchunk_ptr->xet_array)
            {
                xchunk_ptr->xprev_ptr = nullptr;
                xchunk_ptr->xnext_ptr = nullptr;
            }
            else
            {
                xchunk_allocator.deallocate(xchunk_ptr, 1);
                xchunk_ptr = nullptr;
            }
        }

        return xchunk_ptr;
    }

    /**********************************************************/
    /**
     * @brief 释放一个存储元素节点的内存块。
     */
    void free_chunk(x_chunk_ptr_t xchunk_ptr)
    {
        if (nullptr != xchunk_ptr)
        {
            if (nullptr != xchunk_ptr->xet_array)
                x_allocator_t::deallocate(xchunk_ptr->xet_array, _En);

            x_chunk_alloc_t xchunk_allocator(*(x_allocator_t *)this);
            xchunk_allocator.deallocate(xchunk_ptr, 1);
        }
    }

    /**********************************************************/
    /**
     * @brief 将起始端位置向后移（该接口仅由 pop() 接口调用）。
     */
    void move_begin_pos(void)
    {
        if (++m_xst_begin_pos == _En)
        {
            x_chunk_ptr_t xchunk_ptr = m_chk_begin_ptr;
            m_chk_begin_ptr = m_chk_begin_ptr->xnext_ptr;
            XASSERT(nullptr != m_chk_begin_ptr);
            m_chk_begin_ptr->xprev_ptr = nullptr;
            m_xst_begin_pos = 0;

            xchunk_ptr = m_chk_swap_ptr.exchange(xchunk_ptr);
            if (nullptr != xchunk_ptr)
                free_chunk(xchunk_ptr);
        }
    }

    /**********************************************************/
    /**
     * @brief 将结束端位置向后移（该接口仅由 push() 接口调用）。
     */
    void move_end_pos(void)
    {
        if (++m_xst_end_pos == _En)
        {
            x_chunk_ptr_t xchunk_ptr = m_chk_swap_ptr.exchange(nullptr);
            if (nullptr != xchunk_ptr)
            {
                m_chk_end_ptr->xnext_ptr = xchunk_ptr;
                xchunk_ptr->xprev_ptr = m_chk_end_ptr;
            }
            else
            {
                m_chk_end_ptr->xnext_ptr = alloc_chunk();
                m_chk_end_ptr->xnext_ptr->xprev_ptr = m_chk_end_ptr;
            }

            m_chk_end_ptr = m_chk_end_ptr->xnext_ptr;
            m_xst_end_pos = 0;
        }
    }

    // data members
protected:
    x_chunk_ptr_t    m_chk_begin_ptr;  ///< 内存块链表的起始块
    ssize_t          m_xst_begin_pos;  ///< 队列中的首个元素位置
    x_chunk_ptr_t    m_chk_end_ptr;    ///< 内存块链表的结束块
    ssize_t          m_xst_end_pos;    ///< 队列中的元素结束位置
    x_chunk_ptr_t    m_chk_back_ptr;   ///< 内存块链表的结尾块
    ssize_t          m_xst_back_pos;   ///< 队列中的结尾元素位置
    x_atomic_size_t  m_xst_queue_size; ///< 队列中的有效元素数量
    x_atomic_ptr_t   m_chk_swap_ptr;   ///< 用于保存临时内存块（备用缓存块）
};

////////////////////////////////////////////////////////////////////////////////

#endif // __XSPSC_QUEUE_H__

```

## 3. 使用示例

```
/**
 * @file    main.cpp
 * <pre>
 * Copyright (c) 2019, Gaaagaa All rights reserved.
 * 
 * 文件名称：main.cpp
 * 创建日期：2019年02月07日
 * 文件标识：
 * 文件摘要：单生产者/单消费者（single producer/single consumer）FIFO 队列 的测试程序。
 * 
 * 当前版本：1.0.0.0
 * 作    者：
 * 完成日期：2019年02月07日
 * 版本摘要：
 * 
 * 取代版本：
 * 原作者  ：
 * 完成日期：
 * 版本摘要：
 * </pre>
 */

#include "xspsc_queue.h"
#include <iostream>
#include <thread>
#include <chrono>

#include <list>

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char * argv[])
{
    using x_int_queue_t = x_spsc_queue_t< int, 8 >;

    x_int_queue_t spsc;

    std::cout << "sizeof(x_int_queue_t) : " << sizeof(x_int_queue_t) << std::endl;

    bool b_push_finished = false;
    std::thread xthread_in([&spsc, &b_push_finished](void) -> void
    {
        for (int i = 1; i < 10000; ++i)
        {
            spsc.push(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        b_push_finished = true;
    });

    std::thread xthread_out([&spsc, &b_push_finished](void) -> void
    {
        while (true)
        {
            if (!spsc.empty())
            {
                std::cout << spsc.size() << " : " << spsc.front() << std::endl;
                spsc.pop();
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            else if (b_push_finished)
            {
                break;
            }
        }
    });

    if (xthread_in.joinable())
    {
        xthread_in.join();
    }

    if (xthread_out.joinable())
    {
        xthread_out.join();
    }

    return 0;
}

```
