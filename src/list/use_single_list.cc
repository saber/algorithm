/*
 * File: use_single_list.hpp
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
int main(int argc, char **argv) {
    // 简单测试 string 类是否能够通过案例
    glib::SingleList<string> string_list;
    string_list.InsertHead(string(1,'a'));
    string_list.InsertHead(string(1,'b'));
    string_list.print_value();
    string_list.DeleteTail();
    string_list.print_value();

    vector<int> values;
    for (int i = 0; i < 5; i++) {
        values.emplace_back(i);
    }
    cout << "before values.size(): " << values.size() << endl;
    glib::SingleList<int> single_list(values.size(), values);

    // 打印初始链表数据
    cout << "print single list" << endl;
    single_list.print_value();

    // 删除单个数据测试
    single_list.Delete(4); // 0 1 4
    cout << "after delete" << endl;
    single_list.print_value();

    // 插入数据测试
    single_list.InsertHead(6);
    cout << "after InsertHead()" << endl;
    single_list.print_value();

    // 尾部插入数据
    single_list.InsertTail(8);
    single_list.InsertTail(1);
    cout << "after InsertTail()" << endl;
    single_list.print_value();

    // 交换数据测试、查找数据测试
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
    single_list.print_value();
    cout << "after exchange,node_count: " << single_list.node_count() << "tail_->data: "
         << single_list.tail_ptr()->data << endl;
    single_list.Exchange(single_list.tail_ptr(), single_list.first_node_ptr()->next->next->next);
    single_list.print_value();
    cout << "after exchange,node_count: " << single_list.node_count() << "tail_->data: "
         << single_list.tail_ptr()->data << endl;

    // 移位数据测试
    single_list.MoveHead(single_list.tail_ptr());
    single_list.MoveHead(single_list.first_node_ptr());
    single_list.MoveHead(single_list.first_node_ptr()->next->next);
    cout << "after MoveHead" << endl;
    single_list.print_value();
    cout << "tail_ptr->data: " << single_list.tail_ptr()->data << endl;

    // 测试尾部删除、头部删除、普通删除
    cout << "before dele" << endl;
    single_list.DeleteHead();
    single_list.print_value();
    single_list.DeleteTail();
    single_list.print_value();
    cout << "tail data: " << single_list.tail_ptr()->data << endl;
    single_list.Delete(single_list.head_ptr()->next->next->next->next->next);
    cout << "tail data: " << single_list.tail_ptr()->data << endl;
    single_list.print_value();


    // 测试删除重复数据,一些边界条件！！！
    // 普通插入数据
    cout << "common insert test" << endl;
    single_list.Insert(100);
    single_list.print_value();
    single_list.Insert(101,10);
    single_list.print_value();

    // 建立空节点链表测试，默认有一个头结点
    glib::SingleList<int> lists;
    lists.print_value();

    // 测试输入重复数据
    cout << "after init" << endl;
    for (size_t i = 0; i < 2; i++)
        lists.InsertHead(1);
    for (size_t i = 0; i < 2; i++)
        lists.InsertHead(2);
    lists.print_value();

    lists.Delete(2);
    cout << "after default delete " << endl;
    lists.print_value();
    lists.Delete(1, true);
    cout << "after delete by repeat" << endl;
    lists.print_value();

    // 测试拷贝以及赋值函数，引用资源相关！
    cout << "测试拷贝赋值函数--引用!" << endl;
    single_list.print_value(); // 第一个链表节点打印
    lists.print_value(); // 第二个链表节点打印
    cout << "1 2 引用计数: "<< single_list.reference_count() << " " << lists.reference_count() << endl; // 1 1

    glib::SingleList<int>single_list_copy = single_list;
    auto lists_copy = lists;
    single_list_copy.print_value();
    cout << "after copy ，引用计数: " << single_list_copy.reference_count() << endl; //2

    lists = single_list_copy;
    lists.print_value();
    cout << "operator引用计数:" << lists.reference_count() << endl; // 3
    cout << "lists_copy 引用计数: " << lists_copy.reference_count() << endl; // 1

    // 测试默认类型 int
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
        LRUBySingleList(lru_list, int(data));
        lru_list.print_value();
        cout << "tail data->: " << lru_list.tail_ptr()->data << " ";
    }

    // 单链表翻转测试，以及求取中间节点测试
    cout << "Reserve single list test!" << endl;
    vector<int> valuess;
    for (int i = 0; i < 5; i++) {
        valuess.emplace_back(i);
    }
    glib::SingleList<int> single_lists(valuess.size(), valuess);
    single_lists.print_value();
    cout << "after reserve!" << endl;
    single_lists.Reserve();
    single_lists.print_value();
    if (single_lists.node_count()) {
        cout << "tail->data: " << single_lists.tail_ptr()->data << endl;
        cout << "中间节点数据： " << single_lists.MiddleNode()->data << endl;
    }

    // 测试单链表存储的回文字符串判断---递归方法！
    glib::SingleList<char> palindrome_string;
    for (size_t i = 0; i < 3; i++) {
        palindrome_string.InsertHead('a' + i);
    }
    for (size_t i = 0; i < 2; i++) {
        palindrome_string.InsertTail('b' + i);
    }
    palindrome_string.print_value();
    if (PalingromeString(palindrome_string))
        cout << "yes" << endl;
    else
        cout << "no" << endl;

    return 0;
}
