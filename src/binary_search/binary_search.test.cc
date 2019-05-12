/*
 * CopyRight (c) 2019 gcj
 * File: binary_search.test.cc
 * Project: algorithm
 * Author: gcj
 * Date: 2019/5/8
 * Description: use and test binary_search function
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#include "./binary_search.hpp"
#include <iostream>
#include <vector>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <typeinfo>

using namespace std;

//! \brief 简单测试二分搜索功能{二分搜索各种功能、求解某个值的根、循环数组二分求解}
//! \run
//!     g++ binary_search.test.cc -std=c++11 && ./a.out
int main(int argc, char const *argv[]) {
    // 二分搜索非递归测试
    cout << "二分搜索非递归测试" << endl;
    vector<int> binary_search_array = {2, 2, 2, 2, 5, 6, 7, 8, 9, 10, 10, 10};
    const auto nonrecur_iter = glib::BinarySearch(binary_search_array, 4,
                glib::BinarySearchOption::NON_RECURSION_FIRST_GREATER_EQUAL);
    if (nonrecur_iter != binary_search_array.end()) {
        std::vector<int>::const_iterator const_iter = binary_search_array.begin();
        cout << "find the index in array: " << distance(const_iter, nonrecur_iter) << endl;
        cout << "value: " << *nonrecur_iter << endl;
    } else {
        cout << "Not found this value" << endl;
    }
    cout << endl;

    // 二分搜索递归测试—— 对于重复数据元素目前还不适用，仅仅测试一下功能
    cout << "二分搜索递归测试" << endl;
    vector<int> binary_search_array1 = {2, 2, 2, 2, 5, 6, 7, 8, 9, 10, 10, 10};
    const auto recursion_iter = glib::BinarySearch(binary_search_array1, 6, glib::BinarySearchOption::RECURSION);
    if (recursion_iter != binary_search_array1.end()) {
        cout << "find the value in array: " << *recursion_iter << endl;
    } else {
        cout << "Not found this value" << endl;
    }
    cout << endl;

    // 测试利用二分查找思路，求取一个值的平方根
    cout << "测试利用二分查找思路，求取一个值的平方根" << endl;
    cout << "测试二分思路" << endl;
    auto start_sqrt_binary_search = chrono::system_clock::now();
    cout<< setiosflags(ios::fixed) << setprecision(7)
        << glib::Sqrt(30, 4, 6, glib::binary_search_app::SqrtMethod::NEWTON) << endl;
    auto end_sqrt_binary_search = chrono::system_clock::now();
    chrono::duration<double> elaspsed_seconds_binary_search =
                             end_sqrt_binary_search - start_sqrt_binary_search;
    cout << " binary_search sqrt time: " << elaspsed_seconds_binary_search.count() << endl;
    cout << endl;

    cout << "测试 Newton 思路" << endl;
    auto start_sqrt_newton = chrono::system_clock::now();
    cout<< setiosflags(ios::fixed) << setprecision(7)
        << glib::Sqrt(30, 4, 6, glib::binary_search_app::SqrtMethod::NEWTON) << endl;
    auto end_sqrt_newton = chrono::system_clock::now();
    chrono::duration<double> elaspsed_seconds_newton =
                             end_sqrt_newton - start_sqrt_newton;
    cout << " newton sqrt time: " << elaspsed_seconds_newton.count() << endl;
    cout << endl;

    // 测试循环有序数组
    cout << "测试循环有序数组" << endl;
    vector<int> rotated_array = {1, 2, 3, 4, 5, 6};
    cout << "索引: " << glib::SearchRotatedSortedArray(rotated_array, 3) << endl; // pivot:-1 索引 2
    vector<int> rotated_array1 = {2, 3, 4, 5, 6, 1};
    cout << "索引: " << glib::SearchRotatedSortedArray(rotated_array1, 3) << endl; // pivot:4 索引：1
    vector<int> rotated_array2 = {5, 6, 1, 2, 3, 4};
    cout << "索引: " << glib::SearchRotatedSortedArray(rotated_array2, 6) << endl; // pivot:1 索引：1
    vector<int> rotated_array3 = {4, 5, 6, 1, 2, 3};
    cout << "索引: " << glib::SearchRotatedSortedArray(rotated_array3, 2) << endl; // pivot: 2 索引：4
    vector<int> rotated_array4 = {6, 1, 2, 3, 4, 5};
    cout << "索引: " << glib::SearchRotatedSortedArray(rotated_array4, 6) << endl; // 索引：0

    return 0;
}
