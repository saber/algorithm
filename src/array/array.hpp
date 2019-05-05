/*
 * CopyRight (c) 2019 gcj
 * File: array.hpp
 * Project: algorithm
 * Author: gcj
 * Date: 2019/4/8
 * Description: dynamic array simple implementation
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#ifndef GLIB_ARRAY_HPP_
#define GLIB_ARRAY_HPP_

#include <iostream>
#include <assert.h>
#include <cstring>          // use memcpy() 对于 stl 类类型 无法使用！
#include <math.h>
#include <algorithm>        // use sort()
#define GUARD_ORDERED(x)    // 保证有序

//! \brief 简单实现了一个动态的数组
//!        基本功能：
//!           1）支持动态增加内存从后插入元素、下标访问功能[]、合并两个有序数组（仅支持小到大内置类型）
//!                 对应函数：PushBack、operator[]、Merge
//!
//!           2）其他普通函数：返回数组有效字节数、数组总容量、排序函数
//!                 对应函数：size、capacity、Sort
//!
//!           3）内部函数：比较两个元素大小
//!                 对应函数：Compare
//!
//! \Note
//!      1）该数组不支持，赋值、移动赋值、移动拷贝功能，仅仅做了一个动态数组的简单实现
//!
//! \TODO
//!      1）支持移动赋值、移动构造功能
//!      1）实现 LRU 缓存淘汰策略——在单链表中已经实现了
//!      2）实现一个大小固定的有序数组，支持动态增删改操作——实际上考核插入时的数据移动、删除时的垃圾回收策略
//!      3）leetCode 题目待查找
//!
//! \platform
//!      ubuntu16.04 g++ version 5.4.0

namespace glib {
using namespace std;

namespace array_internal {

//! \brief 比较两个元素的大小
//! \return 1: 1 > 2
//!         0: 1 == 2
//!        -1: 1 < 2
template <typename T>
int Compare(T &element1, T &element2) {
    if (element1 > element2)
        return 1;
    if (element1 == element2)
        return 0;
    if (element1 < element2)
        return -1;
}
} // namespace internal

//! \brief 动态扩容的数组
template <typename T = int>
class Array {
public:

    Array() : size_(1), offset_(0) { // 默认申请构造一个大小空间
        array_ = new T[size_];
        if (nullptr == array_)
            exit(-1);
        array_[0] = T();
    }

    // 构造指定大小数组
    Array(size_t size) : size_(size), offset_(size_-1) {
        assert(size_); // 保证至少申请一个元素
        array_ = new T[size_]();
        if (nullptr == array_)
            exit(-1);
        for (size_t i = 0; i < size_; i++)
            array_[i] = T();
        // memset(array_, T(), size_); // 仅仅适用与基本类型
    }

    Array(const Array &obj) : size_(obj.size_), offset_(obj.offset_) {
        array_ = new T[size_]();
        for (size_t i = 0; i <= offset_; i++)
            array_[i] = obj.array_[i];
        // memcpy(array_, obj.array_, sizeof(T) * size_); // stl 类无法使用
    }

    // 删除赋值函数
    Array& operator=(const Array &obj) = delete;
    ~Array() { delete[] array_; }

    size_t size()     const {return offset_ + 1;                 } // 返回数组有效字节数
    size_t capacity() const {return size_;                       } // 返回数组容量
    void   Sort()           {sort(array_, array_ + offset_ + 1); } // 从小到大排序，最后一个输入的是无效的元素

    //! \brief 从数组后面插入数组元素
    //! \note 数组满了的时候，会进行申请内存，(数组内至少有一个元素)
    //! \complexity best case O(1) worst case O(n) average case O(1)
    void PushBack(const T& element) {
        offset_++;
        if (offset_ > size_-1) { // 建立新的存储空间
            size_t new_size = size_ + size_/2 + 1;
            if (new_size <= 5) new_size = 5;
            T *array_temp = new T[new_size]();
            // memcpy(array_temp, array_, sizeof(T) * size_);
            for (size_t i = 0; i < size_; i++)
                array_temp[i] = array_[i];
            delete[] array_;
            array_ = array_temp;
            size_  = new_size;
        }
        array_[offset_] = element;
    }

    //! \note 必须定义下面两个重载(注意数组越界问题)

    // 重载 [] ，可以想数组那样利用下标进行访问
    T& operator[](size_t index) {
        if (index >= size_) {
            std::cerr << "数组越界" << endl;
            exit(-1);
        }
        return *(array_ + index);
    }

    const T& operator[](size_t index) const { // 只能作为右值
        if (index >= size_) {
            std::cerr << "数组越界" << endl;
            exit(-1);
        }
        return *(array_ + index);
    }

private:
    T*     array_;  // 数组指针
    size_t size_;   // 数组总容量（内存空间）
    size_t offset_; // 数组中当前数据的有效偏移，PushBack() 后会在 offset_ + 1 处加入新元素。

}; // class Array


//! \brief 将两个有序数组合并为一个
//! \note 假定两个有序数组都是从小到大排序好的，仅支持基本数据类型
//! \complexity 时间复杂度：O(n)，空间复杂度：O(n)
//! \method 直接比较两个数组的最小值，谁小谁先放在临时数组中。知道所有值都放入到临时数组中
//! \TODO
//!     1）支持类类型的数组合并比如 std::string
template <typename T>
Array<T> Merge(Array<T> &array1, Array<T> &array2) {
    // 为了安全可以先排序
    // array1.Sort();
    // array2.Sort();
    GUARD_ORDERED(array1);
    GUARD_ORDERED(array2);

    // 合并两个数组
    int array_index1 = 0;
    int array_index2 = 0;
    Array<T> temp(array1.size() + array2.size());
    for (size_t i = 0; i < temp.size(); ++i) {
        if (array_index1 != array1.size() && array_index2 != array2.size()) {
            if (array_internal::Compare(array1[array_index1], array2[array_index2]) <= 0)
                temp[i] = array1[array_index1++];
            else
                temp[i] = array2[array_index2++];
        } else {
            if (array_index1 == array1.size())
                temp[i] = array2[array_index2++];
            else
                temp[i] = array1[array_index1++];
        }
    }

    return temp;
}

} // namespace glib

#endif // GLIB_ARRAY_HPP_
