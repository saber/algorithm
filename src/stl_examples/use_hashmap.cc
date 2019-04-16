/*
 * File: use_hashmap.cc
 * Project: algorithm
 * Author: gcj
 * Date: 2019/4/15
 * Description: use hash_map
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

// #include <hash_map> // 这种方式已经不用了！
#include <unordered_map> // 这个就是哈希表
#include <iostream>
#include <string>
#include <map>
using namespace std;

// 使用方法参考：C++ 中标准库 map 和 hash_map 的使用方法--->https://blog.csdn.net/qq_26399665/article/details/52295380
// C++ STL中哈希表 hash_map介绍--->https://blog.csdn.net/ddkxddkx/article/details/6555754#t4
template <typename _Key, typename _Value>
using KeyMap = std::unordered_map<_Key, _Value>;

int main(int argc, char const *argv[]) {
    KeyMap<int, string> hashs;
    hashs[0] = "唐伯虎";
    hashs[1] = "sans";
    if (hashs.find(1) != hashs.end()) {
        cout << "hashs: " << hashs[1] << endl;
    }

    return 0;
}
