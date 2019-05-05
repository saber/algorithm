/*
 * CopyRight (c) 2019 gcj
 * File: stack.test.cc
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

//! \brief 简单测试栈的基本功能
//! \run
//!     g++ stack.test.cc -std=c++11 && ./a.out
int main(int argc, char const *argv[]) {
    // 测试堆栈！
    cout << "测试压栈，出栈" << endl;
    glib::Stack<string> stac(5);
    for (size_t i = 0; i < 10; i++) {
        stac.Push("abc");
    }
    for (size_t i = 0; i < 20; i++) {
        cout << stac.Pop().second << " ";
    } // 10 个 abc
    cout << "sizeof(string): " << sizeof(string) << endl;
    cout << endl;

    // 测试 glib::EliminateAdjacent()
    cout << "测试消消乐" << endl;
    string ss("10");
    cout << glib::stack_app::EliminateAdjacent(ss) << endl; // 0
    cout << endl;

    // 测试 Element 元素 使用 EliminateAdjacent()
    cout << "测试消消乐通用框架，测试 Element 元素 使用 EliminateAdjacent()" << endl;
    vector<glib::stack_app::Element<char>> str;
    for (size_t i = 0; i < 20; i++) {
        str.push_back(glib::stack_app::Element<char>('1'));
        str.push_back(glib::stack_app::Element<char>('1'));
    }
    for (size_t i = 0; i < 2; i++) {
        str.push_back(glib::stack_app::Element<char>('0'));
    }
    cout << glib::stack_app::EliminateAdjacent(str) << endl;
    cout << endl;

    // 测试 vector<char> 使用 glib::EliminateAdjacent()
    cout << "测试 vector<char> 直接进行消消乐 " << endl;
    vector<char> str1=  {'1','1','1','1','1','1','1','1','1','1'};
    cout << glib::stack_app::EliminateAdjacent({'1','1','1','1','1','1','1','1','1','1'}) << endl;
    cout << endl;

    // 测试运算符表达式 34+13*9+44-12/3。结果为 191
    // 运行时输入上面表达式
    cout << "测试运算符表达式" << endl;
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
    cout << glib::stack_app::StackForExpression(input, {'+','-','*','/'}, priority_table) << endl;

    return 0;
}
