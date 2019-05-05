/*
 * CopyRight (c) 2019 gcj
 * File: array.test.cc
 * Project: algorithm
 * Author: gcj
 * Date: 2019/4/8
 * Description: simple test for using self-make dynamic array
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#include "array.hpp"
#include <iostream>
#include <string>
using namespace std;

//! \brief 数组类型的基本测试
//! \run
//!     g++ array.test.cc -std=c++11 && ./a.out
int main(int argc, char const *argv[]) {
    // 基本类型测试
    cout << "基本类型测试" << endl;
    glib::Array<int> array;
    for (size_t i = 0; i < array.size(); i++) {
        // array.PushBack(i);
        array[i] = i;
    }
    array.PushBack(5);
    array.PushBack(6);
    array.PushBack(7);

    cout << "数组有效数据: " << array.size() << endl;
    cout << "数组容量: " << array.capacity() << endl;
    for (size_t i = 0; i < array.size(); i++) {
        cout << "array[i]: " << array[i] << endl;
    } // 输出 0 5 6 7
    cout << endl;

    // string 类型测试
    cout << "string 类型测试" << endl;
    glib::Array<string> array_string(5);
    for (size_t i = 0; i < array_string.size(); i++) {
        array_string[i] = "st";
    }
    array_string.PushBack("st2");
    cout << "print" << endl;
    for (size_t i = 0; i < array_string.size(); i++) {
        cout << array_string[i] << " ";
    } // 输出 st st st st st st2
    cout << endl;
    cout << endl;

    // 测试合并两个有序数组
    cout << "测试合并两个有序数组" << endl;
    glib::Array<int> array1(7);
    glib::Array<int> array2(9);
    for (size_t i = 0; i < 7; i++) {
        array1[i] = 7 - i;
    } // 7 6 5 4 3 2 1
    for (size_t i = 0; i < 9; i++) {
        array2[i] = 9 - i;
    }// 9 8 7 6 5 4 3 2 1
    // 先排序
    array1.Sort();
    array2.Sort();

    auto array_merge = glib::Merge(array1, array2);
    for (size_t i = 0; i < array_merge.size(); i++) {
        cout << array_merge[i] << " ";
    } // 1 1 2 2 3 3 4 4 5 5 6 6 7 7 8 9
    cout << endl;

    return 0;
}
