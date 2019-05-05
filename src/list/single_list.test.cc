/*
 * CopyRight (c) 2019 gcj
 * File: single_list.test.cc
 * Project: algorithm
 * Author: gcj
 * Date: 2019/4/8
 * Description: test single linked list
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#include "single_list.hpp"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

//! \brief 简单测试单链表结构的所有功能
//! \run
//!     g++ single_list.test.cc -std=c++11 && ./a.out
//! \TODO
//!
int main(int argc, char **argv) {
    // 简单测试 string 类是否能够通过案例
    cout << "简单测试 string 类是否能够通过案例" << endl;
    glib::SingleList<string> string_list;
    string_list.InsertHead(string(1,'a'));
    string_list.InsertHead(string(1,'b'));
    string_list.print_value(); // b a
    string_list.DeleteTail();
    string_list.print_value(); // b

    vector<int> values;
    for (int i = 0; i < 5; i++) {
        values.emplace_back(i);
    }
    cout << "before values.size(): " << values.size() << endl; // 5
    glib::SingleList<int> single_list(values.size(), values);
    cout << endl;

    // 打印初始链表数据测试
    cout << "打印初始链表数据测试" << endl;
    cout << "print single list" << endl;
    single_list.print_value(); // 0 1 2 3 4
    cout << endl;

    // 删除单个数据测试
    cout << "删除单个数据测试" << endl;
    single_list.Delete(4);
    cout << "after delete" << endl;
    single_list.print_value(); // 0 1 2 3
    cout << endl;

    // 头部插入数据测试
    cout << "头部插入数据测试" << endl;
    single_list.InsertHead(6);
    cout << "after InsertHead()" << endl;
    single_list.print_value(); // 6 0 1 2 3
    cout << endl;

    // 尾部插入数据测试
    cout << "尾部插入数据测试" << endl;
    single_list.InsertTail(8);
    single_list.InsertTail(1);
    cout << "after InsertTail()" << endl;
    single_list.print_value(); // 6 0 1 2 3 8 1
    cout << endl;

    // 交换数据测试、查找数据测试
    cout << "交换数据测试、查找数据测试！" << endl;
    if (!single_list.Find(nullptr) && !single_list.Find(single_list.head_ptr()))
        cout << "false" << endl;
    if (single_list.Find(single_list.first_node_ptr()))
        cout << "yes" << endl;
    const auto vec_nodes = single_list.Find(1);
    for (size_t i = 0; i < vec_nodes.size(); i++) {
        cout << "vec_nodes[i]: " << vec_nodes[i]->data << endl;
        if (vec_nodes[i] == single_list.tail_ptr())
            cout << "尾部指针success!" << endl;
    }
    cout << "after exchange!" << endl;
    single_list.Exchange(single_list.first_node_ptr(), single_list.first_node_ptr()->next);
    single_list.print_value(); // 0 6 1 2 3 8 1
    cout << "after exchange,node_count: " << single_list.node_count() << "tail_->data: "
         << single_list.tail_ptr()->data << endl; // 7 1
    single_list.Exchange(single_list.tail_ptr(), single_list.first_node_ptr()->next->next->next);
    single_list.print_value(); // 0 6 1 1 3 8 2
    cout << "after exchange,node_count: " << single_list.node_count() << "tail_->data: "
         << single_list.tail_ptr()->data << endl; // 7 2
    cout << endl;

    // 移位数据测试
    cout << "移位数据测试！" << endl;
    single_list.MoveHead(single_list.tail_ptr());
    single_list.MoveHead(single_list.first_node_ptr());
    single_list.MoveHead(single_list.first_node_ptr()->next->next);
    cout << "after MoveHead" << endl;
    single_list.print_value(); // 6 2 0 1 1 3 8
    cout << "tail_ptr->data: " << single_list.tail_ptr()->data << endl; // 8
    cout << endl;

    // 测试尾部删除、头部删除、普通删除
    cout << "测试尾部删除、头部删除、普通删除" << endl << "before dele" << endl;
    single_list.DeleteHead();
    single_list.print_value(); // 2 0 1 1 3 8
    single_list.DeleteTail();
    single_list.print_value(); // 2 0 1 1 3
    cout << "tail data: " << single_list.tail_ptr()->data << endl; // 3
    single_list.Delete(single_list.head_ptr()->next->next->next->next->next);
    cout << "tail data: " << single_list.tail_ptr()->data << endl; // 1
    single_list.print_value(); // 2 0 1 1
    cout << endl;


    // 测试删除重复数据,一些边界条件！！！
    // 普通插入数据
    cout << "测试删除重复数据,一些边界条件" << endl;
    cout << "common insert test!" << endl;
    single_list.Insert(100);
    single_list.print_value(); // 100 2 0 11
    single_list.Insert(101,10);
    single_list.print_value(); // 101 100 2 0 1 1
    cout << endl;

    // 建立空节点链表测试打印，默认有一个头结点
    cout << "测试空链表打印" << endl;
    glib::SingleList<int> lists;
    lists.print_value(); // 空值
    cout << endl;

    // 输入重复数据测试
    cout << "测试输入重复数据!";
    cout << endl << "after init" << endl;
    for (size_t i = 0; i < 2; i++)
        lists.InsertHead(1);
    for (size_t i = 0; i < 2; i++)
        lists.InsertHead(2);
    lists.print_value(); // 2 2 1 1

    lists.Delete(2);
    cout << "after default delete " << endl;
    lists.print_value(); // 1 1
    lists.Delete(1, true);
    cout << "after delete by repeat" << endl;
    lists.print_value(); // 1 1
    cout << endl;

    // 测试拷贝以及赋值函数，引用资源相关！
    cout << "测试拷贝赋值函数--引用!" << endl;
    single_list.print_value(); // 第一个链表节点打印 101 100 2 0 1 1
    lists.print_value(); // 第二个链表节点打印 1
    cout << "1 2 引用计数: "<< single_list.reference_count() << " " << lists.reference_count() << endl; // 1 1

    glib::SingleList<int>single_list_copy = single_list;
    auto lists_copy = lists;
    single_list_copy.print_value(); // 101 100 2 0 1 1
    cout << "after copy ，引用计数: " << single_list_copy.reference_count() << endl; // 2

    lists = single_list_copy;
    lists.print_value(); // 101 100 2 0 1 1
    cout << "operator引用计数:" << lists.reference_count() << endl; // 3
    cout << "lists_copy 引用计数: " << lists_copy.reference_count() << endl; // 1
    cout << endl;

    // 测试单链表实现 lru ，默认类型 int
    cout << "测试单链表实现 lru" << endl;
    glib::SingleList<> lru_list;
    int data = 0;
    for (size_t i = 0; i < 15; i++) {
        if (i > 9 && i <= 12)
            data = 15 - i;
        else if (i > 12)
            data = 0;
        else
            data = i;
        glib::single_list_app::LRUBySingleList(lru_list, int(data));
        lru_list.print_value();
        cout << "tail data->: " << lru_list.tail_ptr()->data << " ";
    }
    cout << endl;
    cout << endl;

    // 单链表翻转测试，以及求取中间节点测试
    cout << "测试翻转单链表以及求取中间节点测试" << endl;
    cout << "Reserve single list test!" << endl;
    vector<int> valuess;
    for (int i = 0; i < 5; i++) {
        valuess.emplace_back(i);
    }
    glib::SingleList<int> single_lists(valuess.size(), valuess);
    single_lists.print_value(); // 0 1 2 3 4
    cout << "after reserve!" << endl;
    single_lists.Reserve();
    single_lists.print_value(); // 4 3 2 1 0
    if (single_lists.node_count()) {
        cout << "tail->data: " << single_lists.tail_ptr()->data << endl; // 0
        cout << "中间节点数据： " << single_lists.MiddleNode()->data << endl; // 2
    }
    cout << endl;

    // 测试单链表存储的回文字符串判断---递归方法！
    cout << "单链表检测回文测试---递归方法" << endl;
    glib::SingleList<char> palindrome_string;
    for (size_t i = 0; i < 3; i++) {
        palindrome_string.InsertHead('a' + i);
    }
    for (size_t i = 0; i < 2; i++) {
        palindrome_string.InsertTail('b' + i);
    }
    palindrome_string.print_value();
    if (glib::single_list_app::PalingromeString(palindrome_string))
        cout << "yes" << endl;
    else
        cout << "no" << endl;

    return 0;
}
