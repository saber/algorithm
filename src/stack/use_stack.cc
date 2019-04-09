/*
 * File: use_stack.cc
 * Project: algorithm
 * Author: gcj
 * Date: 2019/4/8
 * Description: using stack test
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */
#include "./stack.hpp"
#include <iostream>
#include <string>

using namespace std;
//! \brief 可以取消相应部分注释，然后编译运行查看测试结果！

int main(int argc, char const *argv[]) {
// 测试堆栈！
    // glib::Stack<string> stac(5);
    // cout << "测试输入" << endl;
    // for (size_t i = 0; i < 10; i++) {
    //     stac.Push("abc");
    // }
    // for (size_t i = 0; i < 20; i++) {
    //     cout << stac.Pop().second << " ";
    // }
    // cout << "sizeof(string): " << sizeof(string);
// 测试 glib::EliminateAdjacent()
    // string ss("10");
    // cout << glib::EliminateAdjacent(ss) << "e"; // 0

// 测试 Element 元素 使用 EliminateAdjacent()
    // vector<glib::Element<char>> str;
    // for (size_t i = 0; i < 20; i++) {
    //     str.push_back(glib::Element<char>('1'));
    //     str.push_back(glib::Element<char>('1'));
    // }
    // for (size_t i = 0; i < 2; i++) {
    //     str.push_back(glib::Element<char>('0'));
    // }
    // cout << glib::EliminateAdjacent(str);

// 测试 vector<char> 使用 glib::EliminateAdjacent()
    // vector<char> str1=  {'1','1','1','1','1','1','1','1','1','1'};
    // cout << glib::EliminateAdjacent({'1','1','1','1','1','1','1','1','1','1'});

// 测试运算符表达式 34+13*9+44-12/3。
// 运行时输入上面表达式
    string input;
    cin >> input;
    // std::map<string, size_t> priority_table;
    // priority_table.insert({"+",0});
    // priority_table.insert({"-",0});
    // priority_table.insert({"*",1});
    // priority_table.insert({"/",1});
    //or // 优先级表，对应数字越大表示优先级越高！
    std::map<string, size_t> priority_table = { {"+", 0}, {"-", 0},
                                                {"*", 1}, {"/", 1} };
    cout << glib::StackForExpression(input, {'+','-','*','/'}, priority_table);

    return 0;
}
