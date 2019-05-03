/*
 * CopyRight (c) 2019 gcj
 * File: huawei.cc
 * Project: algorithm
 * Author: gcj
 * Date: 2019/4/8
 * Description:
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

//! 题目描述： 多维数组合并：
//!         现在有多组整数数组，需要将他们合并成一个新的数组。合并规则，从每个数组里按照顺序取出固定长度
//!     的内容合并到新的数组中，取完的内容会删除掉，如果该行不足固定长度或者已经为空，则直接取出剩余部分
//!     内容放到新的数组中，继续下一行。
//!  如样例1，获得长度 3，先遍历第一行，获得 2,5,6；在遍历第二行，获得 1,7,4；在循环回到第一行，获得
//!  7,9,5;在遍历第二行，获得 3,4；在回到第一行，获得 7，按照顺序拼接成最终结果
//!
//! 输入描述：
//!     第一行是每次读取的固定长度，长度 >0
//!     第 2-n 是需要合并的数组，不同的数组用回车换行分割，数组内部用逗号分隔
//! 输出描述：
//!     输出一个新的数组，用逗号分隔
//! 示例：
//! 输入
//!     3
//!     2,5,6,7,9,5,7
//!     1,7,4,3,4
//! 输出
//!     2,5,6,1,7,4,79,5,3,4,7
//!


//! 思路：按照行读取，存储成字符串。然后将字符串分割为 int 数组。之后对数组按照题目要求进行分别打印即可！

#include "../src/utils/string_common.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cctype> // 判断 string 中的字符的特性 c++ primer p82

using namespace std;


int main(int argc, char const *argv[]) {

    int encode_group_count = 0;
    cin >> encode_group_count;
    vector<string> vec_string;
    string input_string;
    char ch = getchar();
    char ch_second = '\0';
    while (cin >> input_string) {
        if (ch_second != '\0')
            input_string.insert(0, 1, ch_second);
        vec_string.push_back(input_string);
        ch = getchar();
        ch_second = getchar(); // 读取第二次
        if (ch_second == '\n')
            break;
    }
    vector<vector<int> > inputs;
    vector<string> temp_strings;
    auto iter = vec_string.begin();
    for (; iter != vec_string.end(); ++iter) {
        temp_strings = glib::StringCommon::Split(*iter, ",");
        inputs.emplace_back(glib::StringCommon::StringsToInt(temp_strings));
    }

    while (inputs.size()) {
        auto iter = inputs.begin();
        for (; iter != inputs.end(); ) {
            if ((*iter).size()) { // 存在数值类型

                if ((*iter).size() <= encode_group_count) {
                     // 全部输出然后删除
                     for (size_t i = 0; i < (*iter).size(); i++) {
                         cout << (*iter)[i];
                         if ((inputs.size() != 1) || (i != (*iter).size()-1)) // 仅仅剩下一个 int，则末尾不要逗号
                             cout << ",";
                     }
                     (*iter).erase((*iter).begin(), (*iter).begin() + (*iter).size());
                     iter = inputs.erase(iter);
                     continue;
                } else {
                    for (size_t i = 0; i < encode_group_count; i++)
                        cout << (*iter)[i] << ",";
                    (*iter).erase((*iter).begin(), (*iter).begin() + encode_group_count);
                }

            }
            iter++;
        }
    }

    return 0;
}
