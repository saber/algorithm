/*
 * CopyRight (c) 2019 gcj
 * File: recursion.test.cc
 * Project: algorithm
 * Author: gcj
 * Date: 2019/4/15
 * Description: test some recursion examples
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#include <iostream>
#include "../utils/types.h" // 包含了基本类型的别名
#include "./recursion.hpp"

using namespace std;

//! \brief  对递归用例简单测试
//! \run
//!     g++ recursion.test.cc -std=c++11 && ./a.out
int main(int argc, char const *argv[]) {
    cout << "台阶问题求解：" << glib::ClimbStairs(7) << endl;
    cout << "电影院排求解：" << glib::WhichRow(5) << endl;

    // 有环情况测试
    cout << "最终推荐人有环测试" << endl;
    glib::KeyMap<char, char> record_table = { {'A', 'B'},
                                              {'B', 'C'},
                                              {'C', 'D'},
                                              {'D', 'A'} };
    for (const auto& m: record_table)
        cout << "record_table: " << m.first << " " << m.second << endl;
    cout << "最终推荐人: " << glib::FinalRecommender(record_table, 'A') << endl;

    return 0;
}
