/*
 * CopyRight (c) 2019 gcj
 * File: sort.test.cc
 * Project: algorithm
 * Author: gcj
 * Date: 2019/4/15
 * Description: test and use sort algorithms
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#include "sort.hpp"
#include <iostream>
#include <chrono>

using namespace std;

size_t Random(int x) {
    return rand()%x;
}

//! \brief 排序算法简单测试，通过随机生成 10000 个数据，分别测试不同排序算法的执行时间。
//! \run
//!     g++ sort.test.cc -std=c++11 && ./a.out

int main(int argc, char const *argv[]) {
    // 随机生成一个大的数列，测试下面排序算法效率
    srand((int)time(0));
    size_t n = 10000;
    vector<int> vec_bubble(n);
    vector<int> vec_insert(n);
    vector<int> vec_selection(n);
    vector<int> vec_merge(n);
    vector<int> vec_quick(n);
    vector<int> vec_counting(n);
    for (size_t i = 0; i < n; i++) {
        size_t re = Random(n);
        vec_bubble[i] = re;
        vec_insert[i] = re;
        vec_selection[i] = re;
        vec_merge[i] = re;
        vec_quick[i] = re;
        vec_counting[i] = re;
    }


    // 冒泡排序测试
    // std::vector<int> vec_bubble = {6, 5, 4, 3, 2, 1};
    auto start_bubble = chrono::system_clock::now();
    glib::Sort(vec_bubble, glib::SortOption::BUBBLE);
    auto end_bubble = chrono::system_clock::now();
    chrono::duration<double> elaspsed_seconds_bubble = end_bubble - start_bubble;
    cout << " bubble elaspsed_seconds: " << elaspsed_seconds_bubble.count() << endl;
    // for (auto &p: vec_bubble)
    //     cout << " " << p;
    cout << endl;


    // 插入排序测试
    // std::vector<int> vec_insert = {6, 6, 6, 6, 6, 5}; // 5,5,7,8,5
    // std::vector<int> vec_insert = {5, 5, 7, 8, 5, 1}; // 5,5,7,8,5
    auto start_insert = chrono::system_clock::now();
    glib::Sort(vec_insert, glib::SortOption::INSERTION);
    auto end_insert = chrono::system_clock::now();
    chrono::duration<double> elaspsed_seconds = end_insert - start_insert;
    cout << " insert elaspsed_seconds: " << elaspsed_seconds.count() << endl;
    // for (auto &p: vec_insert)
    //     cout << " " << p;
    cout << endl;


    // 选择排序测试
    // vector<int> vec_selection = {6, 5, 4, 3, 2, 1}; // 简单测试
    // vector<int> vec_selection = {5, 5, 7, 8, 5, 1};
    auto start_selection = chrono::system_clock::now();
    glib::Sort(vec_selection, glib::SortOption::SELECTION);
    auto end_selection = chrono::system_clock::now();
    chrono::duration<double> elaspsed_seconds_selection = end_selection - start_selection;
    cout << " selection elaspsed_seconds: " << elaspsed_seconds_selection.count() << endl;
    // for (auto &p: vec_selection)
    //     cout << " " << p;
    cout << endl;


    // 归并排序测试
    // vector<int> vec_merge = {4, 4, 5, 2, 4};
    // vector<int> vec2 = {1, 6, 10, 15};
    // glib::Sort<int> u;
    // auto vv = u.Merge(vec_merge, vec2); // 测试两个有序数组融合没问题
    auto start_merge = chrono::system_clock::now();
    glib::Sort(vec_merge, glib::SortOption::MERGE);
    auto end_merge = chrono::system_clock::now();
    chrono::duration<double> elaspsed_seconds_merge = end_merge - start_merge;
    cout << " merge elaspsed_seconds: " << elaspsed_seconds_merge.count() << endl;
    // for (auto &p: vec_merge)
    //     cout << " " << p;
    cout << endl;


    // 快速排序测试
    // vector<int> vec_quick = {6, 5, 4, 3, 3, 2, 1, 5};
    auto start_quick = chrono::system_clock::now();
    glib::Sort(vec_quick, glib::SortOption::QUICK);
    auto end_quick = chrono::system_clock::now();
    chrono::duration<double> elaspsed_seconds_quick = end_quick - start_quick;
    cout << " quick elaspsed_seconds: " << elaspsed_seconds_quick.count() << endl;
    // for (auto &p: vec_quick)
    //     cout << " " << p;
    cout << endl;


    // 计数排序测试
    // vector<int> vec_counting = {6, 5, 4, 3, 2, 1, 6, 2, 0, 3, -1, -1, -2, -2, -3, -3};
    // vector<int> vec_counting = {1};
    auto start_counting = chrono::system_clock::now();
    glib::Sort(vec_counting, glib::SortOption::COUNTING);
    auto end_counting = chrono::system_clock::now();
    chrono::duration<double> elaspsed_seconds_counting = end_counting - start_counting;
    cout << " counting elaspsed_seconds: " << elaspsed_seconds_counting.count() << endl;
    // for (auto &p: vec_counting)
    //     cout << " " << p;
    cout << endl;

    // 验证后面的的排序结果是否与冒泡排序一致！进而检验后面排序算法的实现是否正确
    for (size_t i = 0; i < n; i++) {
        // cout << vec_bubble[i] << " ";
        if (vec_bubble[i] != vec_insert[i]) {
            cout << "insert sort error!" << endl;
            break;
        }
        if (vec_bubble[i] != vec_selection[i]) {
            cout << "selection sort error!" << endl;
            break;
        }
        if (vec_bubble[i] != vec_merge[i]) {
            cout << "merge sort error!" << endl;
            break;
        }
        if (vec_bubble[i] != vec_quick[i]) {
            cout << "quick sort error!" << endl;
            break;
        }
        if (vec_bubble[i] != vec_counting[i]) {
            cout << "counting sort error!" << endl;
            break;
        }
    }

    // 利用快速排序思路查找第 k 大元素
    vector<int> vev{1, 2, 3, 4, 5, 6, 6, 6};
    cout << "kth ---> value " << glib::FindKthBigElement(vev, 3) << endl;

    // 验证，提示其他类型数据不能使用计数排序！
    vector<char> vec_char_counting = {'c', 'a'};
    glib::Sort(vec_char_counting, glib::SortOption::COUNTING);

    return 0;
}
