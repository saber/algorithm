/*
 * File: sort.hpp
 * Project: algorithm
 * Author: gcj
 * Date: 2019/4/15
 * Description: some sort algorithms
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#ifndef GLIB_SORT_HPP_
#define GLIB_SORT_HPP_
#include <cstddef> // 定义了 size_t 类型
#include <vector>
#include <assert.h>
#include <iostream>

namespace glib {
using namespace std;

//! \brief 简单实现冒泡排序、插入排序、选择排序、快速排序、归并排序、桶排序、基数排序
//!
//! \TODO
//!     1）快速排序、归并排序、桶排序、基数排序的基本实现，
//!     2) 目前的实现版本是从小到大排序。没有进行大到小的转换
//!     3）转换为 stl 接口形式。可以接受一个外部的谓语。比较 Key 值来排序类元素！
//!
//! \conclusion 在数值排序时，推荐使用插入排序(二分形式)。其次是不稳定的选择排序。
//!  冒泡排序、插入排序、选择排序。在排序交换数值类型时。经过测试 10000 个数据，得到的结果显示，效率如下：
//!            插入排序 > 选择排序 > 冒泡排序。倍数关系依次为 5 倍、2 倍
//!            在插入排序中，自己的实现方式：二分查找 > 后向比较 > 前向比较。依次相差 50ms、20ms
//!            在选择排序中，自己实现方式：不稳定选择排序 > 稳定选择排序(类似冒泡)。约为 2 倍的关系
template <typename _Scalar>
class Sort {
public:
    enum class Option {
        kBubble,
        kInsertion,
        kSelection
    };
    // 这个可以直接用类的名字操作，也可以用该类的对象操作。只接受 vector 容器
    // TODO : 原因未知
    void operator()(vector<_Scalar> &array, const Option option = Option::kBubble) {
        switch (option) {
            case Option::kBubble: {
                Bubble(array);
                break;
            }
            case Option::kInsertion: {
                Insertion(array);
                break;
            }
            case Option::kSelection: {
                // SelectionStable(array);
                SelectionUnstable(array); // 不稳定排序算法，对于排序和交换的原始是内置数值类型，此时不稳定的排序要快快与稳定的排序。
            }
        }
    }

private:
    // 打印当前数组元素顺序值
    void print_value(const vector<_Scalar> &array) {
        for (auto &b: array)
            cout << " " << b;
        cout << endl;
    }

    // 冒泡排序
    void Bubble(vector<_Scalar> &array) {
        cout << "Bubble Sort" << endl;
        assert(array.size() != 0);
        auto n = array.size();
        for (size_t i = 0; i < n; i++) {
            bool success_sort = false;
            for (size_t j = 0; j < n - 1 - i; j++) {
                if (array[j] > array[j+1]) {
                    // 满足交换顺序，可以直接交换
                    _Scalar temp = array[j];
                    array[j] = array[j+1];
                    array[j+1] = temp;

                    success_sort = true;
                }
            }
            if (!success_sort) return; // 数据已经是有序状态
        }
    }

    // 插入排序:移动操作总数等于逆序度。不管什么操作，移动次数是不变的
    // 可以分别注释下面的三种遍历方式 1)二分查找方式 2）前向遍历 3）后向遍历 看看不同的效果对应的时间
    // 经过测试 二分查找方式执行时间最短。是因为交换移动次数是固定的，只要比较的次数少一些。时间就会少！
    void Insertion(vector<_Scalar> &array) {
        cout << "Insertion Sort" << endl;
        assert(array.size() != 0);
        auto n = array.size();
        if (1 == n) return;

        // 查找插入方法用的是从头到尾的方法
        for (size_t separate_point = 1; separate_point < n; ++separate_point) {
            auto separate_point_value = array[separate_point];
            //------分割线---------------
            // 1)利用二分查找方式:从前到后 。这种方式的运行效率最高。比下面的尾到头的方式时间要短了 50 ms
            size_t low_limit = 0;
            size_t up_limit = separate_point - 1;
            // size_t middle = (low_limit + up_limit)/2;
            size_t middle = low_limit + (up_limit - low_limit)/2;
            while (low_limit <= up_limit) { // 二分查找
                if ((0 == middle && array[middle] > separate_point_value) || // 保证稳定排序，只有大于当前 separate_point 对应的值才会交换
                    (middle >= 1 &&
                    array[middle] > separate_point_value &&
                    array[middle-1] <= separate_point_value)) {
                    // 插入第一个位置或者中间某个位置，交换
                    auto temp_value = array[separate_point];
                    for (size_t j = separate_point; j > middle; --j)
                        array[j] = array[j-1];
                    array[middle] = temp_value;
                    break;
                }

                if (array[middle] <= separate_point_value) { // 保证稳定排序
                    low_limit = middle + 1;
                } else {
                    up_limit = middle - 1;
                }
                middle = low_limit + (up_limit - low_limit)/2;
                // middle = (low_limit + up_limit)/2;
            }

            // 2)遍历循环查找：从头到尾。实现的方式效率最低。
            // for (size_t i = 0; i < separate_point; ++i) { // 遍历查找复杂度高，因为假定前面已经是有序的了，所以可以使用二分查找
            //     if (separate_point_value < array[i]) {
            //         // 交换位置
            //         auto temp_value = array[separate_point];
            //         for (size_t j = separate_point; j > i ; --j)
            //             array[j] = array[j-1];
            //         array[i] = temp_value;
            //         break; // 交换完成后直接退出当前循环
            //     }
            // }

            // 3)遍历循环查找：从尾到头，一遍比较一遍交换。当前实现的效率不是最高的。比上面第 2 中方式高些。1000 个数句差了 20 ms 左右
            // int i = separate_point - 1;
            // for (; i >= 0 ; --i) {
            //     if (array[i] > separate_point_value) // 向后移动一位
            //         array[i+1] = array[i];
            //     else
            //         break;
            // }
            // array[i+1] = separate_point_value;

            // debug
            // 每次排序完成，打印一下顺序
            // cout << "separate_point: " << separate_point << " and its value: " << separate_point_value << endl;
            // print_value(array);
        }
    }

    // 选择排序（稳定版）：从剩下的未排序数组中找到最小值，然后放到已排序序列的后面。
    // 下面的实现方式，本质上与冒泡排序一致。是一个稳定的选择排序
    void SelectionStable(vector<_Scalar> &array) {
        assert(array.size() != 0);
        cout << "Selection Sort" << endl;

        // 需要遍历 n 次，每次寻找一个最小值。可以从后向前遍历。一边遍历一边交换顺序，实际上是上面插入排序以及冒泡排序实现的思路。
        for (size_t i = 0; i < array.size(); ++i) {
            int j = array.size() - 1;
            bool success_sort = false;
            for (; j > i; --j) { // 在剩下的未排序数组中找到最小值
                if (array[j] < array[j-1]) {
                    auto temp_value = array[j-1];
                    array[j-1] = array[j];
                    array[j] = temp_value;
                    success_sort = true;
                }
            }
            if (!success_sort) return;
        }
    }

    // 不稳定的选择排序，其时间要比上面的稳定选择排序快。仅仅针对数值内置类型。
    void SelectionUnstable(vector<_Scalar> &array) {
        assert(array.size() != 0);
        cout << "Unstable Selection" << endl;
        for (size_t i = 0; i < array.size() - 1; ++i) {
            auto min_index = i;
            for (size_t j = i + 1; j < array.size(); ++j) {
                if (array[j] < array[min_index])
                    min_index = j;
            }
            // 交换元素
            if (min_index != i) {
                auto temp_value = array[min_index];
                array[min_index] = array[i];
                array[i] = temp_value;
            }
        }
    }


}; // class Sort

} // namespace glib
#endif
