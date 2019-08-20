/*
 * CopyRight (c) 2019 gcj
 * File: binary_search_tree.test.cc
 * Project: algorithm
 * Author: gcj
 * Date: 2019/4/8
 * Description: using stack test
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#include "binary_search_tree.hpp"
#include <iostream>
using namespace std;

//! \brief 测试二叉搜索树核心算法
//! \run
//!     g++ binary_search_tree.test.cc -std=c++11 && ./a.out

int main(int argc, char const *argv[]) {
    // 二叉查找树的构建及插入测试
    cout << "二叉查找树插入测试" << endl;
    glib::BinarySearchTree<int> tree{33, 16, 50, 13, 18, 34, 58, 15, 17, 25, 51, 66, 19, 27, 55, 56};

    // glib::BinarySearchTree<int> tree{1, 2};
    // glib::BinarySearchTree<int> tree{13, 10, 16, 9, 11, 14};
    cout << endl;

    // 测试二叉查找树三种遍历方式
    cout << "二叉查找树四种遍历方式" << endl;
    cout << "前序遍历：";
    tree.print_by_front_order();
    cout << endl;
    cout << "中序遍历：";
    tree.print_by_middle_order();
    cout << endl;
    cout << "后序遍历：";
    tree.print_by_back_order();
    cout << endl;
    cout << "层序遍历：";
    tree.print_by_level_order();
    cout << endl;

    // 测试二叉查找树查询
    cout << "二叉查找树查询测试" << endl;
    cout << "当前已有数据中序顺序为（从小到大输出）：";
    tree.print_by_middle_order();
    cout << endl;
    cout << "请输入要查找的数据：";
    int element_inquired;
    cin >> element_inquired;
    if (tree.Find(element_inquired) != nullptr)
        cout << "内部含有被查询的数据！" << endl;
    else
        cout << "内部不含被查询的数据" << endl;

    // 测试树的高度
    cout << "测试树的高度" << endl;
    cout << "树的高度（递归方法）：" << tree.TreeHeight(glib::internal::TreeHeightOption::RECURSIVE) << endl;
    cout << "树的高度（非递归方法）：" << tree.TreeHeight() << endl;

    // 测试树的删除操作
    // while (1) {
        cout << "测试树的删除操作" << endl;
        cout << "请输入要删除的值：";
        int element_deleted;
        cin >> element_deleted;
        tree.Delete(element_deleted);
        cout << "删除后，中序遍历结果：";
        tree.print_by_level_order();
        cout << endl;
    // }

    return 0;
}
