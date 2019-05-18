/*
 * CopyRight (c) 2019 gcj
 * File: skip_list.test.cc
 * Project: algorithm
 * Author: gcj
 * Date: 2019/5/15
 * Description: skip list simple test
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */
#include "skip_list.hpp"
#include <iostream>
using namespace std;

//! \brief 跳表功能的简单测试
//! \run
//!     g++ skip_list.test.cc -std=c++11 && ./a.out
int main(int argc, char const *argv[]) {
    {
    glib::SkipList<int> skip_list{5, 4, 3, 2, 1};

    // 测试插入
    cout << "测试插入" << endl;
    skip_list.Insert(9);
    skip_list.print_value();
    cout << endl;

    // 测试查找
    cout << "测试查找" << endl;
    cout << skip_list.Find(3)->data << endl;
    cout << endl;

    // 测试删除
    cout << "测试删除" << endl;
    skip_list.Delete(6);
    skip_list.print_value();
    skip_list.Delete(5);
    skip_list.print_value();
    cout << endl;
    }

    // 测试跳表超出作用域后，释放内部节点
    cout << "退出" << endl;
    return 0;
}
