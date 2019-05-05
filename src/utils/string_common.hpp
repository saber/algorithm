/*
 * CopyRight (c) 2019 gcj
 * File: string_common.hpp
 * Project: algorithm
 * Author: gcj
 * Date: 2019/4/8
 * Description: manipulate string method
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#ifndef GLIB_STRING_COMMON_HPP_
#define GLIB_STRING_COMMON_HPP_
#include <string>
#include <vector>
#include <iostream>

//! \brief 一些自定义处理字符串函数库，将字符串和 vector 进行转换
//!      基本功能
//!         1）字符串按照符号分割、将由纯数字构成的字符串容器转换成数值类型,
//!           可用符号集：',' '.' ' ', '-' 等等各种符号
//!                 对应函数：Split、StringsToInt、StringsToFloat
//!
//! \platform
//!      ubuntu16.04 g++ version 5.4.0
//!
//! \example
    // 假定使用了 using namespace std;
    //string str("1,2,3_44-55");
    //string symbol = {',','_',' ', '-'};
    // vector<string> substrs = glib::utils::Split(str, symbol); // 产生 1 2 3 44 55 总共 5 个字符串
    // vector<string> substrs = glib::utils::Split(str, {',','.','!'});
    // for (auto iter = substrs.begin(); iter != substrs.end(); )
    //     cout << *iter++ << " ";
    // cout << endl;
    // vector<int> ints = glib::utils::StringsToInt(substrs);
    // for (size_t i = 0; i < ints.size(); i++) {
    //     cout << ints[i] << " ";
    // }

namespace glib {
namespace utils {

//! \brief 将 string 保存的字符串按照符号分割成子串。
//! \complexity 时间复杂度：O(n) 空间复杂度：O(n)
//! \param str 被分割的字符串
//! \param symbol 分割的字符集。可用初始化列表作为输入
//! \param reserve_symbol 是否保留符号，默认情况下，分割字符串时不保留符号。
//! \return 分割好的字符串
std::vector<std::string> Split(const std::string &str,
                               const std::string symbol,
                               const bool reserve_symbol = false) {
    std::vector<std::string> substrs;
    std::string sub;
    for (const auto &ch : str) {
        if (symbol.find(ch) != std::string::npos) { // 表示在给定的分隔符号表中
            if (!sub.empty()) // 防止连续的符号在一块
                substrs.push_back(sub);
            if (reserve_symbol) { substrs.push_back(std::string(1, ch)); } // 保存符号
            sub.clear(); // 清空 string
        } else
            sub += ch;
    }
    if (!sub.empty())
        substrs.push_back(sub);
    return substrs;
}

//! \brief 将由数字组成的多个子串转换为内置数值类型 比如 vector{"12", "23"}---> 12 23
//! \note 所有字符串包含的数字，要转换成的类型必须相同！！ 默认 int 类型
std::vector<int> StringsToInt(const std::vector<std::string> &str) {
    std::vector<int> ints;
    for (auto iter = str.begin(); iter != str.end(); )
        ints.emplace_back(stoi(*iter++));
    return ints;
}

//! \brief 字符串转换为 float 类型集合
//! \note 与上个 StringsToInt 函数一致
std::vector<float> StringsToFloat(const std::vector<std::string> &str) {
    std::vector<float> floats;
    for (auto iter = str.begin(); iter != str.end(); )
        floats.emplace_back(stof(*iter++));
    return floats;
}

} // namespace utils
} // namespace glib

#endif // GLIB_STRING_COMMON_HPP_
