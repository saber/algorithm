/*
 * File: use_sort.cc
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

int main(int argc, char const *argv[]) {
    // 随机生成一个大的数列，测试下面排序算法效率
    srand((int)time(0));
    size_t n = 100000;
    vector<int> vec_bubble(n);
    vector<int> vec_insert(n);
    vector<int> vec_selection(n);
    for (size_t i = 0; i < n; i++) {
        size_t re = Random(n);
        vec_bubble[i] = re;
        vec_insert[i] = re;
        vec_selection[i] = re;
    }

    // 冒泡排序测试
    // std::vector<int> vec_bubble = {6, 5, 4, 3, 2, 1};
    auto start_bubble = chrono::system_clock::now();
    glib::Sort<int>()(vec_bubble);
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
    glib::Sort<int>()(vec_insert, glib::Sort<int>::Option::kInsertion);
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
    glib::Sort<int>()(vec_selection, glib::Sort<int>::Option::kSelection);
    auto end_selection = chrono::system_clock::now();
    chrono::duration<double> elaspsed_seconds_selection = end_selection - start_selection;
    cout << " selection elaspsed_seconds: " << elaspsed_seconds_selection.count() << endl;
    // for (auto &p: vec_selection)
    //     cout << " " << p;

    // 验证三者的排序结果是否一致！进而检验排序算法是否正确
    for (size_t i = 0; i < n; i++) {
        if (vec_bubble[i] != vec_insert[i])
            cout << "insert sort error" << endl;
        if (vec_bubble[i] != vec_selection[i])
            cout << "selection sort error" << endl;
    }

    return 0;
}
