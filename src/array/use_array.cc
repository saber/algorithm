/*
 * File: use_array.cc
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
int main(int argc, char const *argv[]) {
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
    }

    // TODO string 测试
    // glib::Array<string> array_string;
    // for (size_t i = 0; i < ; i++) {
    //     /* code */
    // }
    return 0;
}
