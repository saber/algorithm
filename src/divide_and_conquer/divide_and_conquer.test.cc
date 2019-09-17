/*
 * CopyRight (c) 2019 gcj
 * File:divide_and_conquer.test.cc
 * Project: algorithm
 * Author: gcj
 * Date: 2019/9/15
 * Description: test divide and conquer algorithm
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#include "./divide_and_conquer.hpp"
#include <iostream>
#include <vector>
#include <deque>
#include <list>
using namespace std;

//! \brief 简单测试分治算法求取数据的逆序对数，以及归并排序
//! \run
//!     g++ divide_and_conquer.test.cc -std=c++11 && ./a.out
//! \note 这里只能用随机获取迭代器类型，也就是只能使用 vector deque array
int main(int argc, char const *argv[]) {
    // vector test
    cout << "测试 vector 是否成立" << endl;
    vector<int> vec{2, 4, 3, 1, 5, 6};
    int count = 0;
    glib::MergeSortCount(vec.begin(), vec.end(), count, std::greater_equal<int>()); // 此时求解的为正续对数，非逆序对
    cout << "count: " << count << endl;
    for (auto ele: vec)
        cout << ele << " ";

    // test
    cout << endl << "测试 deque 是否成立" << endl;
    deque<int> deq{2, 4, 3, 1, 5, 6};
    int count2 = 0;
    glib::MergeSortCount(deq.begin(), deq.end(), count2);
    cout << "count: " << count2 << endl;
    for (auto ele: deq)
        cout << ele << " ";

    // test arr
    cout << endl << "测试数组归并排序" << endl;
    int arr[6] = {2, 4, 3, 1, 5, 6};
    glib::MergeSort(arr, arr + 6);
    for (auto ele: arr)
        cout << ele << " ";

    // list queue not usefull
    // list<int> lis{2, 4, 3, 1, 5, 6};
    // int count1 = 0;
    // glib::MergeSort(lis.begin(), lis.end(), count1);
    // cout << "count: " << count1 << endl;
    // for (auto ele: lis)
    //     cout << ele << " ";

    return 0;
}
