/*
 * File: Stack.hpp
 * Project: algorithm
 * Author: gcj
 * Date: 2019/4/8
 * Description: stack simple implementation
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#ifndef GLIB_STACK_H_
#define GLIB_STACK_H_
#include <iostream>
#include <cstring> // use memcpy()
#include <string>
#include <vector>
#include <map> // 保存符号对应的优先级

//! \brief 本栈的实现是基于动态数组，对于内置类型都可以通过测试，对于 string 、vector 标准库类也能通过。
//!        在 Stack 后面是一些应用实例。
//! TODO leetCode example implementation
//! \leetCode: 20,155,232,844,224,682,496
namespace glib {
using namespace std;


template <typename T>
class Stack {
public:
    Stack() : stack_size_(10), top_of_stack_(0) {
        stack_ = new T[stack_size_]();
        if (nullptr == stack_)
            exit(-1);
    }
    Stack(size_t size) : stack_size_(size), top_of_stack_(0) {
        if (!size)
            stack_ = new T[10](); // 默认申请 10 个空间
        else
            stack_ = new T[stack_size_]();
        if (nullptr == stack_)
            exit(-1);
    }
    Stack(const Stack &obj)
        : stack_size_(obj.stack_size_), top_of_stack_(obj.top_of_stack_) {
        stack_ = new T[stack_size_]();
        if (nullptr == stack_)
            exit(-1);
        for (size_t i = 0; i < stack_size_; i++)
            stack_[i] = obj.stack_[i];
        // memcpy(stack_, obj.stack_, sizeof(T) * stack_size_); 复制 stl 类时会出错！
    }
    Stack& operator = (const Stack &obj) {
        if (*this == obj)
            return *this;
        if (nullptr != stack_)
            delete[] stack_;
        top_of_stack_ = obj.top_of_stack_;
        stack_size_ = obj.stack_size_;
        stack_ = new T[stack_size_]();
        if (nullptr == stack_)
            exit(-1);
        for (size_t i = 0; i < stack_size_; i++)
            stack_[i] = obj.stack_[i];
        // memcpy(stack_, obj.stack_, sizeof(T) * stack_size_); // 复制 stl 类时会出错！
        return (*this);
    }
    ~Stack() { if (nullptr != stack_) delete[] stack_; }

    bool empty() { return !stack_size_; }
    size_t top_of_stack() { return top_of_stack_; }
    void print_value() {
        for (size_t i = 0; i < top_of_stack_; i++)
            cout << stack_[i] << " ";
    }
    void Push(const T& data) {
        stack_[top_of_stack_] = data;
        ++top_of_stack_;
        if (top_of_stack_ >= stack_size_) { // 超出堆栈大小，需要从新申请 1.5 倍内存
            if (stack_size_ < 5)
                stack_size_ = 6;
            T *temp_space = new T[stack_size_ * 2 ]();
            for (size_t i = 0; i < stack_size_; i++)
                temp_space[i] = stack_[i];
            // memcpy(temp_space, stack_, sizeof(T) * stack_size_); // 复制指定字节数！但是对于 stl 类来说，这样复制会出错！
            stack_size_ *= 2; // 不要忘记！
            delete[] stack_;
            stack_ = temp_space;
        }
    }
    const pair<bool, T> Pop() {
        if (!top_of_stack_)
            return make_pair(false, T());
        --top_of_stack_;
        return make_pair(true, stack_[top_of_stack_]);
    }

private:
    T *stack_;
    size_t top_of_stack_; // 栈顶索引
    size_t stack_size_;   // 栈大小
};
/*-----------------------------Stack 应用-------------------------------------------*/

