/*
 * CopyRight (c) 2019 gcj
 * File: hash_table.test.cc
 * Project: algorithm
 * Author: gcj
 * Date: 2019/5/30
 * Description: hash_table simple test
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#include "./hash_table.hpp"
#include <string>
#include <iostream>
using namespace std;

//! \brief LRU 哈希表 + 双链表实现，简单测试
//! \run
//!     g++ hash_table.test.cc -std=c++11 && ./a.out
int main(int argc, char const *argv[]) {
    glib::HashTable<string, string> hash_table;

    // 验证插入是否正确
    cout << "验证 Insert 插入函数" << endl;
    // hash_table.Insert(std::make_pair("1", "n"));
    hash_table.Insert("1", "n");
    hash_table.Insert(std::make_pair("2", "c"));
    hash_table.Insert(std::make_pair("3", "u"));
    hash_table.Insert(std::make_pair("4", "g"));
    hash_table.Insert(std::make_pair("5", "c"));
    hash_table.Insert(std::make_pair("6", "j"));
    hash_table.print_value();
    hash_table.Insert(std::make_pair("5", "G"));

    // 验证扩容
    cout << "验证扩容" << endl;
    cout << "扩容前，总容量：" << hash_table.capacity() << endl; // 8
    cout << "扩容前，当前容量：" << hash_table.size() << endl; // 6
    hash_table.Insert(std::make_pair("7", "o")); // 此时会扩容
    cout << "扩容后，总容量：" << hash_table.capacity() << endl; // 16
    cout << "扩容后，当前容量：" << hash_table.size() << endl; // 7
    hash_table.print_value();
    cout << endl;

    // 验证查找函数
    cout << "验证查找函数" << endl;
    auto return_result = hash_table.Find("5");
    if (return_result.first) {
        cout << "找到目标: " << return_result.second << endl;
    }
    cout << endl;

    // 验证删除函数
    cout << "验证删除函数" << endl;
    cout << "删除前，总容量：" << hash_table.capacity() << endl;
    cout << "删除前，当前容量：" << hash_table.size() << endl;

    hash_table.Delete("5");
    cout << "删除一个元素后，当前容量：" << hash_table.size() << endl;
    hash_table.print_value();
    cout << endl;

    // 验证缩容
    cout << "验证缩容" << endl;
    cout << "缩容前，总容量：" << hash_table.capacity() << endl; // 16
    cout << "扩容前，当前容量：" << hash_table.size() << endl; // 6
    hash_table.Delete("7");
    hash_table.Delete("6");
    hash_table.Delete("5");
    hash_table.Delete("4");
    hash_table.Delete("3");
    cout << "缩容后，总容量: " << hash_table.capacity() << endl; // 8
    cout << "缩容后，当前容量：" << hash_table.size() << endl; // 2
    hash_table.print_value();

    // 验证修改数据内容
    cout << "例子：如何修改 key 对应数据" << endl;
    auto result = hash_table.Find("1");
    if (result.first) {
        hash_table.Insert("1", "N");
    }
    hash_table.print_value();

    return 0;
}
