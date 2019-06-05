/*
 * CopyRight (c) 2019 gcj
 * File: use_map.cc
 * Project: algorithm
 * Author: gcj
 * Date: 2019/4/15
 * Description: use map
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#include <map>
#include <iostream>
#include <string>
using namespace std;

// 通用类型声明
template <typename _Key, typename _Value>
using KeyMap = std::map<_Key, _Value>;

//! \brief 简单学习使用标准库 std::map
//!    map 的功能
//!     1）自动建立Key － value的对应。key 和 value可以是任意你需要的类型。
//!     2）快速插入Key - Value 记录。--- inset()
//!     3) 根据key值快速查找记录，查找的复杂度基本是Log(N)，如果有1000个记录，最多查找10次，
//!        1,000,000个记录，最多查找20次。 --- find()
//!     4）遍历所有记录。--- for(auto &value: xxx) xxx;
//!     5）根据Key 修改value记录。TODO
//!     6）快速删除记录 --- erase() TODO
//!
//! \run
//!     g++ use_hashmap.cc -std=c++11 && ./a.out
//! \TODO
//!     1）完成上面第 5）点
//!     2）完成上面第 6）点
//! \TODO Reference
//!     1）C++ Map常见用法说明
int main(int argc, char const *argv[]) {
    KeyMap<int, string> key_map;
    // 1）插入元素
        // 低效插入方式
    key_map[0] = "One";
    key_map[1] = "Two"; // 先查找有没有键值 1，没有的话先构造一个，然后把 "Two" 拷贝进去

        // 2）高效插入，通过移动构造函数
    key_map.insert(KeyMap<int, string>::value_type(2, "three"));

    // 3）查询元素 auto iter = find(_Key); 返回一个迭代器。 可以使用 iter.first iter.second
    if (key_map.find(1) != key_map.end()) { // 表示查找到，且 find() 返回一个迭代器，类型是 std::pair
        cout << "查找到的元素: " << key_map[1];
    } else {
        // 处理没有查找到的情况！
    }

    // 4）遍历
    for (auto &value: key_map) {
        cout << "key: " << value.first << " value: " << value.second << endl;
    }

    // 5）修改记录


    // 4）删除元素： erase(iterator it); erase(iterator first, iterator last);
    //          erase(const _Key& key); 通过关键字删除 clear(); 相当于删除所有元素！

    return 0;
}