/*----------------------------------------------------------------------------------
** 栈在表达式求值中的应用: 34+13*9+44-12/3
** #include <map> <string> "stack.hpp" 且 using namespace std;
**     正确格式： 34+13*9+44-12/3
**     错误格式：+34+13*9+44-12/3-    34+13**9++44-12/3
** TODO 开始处有符号的表达式、结尾处有符号的表达式、以及中间出现冗余的符号。如何进行检测。计算出正确的结果！
**---------------------------------------------------------------------------------*/
//! \brief 将一段字符串按照分割符号列表进行分割成子串！
//! \param symbol 分割的字符集{可以是加减乘除符号}。可用初始化列表作为输入
vector<string> Split(const string &str, const string symbol) {
    vector<string> substrs;
    string sub;
    for (const auto &ch : str) {
        if (symbol.find(ch) != string::npos) { // 表示在给定的分隔符号表中
            if (!sub.empty()) // 防止连续的符号在一块
                substrs.push_back(sub);
            substrs.push_back(string(1,ch)); // 保存符号
            sub.clear(); // 清空 string
        } else
            sub += ch;
    }
    if (!sub.empty())
        substrs.push_back(sub);
    return substrs;
}

//! \brief 栈在表达式求值中的应用。
//! \note 限制仅仅是加减乘除，而且输入的格式只能是以数字为开始，开始处不能有任何符号。
//!       结尾也不能出现符号。然后中间必须符合表达式的输入格式
//!    正确格式： 34+13*9+44-12/3
//!    错误格式：+34+13*9+44-12/3-    34+13**9++44-12/3
//! \param str 保存键盘输入的表达式
//! \param symbol 表达式包含的运算符号表,比如可以直接输入 {'+','-','*','/'}
//! \param priority_table symbol 中存储的运算符表的优先级顺序，同等优先级是一个水平。
//!                       如减法和加法可以是 0，然后乘法和除法是 1
//! TODO 开始处有符号的表达式、结尾处有符号的表达式、以及中间出现冗余的符号。如何进行检测。计算出正确的结果！
float StackForExpression(const string &str, const string symbol,
                         map<string, size_t> priority_table) { // 如果加上了 const map 则会编译出错
    vector<string> split_strings = Split(str, symbol);
    Stack<float> operand;    // 操作数
    Stack<string> operators; // 运算符

    auto iter = split_strings.begin();
    for (; iter != split_strings.end(); ++iter) {
        if (priority_table.find(*iter) != priority_table.end()) { // 当前是运算符
            // 如果当前运算符比运算符堆栈中栈顶运算符优先级低或等于，需要拿出栈顶运算符和两个操作数进行运算
            // 直到当前运算符优先级大于栈顶优先级为止，然后将当前运算符压入运算符栈
            while (true) {
                const auto top_of_operators = operators.Pop();

                // 运算符栈顶不存在符号，直接压入栈。或者当前运算符优先级高于栈顶符号，则直接压栈当前符号
                if (!top_of_operators.first) {
                    // operators.Push(*iter);
                    break;
                }
                if (priority_table[*iter] > priority_table[top_of_operators.second]) {
                    operators.Push(top_of_operators.second); // 记住这里需要把刚刚出栈的数据在从新压栈！这里之前忘了！！！
                    // operators.Push(*iter);
                    break;
                }

                // 拿出两个操作数和栈顶运算符进行运算
                const auto right_operand = operand.Pop(); // 符号右操作数
                const auto left_operand = operand.Pop();

                // 正常情况下是，运算符比操作数少一个。此时一定能够拿到 2 个操作数。
                if (!right_operand.first || !left_operand.first) {
                    cerr << "input expression format is error,"
                            "Please check out and execute again!" << '\n';
                    exit(-1);
                }
                switch ((top_of_operators.second)[0]) { // "+"--->'+' 变为 char 好比较！
                    case '+': {
                        operand.Push(left_operand.second + right_operand.second);
                        break;
                    }
                    case '-': {
                        operand.Push(left_operand.second - right_operand.second);
                        break;
                    }
                    case '*': {
                        operand.Push(left_operand.second * right_operand.second);
                        break;
                    }
                    case '/': {
                        operand.Push(left_operand.second / right_operand.second);
                        break;
                    }
                }
            } // end while
            operators.Push(*iter); // 压入当前的符号！
        } else // 当前处理的是操作数，直接保存
            operand.Push(stof(*iter));
    } // end for

    // 退出循环后，拿出操作数栈和运算符栈的所有元素进行计算即可
    // 拿出两个操作数和栈顶运算符进行运算
    while (operators.top_of_stack()) { // 直到运算符为空，此时操作数栈中还剩最后的计算结果
        const auto top_of_operators = operators.Pop();
        const auto right_operand = operand.Pop(); // 符号右操作数
        const auto left_operand = operand.Pop();

        // 正常情况下是，运算符比操作数少一个。此时一定能够拿到 2 个操作数。
        if (!right_operand.first || !left_operand.first) {
            cerr << "input expression format is error,"
                    "Please check out and execute again!" << '\n';
            exit(-1);
        }
        switch ((top_of_operators.second)[0]) {
            case '+': {
                operand.Push(left_operand.second + right_operand.second);
                break;
            }
            case '-': {
                operand.Push(left_operand.second - right_operand.second);
                break;
            }
            case '*': {
                operand.Push(left_operand.second * right_operand.second);
                break;
            }
            case '/': {
                operand.Push(left_operand.second / right_operand.second);
                break;
            }
        }
    } // end second while

    auto return_value = operand.Pop();

    // 退出循环后，最后只剩下运算结果了
    if (return_value.first) {
        return return_value.second;
    }
    cerr << "compute is error,"
            "Please check out and execute again!" << '\n';
    exit(-1);
}
/*----------------------------------------------------------------------------------
** TODO 栈在括号匹配中的应用: {} [] () <> 与下面的消消乐同理
**---------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------
** 栈在消消乐中的应用:给定一个仅包含 0 或 1 的字符串，现在可以对其进行一种操作：当有两个相邻的字符，
** 其中有一个是 0 另外一个是 1 的时候，可以消除掉这两个字符。这样的操作可以一直进行下去直到找不到相邻
** 的 0 和 1 为止。问这个字符串经历了操作以后的最短长度。
**---------------------------------------------------------------------------------*/
//! \note strs 必须包含的仅仅是 01字符串,否则调用出错
//! \complexity: O(n)
size_t EliminateAdjacent(const string &strs) {
    Stack<char> stacks(strs.size()/2);
    for (size_t i = 0; i < strs.size(); i++) {
        if (strs[i] != '0' && strs[i] != '1') // 内部元素不是 0 或者 1就直接报错
            exit(-1);
        auto temp_char = stacks.Pop();
        if (temp_char.first) { // 堆栈取出的元素不为空
            if (temp_char.second != strs[i])
                continue;
            stacks.Push(temp_char.second);
            stacks.Push(strs[i]);
        } else {
            stacks.Push(strs[i]);
        }
    }
    return stacks.top_of_stack();
}

