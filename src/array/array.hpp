/*
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
#include <cstring> // use memcpy()
#include <math.h>
namespace glib {
using namespace std;
//! \required: 实现一个支持动态扩容的数组(目前仅仅测试支持内置类型)
//!             实现 LRU 缓存淘汰策略 TODO
//!            实现一个大小固定的有序数组，支持动态增删改操作 TODO
//!            实现两个有序数组合并为一个有序数组 TODO
//! \note :本实现仅仅测试了内置类型,对于 string 、vector 等 stl 类还没有测试。对于自定义类也未测试！
//! TODO 需要测试上面的注意事项
template <typename T = int>
class Array {
public:
    Array() : size_(1), offset_(0) { // 默认申请一个
        array_ = new T[size_]();
        if (nullptr == array_)
            exit(-1);
        array_[0] = T(0);
    }
    Array(size_t size) : size_(size), offset_(size_-1) {
        assert(size_); // 保证至少申请一个元素
        array_ = new T[size_]();
        if (nullptr == array_)
            exit(-1);
        memset(array_, T(0), size_);
    }

    Array(const Array &obj) : size_(obj.size_), offset_(obj.offset_) {
        array_ = new T[size_]();
        memcpy(array_, obj.array_, sizeof(T) * size_);
    }
    // 删除赋值函数
    Array& operator=(const Array &obj) = delete;
    ~Array() { delete[] array_; }

    // 返回数组有效字节数
    size_t size() const {return offset_ + 1; }
    // 返回数组容量
    size_t capacity() const {return size_; }

    // 从数组后面插入数组元素(数组内至少有一个元素)
    void PushBack(const T& element) {
        offset_++;
        if (offset_ > size_-1) {
            size_t new_size = size_ + size_/2 + 1;
            T *array_temp = new T[new_size]();
            memcpy(array_temp, array_, sizeof(T) * size_);
            delete[] array_;
            array_ = array_temp;
            size_ = new_size;
        }
        array_[offset_] = element;
    }

    // 必须定义两个(注意数组越界问题)
    T& operator[](size_t index) {
        if (index >= size_)
            exit(-1);
        return *(array_ + index);
    }
    const T& operator[](size_t index) const{
        if (index >= size_)
            exit(-1);
        return *(array_ + index);
     }
private:
    T *array_;          // 数组指针
    size_t size_;       // 数组总容量
    int offset_;     // 数组中当前数据的有效偏移
};

} // namespace glib

#endif
