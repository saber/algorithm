/*
 * CopyRight (c) 2019 gcj
 * File: backtracking.test.cc
 * Project: algorithm
 * Author: gcj
 * Date: 2019/9/17
 * Description: test Backtracking algorithm
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#include "backtracking.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility> // std::pair
#include <chrono>  // time
using namespace std;

//! \brief 简单测试回溯算法（8 皇后问题、0-1 背包问题、不同路径问题 III）
//! \run
//!     g++ backtracking.test.cc -std=c++11 && ./a.out

int main(int argc, char const *argv[]) {
    cout << "8 皇后问题" << endl;
    const int N = 4;
    cout << glib::EightQueens(N) << endl; // 2

    // 0-1 背包问题
    cout << "测试 0-1 背包最大重量问题" << endl;
    vector<int> backpack{1, 3, 5, 7, 4};
    int max_capacity = 18;
    cout << glib::ZeroOrOneBackpack(backpack, max_capacity) << endl;

    // o-1 背包 给定重量的前提下，最大价值量问题
    // 测试数据链接：https://github.com/skyerhxx/Test-Data-of-01-knapsack-problem-/tree/master/%E6%B5%8B%E8%AF%95%E6%95%B0%E6%8D%AE
    cout << "测试 0-1 背包最大价值问题" << endl;
    vector<pair<int ,int>> backpack1{ // 30 组数据运行时间就在 10 s 左右了
        {71, 26}, {34, 59}, {82, 30}, {23, 19}, {1,  66}, {88, 85}, {12, 94},
        {57, 8 }, {10, 3 }, {68, 44}, {5,  5 }, {33, 1 }, {37, 41}, {69, 82},
        {98, 76}, {24, 1 }, {26, 12}, {83, 81}, {16, 73}, {26, 32}, {18, 74},
        {43, 54}, {52, 62}, {71, 41}, {22, 19}, {65, 10}, {68, 65}, {8,  53},
        {40, 56}, {40, 53}, {24, 70}, {72, 66}, {16, 58}, {34, 22}, {10, 72},
        {19, 33}, {28, 96}, {13, 88}, {34, 68}, {98, 45}, {29, 44}, {31, 61},
        {79, 78}, {33, 78}, {60, 6 }, {74, 66}, {44, 11}, {56, 59}, {54, 83},
        {17, 48}}; // 这 50 组数据最大价值量为 1063，但是自己测试没有运行出来
    vector<pair<int, int>> backpack2{
        {77, 92}, {22, 22}, {29, 87}, {50, 46}, {99, 90}
    };
    vector<pair<int, int>> backpack3{
        {95, 89}, {75, 59}, {23, 19}, {73, 43}, {50, 100},
        {22, 72}, {6 , 44}, {57, 16}, {89, 7 }, {98, 64}
    };
    // int max_capacity1 = 300;
    int max_capacity2 = 100;
    int max_capacity3 = 300;

    // cout << glib::ZeroOrOneBackpackValue(backpack1, max_capacity1) << endl; // 1063 回溯法运行不出来

    cout << glib::ZeroOrOneBackpackValue(backpack2, max_capacity2) << endl; // 133
    cout << glib::ZeroOrOneBackpackValue(backpack3, max_capacity3) << endl; // 388

    // 网格是否能走
    cout << "grid path problem" << endl;
    // vector<vector<int>> grids{{1, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 2, -1}}; // 2
    vector<vector<int>> grids{{1, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 2}}; // 4
    // vector<vector<int>> grids{{0, 1}, {2, 0}}; // 0
    cout << glib::GridPath(grids) << endl;

    return 0;
}
