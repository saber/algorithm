/*
 * File: string_encode.cc
 * Project: algorithm
 * Author: gcj
 * Date: 2019/4/8
 * Description:
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

//! 题目描述： 定义一种字符串编码，其编码格式如下：
//!     9 个字符为一个编码组，第一个字符表示后续 8 个字符串的字节序列（字符'\0' 表示小端，字符'1'表示大端）
//!     后续 8 个字符，每个字符代表一个字节，编码解析后字符串采用大端模式，例如编码组 "012345678",解析后的大端字符串为
//!     "87654321";编码组"112345678" ，解析后的大端字符串为 "12345678"
//! 现输入一个数字和一个字符串，数字表示字符串中有多少组编码，需要解析并打印解析后的字符串，具体要求如下：
//! 按编码组出现的顺序（从左至右）打印一行结果，解析后的结果为大端模式，每个编码组的结果以一个空格分隔行末无空格
//!
//! 输入描述：
//!     第一行为数字 N，表示后续输入的字符串有多少个编码组
//!     第二行为编码字符串
//! 输出描述：
//!     输出为一行，包含 N 个编码组解析结果(大端模式)，每个结果以一个空格分隔，行末无空格
//! 示例：
//! 输入
//!     2
//!     0abcdefgh1abcdefgh
//! 输出
//!     hgfedcba abcdefgh
//!
//! 备注：
//!     输入一定合法，第一行输入不会为 0， 第二行字符串长度小于 128

//! 本程序不要求输入的编码组一定为 8 位
//! 思路：遍历字符串以 0,1 为间隔分组保存，然后每个组按照 0 1 打印输出
//! TODO
//! 最有效率的实现方式应该是遍历一遍就直接输出，遇到 0 先保留，直到遇到1为止，然后遇到1就直接打印输出。
//! 这样对于大端模式直接就可以打印了。减少了后面保存在打印的过程

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// 该字符串是否是 0 or 1 代表小端和大端
bool is_0_or_1(char &ch) {
    if (ch == '0' || ch == '1')
        return true;
    return false;
}

int main(int argc, char const *argv[]) {
    // 读取输入
    int encode_group_count = 0;
    cin >> encode_group_count; // 将要编码组数量
    string input_string;
    char ch;
    while (cin >> input_string) {
        if ((ch = getchar()) == '\n')
            break;
    }

    // 分段保存编码组
    vector<vector<char> > seg_chars; // 分段保存字符串
    vector<char> chars;
    for (size_t i = 0; i < input_string.size(); i++) {
        if (is_0_or_1(input_string[i])) {
            if (i != 0)
                seg_chars.push_back(chars);
            chars.clear();
        }
        chars.push_back(input_string[i]);
    }
    seg_chars.push_back(chars);

    // 分段编码组打印输出
    for (size_t i = 0; i < seg_chars.size(); i++) {
        if (seg_chars[i][0] == '0') {
            // 反向打印
            for (size_t j = seg_chars[i].size()-1; j > 0; j--) {
                cout << seg_chars[i][j];
            }
            encode_group_count--;
            if (encode_group_count != 0)
                cout << " ";
        }
        else {
            // 正向打印
            for (size_t j = 1; j < seg_chars[i].size(); j++) {
                cout << seg_chars[i][j];
            }
            encode_group_count--;
            if (encode_group_count != 0)
                cout << " ";
        }
    }
    // cout << "***" << endl; // 检验行末无空格
    return 0;
}
/* // 直接输出未实现！
enum State {
    init      = 0,
    small_end = 1,
    big_end   = 2
};
State curr_state = init;
int encode_gps = encode_group_count;
vector<char> chs;
for (size_t i = 0; i < input_string.size(); i++) {
    if (input_string[i] == '1') {
        encode_gps--;
        if (curr_state != init)
            cout << " ";
        if (curr_state == small_end) {
            for (int j = chs.size()-1; j >= 0; j--) {
                cout << chs[j];
            }
            chs.clear();
        }
        curr_state = big_end;
        continue;
    } else if (input_string[i] == '0'){
        encode_gps--;
        if (curr_state != init)
            cout << " ";
        if (curr_state == small_end) {
            for (int j = chs.size() -1; j >= 0; j--)
                cout << chs[j];
            chs.clear();
        }
        curr_state = small_end;
        continue;
    }
    if (curr_state == big_end)
        cout << input_string[i];
    else if (curr_state == small_end)
        chs.push_back(input_string[i]);
}
// 最后是小端时直接逆向输出
if (curr_state == small_end) {
    cout << " ";
    for (int i = chs.size()-1; i >= 0; i--) {
        cout << chs[i];
    }
}
cout << "***" << endl; // 检验空格
*/
