/*
 * CopyRight (c) 2019 gcj
 * File: lru_hash.test.cc
 * Project: algorithm
 * Author: gcj
 * Date: 2019/5/30
 * Description: simple test lru by hash talbe
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */
 #include "./lru_hash.hpp"
 #include <iostream>
 #include <string>
 #include <stdlib.h> // sprintf

 using namespace std;

 //! \brief LRU 哈希表 + 双链表实现，简单测试
 //! \run
 //!     g++ lru_hash.test.cc -std=c++11 && ./a.out
 int main(int argc, char const *argv[]) {
     // 对于 int 类型已经测试过了，正常，所以这里保留了 string 类型的测试结果
     glib::LruHash<string> lru(4);
     cout << "哈希容量：" << lru.hash_capacity() << endl;

     // 测试插入
     cout << "测试插入" << endl;
     lru.Insert("1");
     lru.Insert("1");
     lru.print_value(); // 1
     cout << lru.hash_capacity() << endl; // 8
     cout << lru.capacity() <<endl; // 4
     cout << lru.size() << endl; // 1

     lru.Insert("2");
     lru.Insert("3");
     lru.print_value(); // 3 2 1
     cout << lru.hash_capacity() << endl; // 8
     cout << lru.capacity() <<endl; // 4
     cout << lru.size() << endl; // 3
     // lru.debug_print_value();

     lru.Insert("4");
     lru.print_value(); // 4 3 2 1
     lru.Insert("5"); // delete
     lru.print_value(); // 5 4 3 2

     lru.Insert("1");
     lru.print_value(); // 1 5 4 3
     lru.Insert("4");
     lru.print_value(); // 4 1 5 3
     // lru.debug_print_value();

     // 测试查询
     cout << "测试查询" << endl;
     const auto lru_find = lru.Find("1");
     if (lru_find.first) {
         cout << "找到：" << lru_find.second << endl;
     }
     cout << endl;

     // 测试删除
     cout << "测试删除" << endl;
     lru.Delete("5");
     lru.Delete("2");
     lru.print_value(); // 4 1 3
     cout << lru.hash_capacity() << endl; // 8
     cout << lru.capacity() <<endl; // 4
     cout << lru.size() << endl; // 3
     // lru.debug_print_value();
     cout << endl;

     // 测试调节容量
     cout << "测试调节容量" << endl;
     cout << "测试扩容" << endl;
     lru.set_size(13);
     lru.print_value(); // 4 1 3
     cout << "哈希容量：" << lru.hash_capacity() << endl;
     int num = 0;
     for (int i = 0; i < 10; i++) {
         num = i+1;
         char c[1];
         sprintf(c, "%d", i);
         std::string cc = c;
         lru.Insert(cc);
     }
     lru.debug_print_value(); // 10 9 8 7 6 5 4 3 2 1

     cout << "测试缩减容量" << endl;
     cout << lru.size() << endl; // 10
     lru.set_size(4);
     cout << lru.size() << endl; // 4
     lru.print_value(); // 9 8 7 6

     // lru.debug_print_value();
     // lru.print_value_by_hash();

     return 0;
 }
