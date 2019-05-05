/*
 * CopyRight (c) 2019 gcj
 * File: recursion.hpp
 * Project: algorithm
 * Author: gcj
 * Date: 2019/4/15
 * Description: some recursion examples
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#ifndef GLIB_RECURSION_HPP_
#define GLIB_RECURSION_HPP_
#include <iostream>
#include "../utils/types.h" // 包含了基本类型的别名

//! \brief 简单完成了 3 个递归实例。电影院问题、爬楼梯问题、最终推荐人问题
//!         对应函数：WhichRow、ClimbStairs、FinalRecommender
//!
//! \Note
//!     1）下面 2 个递归实例：电影院问题和爬楼梯问题，都有相应的循环版本，
//!         可通过定义或者取消 RECURSIVE 宏，实现运行相应版本。
//!     2）代码中考虑了深度问题，以及重复元素问题（哈希表存储值）
//!
//! TODO
//!     1）调试递归代码
//!     2）编程实现一组数据集合的全排列
//!     3）Leetcode 题目查找
//!     4）最终推荐人非递归实现

namespace glib {

#define RECURSIVE // 表示后面的算法是否使用递归方法，否则将递归改为循环！

#ifdef RECURSIVE
#define MAX_DEPTH 50 // 限制递归深度
// constexpr size_t kMAX_DEPTH = 50; // 或者使用这种方式
#endif

//! \description 电影院第几排问题。假设在第 n 排。
//! \note 需要解决递归深度问题。两种方式其一：定义一个深度变量。如果深度大于某个值直接退出
//!       方法二：转换为 for 循环
//! \complexity 递归实现：O(n) 非递归实现：O(n)
//! \method 当前排数 = 前一排数 + 1，协成递推公式： f(n) = f(n-1) + 1，确定终止条件为 f(1) = 1
static size_t CurrentDepth = 0;
int WhichRow(int row) {
#ifdef RECURSIVE
    if (1 == row)
        return 1;
    if (CurrentDepth++ > MAX_DEPTH) { // 可以用 assert() 来替代
        std::cerr << "achive max depth!" << '\n';
        exit(-1);
    }
    return 1 + WhichRow(row - 1);
#else
    int which_row = 1;
    for (size_t i = 1; i < row; i++) {
        which_row = which_row + 1;
    }
    return which_row;
#endif
}

//! \description  n 个台阶问题: 共有 n 个台阶，每步要么走 1 个台阶，要么走 2 个台阶。问走到第 n 个台阶时有多少种情况？
//! \note 需要用一个全局的散列表记录某个值是否计算成功。减少重复计算。并且对于递归方法限制一下递归深度
//! \complexity 没有哈希表存储重复元素时，复杂度为指数阶：近似 O(2^(o.5n))，有哈希表存储时，复杂度量级也近似指数。
//! \method 分析如下：最后一步时可以分解为两个部分。一个部分是还剩一个台阶，另一个部分是还剩两个台阶。
//!           则可以假设 f(n) = f(n-1) + f(n-2) 其中 f() 函数表示对应有几种情况
//!           终止条件： f(1) = 1, f(2) = 2
static glib::HashMap<int, int> StairAndValue; // 保存楼梯以及 f(楼梯)
static size_t ClimbStairsRecursiveDepth = 0;
int ClimbStairs(int stairs_num) {
    assert(stairs_num >= 1);
#ifdef RECURSIVE
    if (1 == stairs_num)
        return 1;
    else if (2 == stairs_num)
        return 2;

    if ( StairAndValue.end() != StairAndValue.find(stairs_num))
        return StairAndValue[stairs_num];
    if (ClimbStairsRecursiveDepth++ > MAX_DEPTH) { // 可以用 assert 来替代
        std::cerr << "achive max recursion depth!" << '\n';
        exit(-1);
    }
    int kinds = ClimbStairs(stairs_num - 1) +  ClimbStairs(stairs_num - 2);
    // StairAndValue.insert(glib::HashMap<int, int>::value_type(stairs_num, kinds));
    StairAndValue.insert({stairs_num, kinds});
    return kinds;
#else
    if (1 == stairs_num)
        return 1;
    else if (2 == stairs_num)
        return 2;

    int preprekinds = 1;
    int prekinds = 2;
    int kinds = 0;
    for (size_t i = 3; i <= stairs_num; i++) {
        kinds = prekinds + preprekinds;
        preprekinds = prekinds;
        prekinds = kinds;
    }
    return kinds;
#endif
}

//! \description 最终推荐人问题。首先要有一个推荐人表，第一栏是当前要查询的 id。第二栏是对应的推荐人
//! \method 直接用递归方式，递推公式： f(id) = f(f(id)); f(id) == null 表示终止条件
//! \param _Type 表示最终推荐人的表达方式，可以是 char、int 等类型
//! \note 需要解决递归深度问题。两种方式其一：定义一个深度变量。如果深度大于某个值直接退出
//!       方法二：转换为 for 循环
//! \complexity 递归：复杂度为 O(n)
static size_t FinalRecommenderRecursiveDepth = 0;
static glib::HashSet<char> CircleTable;
template <typename _Key, typename _Value>
_Value FinalRecommender(const glib::KeyMap<_Key, _Value> &record_table, const _Key inquiry) {
    assert(inquiry != _Key());
    // assert(FinalRecommenderRecursiveDepth++ < MAX_DEPTH);
    if (FinalRecommenderRecursiveDepth++ > MAX_DEPTH) {
        std::cerr << "achive max recursion depth!" << '\n';
        exit(-1);
    }
    const auto &inquiry_record = record_table.find(inquiry);
    if (record_table.end() == inquiry_record)
        return inquiry;
    if (CircleTable.find(inquiry) != CircleTable.end()) { // 说明找到了环，直接退出
        std::cerr << "This recommender table is circled!" << '\n';
        exit(-1);
    }

    CircleTable.insert(inquiry);
    return FinalRecommender(record_table, inquiry_record->second); // 如何这里用 first 就会出现段错误，也就是一直在这里循环
}

} // namespace glib

#endif // GLIB_RECURSION_HPP_
