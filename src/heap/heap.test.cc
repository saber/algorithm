/*
 * CopyRight (c) 2019 gcj
 * File: heap.test.cc
 * Project: algorithm
 * Author: gcj
 * Date: 2019/8/22
 * Description: test heap and app
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#include "heap.hpp"
#include <iostream>
#include <vector>

//! \brief 简单测试堆及应用
//! \run
//!     g++ heap.test.cc -std=c++11 && ./a.out

using namespace std;
int main(int argc, char const *argv[]) {
    // 大堆测试
    cout << "大顶堆测试" << endl;
    // 两种不同的构造方式：初始化列表和用 vector 来构造！
    // vector<int> vec = {33, 27, 21, 16, 13, 15, 19, 5, 6, 7, 8, 1, 2, 12};
    // glib::Heap<int> heap_big(vec);
    glib::Heap<int> heap_big = {33, 27, 21, 16, 13, 15, 19, 5, 6, 7, 8, 1, 2, 12}; // 默认大堆模式
    heap_big.print_heap();   // 33 27 21 16 13 15 19 5 6 7 8 1 2 12
    heap_big.RemoveTop();
    heap_big.print_heap();   // 27 16 21 12 13 15 19 5 6 7 8 1 2
    heap_big.Sort(); // 排序
    cout << "排序后的值" << endl;
    heap_big.print_heap();

    cout << "小顶堆测试" << endl;
    glib::Heap<int, glib::internal::HeapOption::SMALL_HEAP>
        heap_small = {33, 27, 21, 16, 13, 15, 19, 5, 6, 7, 8, 1, 2, 12}; // 指定小堆
    heap_small.print_heap(); // 1 6 2 13 7 5 12 33 16 21 8 27 15 19
    heap_small.RemoveTop();
    heap_small.print_heap(); // 2 6 5 13 7 15 12 33 16 21 8 27 19
    heap_small.Sort(); // 排序
    cout << "排序后的值" << endl;
    heap_small.print_heap();

    // 测试排序后，从新建立堆
    cout << "测试排序后，从新建立堆，然后在排序" << endl;
    heap_small.Insert(1);
    heap_small.print_heap(); // 1 6 2 8 7 15 5 12 19 27 16 21 33 13
    heap_small.Sort();
    heap_small.print_heap(); // 33 27 21 19 16 15 13 12 8 7 6 5 2 1

    // 边界条件测试
    // cout << "测试边界条件" << endl;
    // glib::Heap<int> condition_heap = {1,2};
    // condition_heap.RemoveTop();
    // condition_heap.RemoveTop();
    // condition_heap.Sort();
    // condition_heap.print_heap();

    return 0;
}
