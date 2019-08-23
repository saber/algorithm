/*
 * CopyRight (c) 2019 gcj
 * File: heap.hpp
 * Project: algorithm
 * Author: gcj
 * Date: 2019/8/23
 * Description: heap simple implementation
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#ifndef GLIB_HEAP_H_
#define GLIB_HEAP_H_

#include "../internal/macros.h"
#include "assert.h"
#include <iostream>
#include <vector>
#include <initializer_list>
#include <algorithm> // swap

//! \brief 简单实现了一个大（小）顶堆以及堆排序
//!     外部调用核心函数：
//!         1）堆中插入数据：Insert()
//!         2）删除堆顶元素：RemoveTop()
//!         3）堆排序：Sort()
//!     外部调用状态函数：
//!         1）查看堆内元素：print_heap()
//!         2）查看堆内有效元素大小：size()
//!         3）堆内元素是否有序：ordered()
//!     内部辅助核心函数：
//!         1）大（小）顶堆比较函数：Compare()
//!         2）堆化（低->上）：HeapifyUp()
//!         3）堆化（上->下）：HeapifyDown()
//!         4）建堆：BuildHeap()
//!         5）扩容堆容量：Extend()
//!         6）缩减堆容量：Reduce()
//! \Note
//!     1）暂且不支持自定义类对象
//!
//! \TODO
//!     1）支持自定义类
//!
//! \platform
//!     ubuntu16.04 g++ version 5.4.0

namespace glib {
namespace internal {
    enum HeapOption { BIG_HEAP, SMALL_HEAP };
} // namespace internal

using namespace internal;

template <typename _Scalar, HeapOption _Option = BIG_HEAP>
class Heap {
public:  // type or struct declaration
    using ValueType = _Scalar;

public:  // construct function
    Heap(std::vector<ValueType> vec) {
        capacity_ = vec.size();
        size_ = 0;
        heap_ = new ValueType[capacity_ + 1];
        for (const auto& element: vec) {
            Insert(element);
            // print_heap();
        }
    }
    Heap(std::initializer_list<ValueType> il) {
        capacity_ = il.size();
        size_ = 0;
        heap_ = new ValueType[capacity_ + 1];
        for (const auto& element: il) {
            Insert(element);
            // print_heap();
        }
    }
    ~Heap() {
        if (heap_ != nullptr) delete heap_;
    }
    GLIB_DISALLOW_IMPLICIT_CONSTRUCTORS_PUBLIC(Heap);
public:  // external call function
    // 向堆中插入一个元素（从后向前插入，之后堆化）
    void Insert(const ValueType& key);

    // 删除堆顶元素
    void RemoveTop();

    // 排序
    void Sort();

    // 查看堆内元素
    void print_heap() {
        for (size_t i = 1; i < size_ + 1; i++) {
            std::cout << heap_[i] << " ";
        }
        std::cout << std::endl;
    }

    inline size_t size()     const {return size_;    } // 返回当前堆大小（有效堆）
    inline size_t capacity() const {return capacity_;} // 返回当前堆的容量
    inline bool   ordered()  const {return ordered_; } // 判断当前堆内元素是否是有序的
private: // internal helper function
    // 大顶堆和小顶堆比较函数
    bool Compare(const ValueType& first, const ValueType& second);

    // 堆化操作
    // 从底向上堆化
    void HeapifyUp(size_t index);

    // 从上向下堆化
    void HeapifyDown(size_t index);

    // 建堆，重新把内部数据组织成一个堆
    void BuildHeap();

    // 扩充容量
    inline void Extend();

    // 减少容量
    inline void Reduce();


private: // internal member variable
    ValueType* heap_     = nullptr; // 不使用 0 索引！
    size_t     capacity_ = 0;       // 当前堆的容量
    size_t     size_     = 0;       // 当前堆中存储的有效容量

    bool       ordered_  = false;   // 当前堆中元素是否是有序的
    bool       need_build_heap_  = false; // 判断是否需要建堆
};

//----------------------- external call function------------------------------//
// 从堆低插入一个元素，之后进行堆化
// \complexity 时间复杂度为：最好： O(logn) 最坏：O(n) 平均：O(logn)
//             空间复杂度为：最好：O(1) 最坏：O(n) 平均 O(1)
template <typename _Scalar, HeapOption _Option>
void Heap<_Scalar, _Option>::Insert(const ValueType& key) {
    // 如果当前堆经历了排序，那么内部元素就假定不符合堆的条件
    // 需要从新建立一个堆
    if (need_build_heap_)
        BuildHeap();
    Extend();         // 如果容量不够，那么就会扩充容量
    ++size_;
    heap_[size_] = key;
    HeapifyUp(size_); // 从最后一个元素（堆底）向上堆化
}

//! \brief 删除堆顶元素
//! \complexity 时间复杂度为：最好： O(logn) 最坏：O(n) 平均：O(logn)
//!             空间复杂度为：最好：O(1) 最坏：O(n) 平均 O(1)
template <typename _Scalar, HeapOption _Option>
void Heap<_Scalar, _Option>::RemoveTop() {
    if (size_ <= 1) {
        size_ = 0;
        return;
    }
    if (need_build_heap_)
        BuildHeap();
    heap_[1] = heap_[size_]; // 用堆底元素替换堆顶元素
    size_--;                 // 不能与下面堆化交换顺序！
    HeapifyDown(1);
    Reduce();                // 看看是不是可以缩减容量
}

// 排序
// \note 1）在调用该函数之前，Insert 函数每次插入数据，都会调整其为堆，
//          也就是说整个堆都是提前建立好了的！只要调用该函数，那么就省略
//          刚开始的建堆过程
//       2）默认情况下根据选择大堆和小堆的不同，这里会自动按照从小到大排
//          序，和从大到小排序
template <typename _Scalar, HeapOption _Option>
void Heap<_Scalar, _Option>::Sort() {
    if (ordered_) return;
    int reserve_size = size_;
    while (size_ > 1) {
        std::swap(heap_[size_], heap_[1]);
        size_--;
        HeapifyDown(1);
    }
    size_ = reserve_size;
    ordered_ = true; // 标记当前堆内元素是有序的
    need_build_heap_ = true; // 假定排序完毕后，内部就不是堆了！
}

//----------------------- internal helper function----------------------------//
// 大顶堆和小顶堆比较函数
// \complexity O(1)
template <typename _Scalar, HeapOption _Option>
bool Heap<_Scalar, _Option>::Compare(const ValueType& first,
                                     const ValueType& second) {
    if (_Option == HeapOption::BIG_HEAP) {
        return (first > second)? true: false;
    }
    if (_Option == HeapOption::SMALL_HEAP) {
        return (first < second)? true: false;
    }
}

// 向上堆化
// \complexity 时间复杂度为 O(logn) 空间复杂度为 O(1)
template <typename _Scalar, HeapOption _Option>
void Heap<_Scalar, _Option>::HeapifyUp(size_t index ) {
    assert(1 <= index && index <= size_ &&
           "index over heap size or index < 1");
    while (index > 1 && Compare(heap_[index], heap_[index/2])) {
            std::swap(heap_[index], heap_[index/2]);
            index /= 2;
    }
    return;
}

// 向下堆化
// \complexity 时间复杂度为 O(logn) 空间复杂度为 O(1)
template <typename _Scalar, HeapOption _Option>
void Heap<_Scalar,_Option>::HeapifyDown(size_t index) {
    assert(1 <= index && index <= size_ &&
           "index over heap size or index < 1");
    while (true) {
        ValueType extremum_value_index = index;
        if (2 * index <= size_ &&
            Compare(heap_[2 * index], heap_[extremum_value_index]))
            extremum_value_index = 2 * index;
        if (2 * index + 1 <= size_ &&
            Compare(heap_[2 * index + 1], heap_[extremum_value_index]))
            extremum_value_index = 2 * index + 1;

        // 如果没有交换，那么堆化成功
        if (extremum_value_index == index) break;

        std::swap(heap_[extremum_value_index], heap_[index]);
        index = extremum_value_index;
    }
    return;
}

// 建堆，重新把内部数据组织成一个堆
// \complexity 时间复杂度：O(n)
template <typename _Scalar, HeapOption _Option>
void Heap<_Scalar, _Option>::BuildHeap() {
    size_t index = size_/2;
    while (index >= 1) {
        HeapifyDown(index);
        index--;
    }
    ordered_         = false; // 此时默认没有顺序
    need_build_heap_ = false; // 表示已经排好序了
}

// 扩充容量
// \complexity 时间复杂度为：平均情况为 O(1) 最好：O(1)，最坏：O(n) 空间复杂度同理
template <typename _Scalar, HeapOption _Option>
inline void Heap<_Scalar, _Option>::Extend() {
    // 超出当前堆的容量，自动扩容 2 倍
    if (size_ + 1 > capacity_) {
        ValueType* temp_array = new ValueType[2 * capacity_ + 1];
        for (int i = 1; i <= size_; ++i)
            temp_array[i] = heap_[i];
        delete heap_;
        heap_ = temp_array;
        capacity_ = 2 * capacity_;
    }
    return;
}

// 减少容量
// \complexity 时间复杂度为：平均情况为 O(1) 最好：O(1)，最坏：O(n) 空间复杂度同理
template <typename _Scalar, HeapOption _Option>
inline void Heap<_Scalar, _Option>::Reduce() {
    // 看看是否需要缩容
    if (size_ <= 0.25 * capacity_ && 0.5 * capacity_ >= 4) {
        ValueType* temp_heap = new ValueType[int(0.5 * capacity_ + 1)];
        for (int i = 1; i <= size_; ++i)
            temp_heap[i] = heap_[i];
        delete heap_;
        capacity_ = 0.5 * capacity_;
        heap_ = temp_heap;
    }
    return;
}

} // namespace glib

#endif // GLIB_HEAP_H_