//! \brief 用于消消乐通用框架内部元素类
template <typename T>
class Element {
public:
    Element() : data_(0) {}
    Element(const T data) : data_(data) {}
    Element(const Element &obj){ data_ = obj.data_; }
    Element& operator=(const T& obj) {
        if (*this == obj)
            return *this;
        data_ = obj.data_;
        return *this;
    }

    // 自己需要定义如何才是不相等（满足消消乐条件），这里默认直接用 data_作为数据。消除规则定义
    // 默认元素不相等才算是消除
    // 消消乐条件设定返回 true
    bool operator !=(const Element &obj) {
        return (this->data_ != obj.data_);
    }
    T data() const { return data_; }

private:
    T data_;
};

//! \brief 相邻两个任意元素消消乐通用实现框架，vec 内部元素是类的话，必须重载了比较运算符号 !=
//!     且 != 是自己定义消除规则
template <typename T>
size_t EliminateAdjacent(const vector<T> &vec) {
    Stack<T> stacks(vec.size()/2);
    for (size_t i = 0; i < vec.size(); i++) {
        // if (vec[i] != '0' && vec[i] != '1') // 用来判断 vec 内部元素是不是符合要求！不符合直接报错
        //     exit(-1);
        auto temp_char = stacks.Pop();
        if (temp_char.first) {
            if (temp_char.second != vec[i]) // 满足消消乐条件，则直接判断下一个。
                continue;
            stacks.Push(temp_char.second); // 不消除，则把数据存储回去
            stacks.Push(vec[i]);
        } else {
            stacks.Push(vec[i]);
        }
    }
    return stacks.top_of_stack();
}

} // namespace stack

#endif
