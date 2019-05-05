/*
 * CopyRight (c) 2019 gcj
 * File: queue.test.cc
 * Project: algorithm
 * Author: gcj
 * Date: 2019/4/15
 * Description: verify correctness implement of queue
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#include "./queue.hpp"
#include <iostream>
#include <exception> // 捕获特定类型异常
using namespace std;

//! \brief 测试实现的队列，包含了循环队列、非循环队列的分别测试
//!        分别测试了入队、出队、移动赋值、移动构造、赋值、构造、取出第一个元素、取出末尾元素、改变第一个元素、改变末尾元素
//!        以及队列改变前后时的状态：有效数据个数、队尾索引及其值、队头索引及其值、队列是否为空、如果为空，取值时会报异常。
//! \run
//!     g++ queue.test.cc -std=c++11 && ./a.out
int main(int argc, char const *argv[]) {

// 测试 针对循环队列
    // Push Pop front back
    cout << "---------------------------测试循环队列！----------------------------------" << endl;
    cout << endl;
    glib::QueueCycle<int> queue_cycle; // 默认初始化为 10 个空间
    for (size_t i = 0; i < 10; i++) {
        queue_cycle.Push(i);
    }
    queue_cycle.print_value(); // 0 1 2 3 4 5 6 7 8 9 tail_index: 10 back_value: 9 head_index: 0 front_value: 0
    queue_cycle.Push(11);
    cout << "after push 11 number: " << endl;
    queue_cycle.print_value(); // 0 1 2 3 4 5 6 7 8 9 tail_index: 10 back_value: 9 head_index: 0 front_value: 0
    cout << "size: " << queue_cycle.size() << endl; // size = 10
    cout << "front: " << queue_cycle.front() << endl; // front = 0
    cout << "back: " << queue_cycle.back() << endl; // back = 9
    queue_cycle.Pop();
    queue_cycle.Push(11);
    cout << "after Pop and Push : " << endl;
    queue_cycle.print_value(); // 1 2 3 4 5 6 7 8 9 11 tail_index: 0 back_value: 11 head_index: 1 front_value: 1
    cout << "size: " << queue_cycle.size() << endl; // size = 10
    cout << "front: " << queue_cycle.front() << endl; // front = 1
    cout << "back: " << queue_cycle.back() << endl; // back = 11
    queue_cycle.Pop();
    queue_cycle.Pop();
    cout << "after two pop: " << endl;
    queue_cycle.print_value(); // 3 4 5 6 7 8 9 11 tail_index: 0 back_value: 11 head_index: 3 front_value: 3
    cout << "size: " << queue_cycle.size() << endl; // size = 8
    cout << "front: " << queue_cycle.front() << endl; // front = 3
    cout << "back: " << queue_cycle.back() << endl; // back = 11
    queue_cycle.back() = 12;
    queue_cycle.front() = 0;
    cout << "after change back(): " << endl;
    queue_cycle.print_value(); // 0 4 5 6 7 8 9 12  tail_index: 0 back_value: 12 head_index: 3 front_value: 0
    cout << "size: " << queue_cycle.size() << endl; // size = 8
    cout << "front: " << queue_cycle.front() << endl; // front = 0
    cout << "back: " << queue_cycle.back() << endl; // back = 12
    cout << "capacity: " << queue_cycle.capacity() << endl; // capacity = 10

    // 测试循环队列赋值拷贝函数
    cout << "测试循环队列拷贝函数，移动拷贝函数" << endl;
    glib::QueueCycle<int> queue2_cycle = std::move(queue_cycle); // or queue_cycle
    cout << "size: " << queue2_cycle.size() << endl; // size = 8
    cout << "front: " << queue2_cycle.front() << endl; // front = 0
    cout << "back: " << queue2_cycle.back() << endl; // back = 12
    cout << "capacity: " << queue2_cycle.capacity() << endl; // capacity = 10
    // 测试队列为空时，队列的状态
    cout << "测试异常" << endl;
    if (queue_cycle.empty()) {
        cout << "queue_cycle is empty" << endl;
        cout << "capacity: " << queue_cycle.capacity() << endl; // capacity = 0
        try {
            queue_cycle.front(); // 测试异常： terminate called after throwing an instance of 'char const*'
        } catch (const char * except) { // 捕获自己 throw 的异常
            std::cerr << except << '\n';
        } catch (const std::out_of_range& except) { // 捕获标准异常
            std::cerr << except.what() << '\n';
        }
    }
    queue_cycle.print_value(); // 不打印，此时队列为空。
    queue2_cycle.print_value(); // 0 4 5 6 7 8 9 12  tail_index: 0 back_value: 12 head_index: 3 front_value: 0
    cout << "测试循环队列赋值，移动赋值函数" << endl;
    glib::QueueCycle<int> queue3_cycle(5);
    queue3_cycle.print_value(); // 不打印，此时队列为空
    queue3_cycle = std::move(queue2_cycle); // or queue_cycle
    queue3_cycle.print_value(); // 0 4 5 6 7 8 9 12  tail_index: 0 back_value: 12 head_index: 3 front_value: 0

    cout << endl;
    cout << "---------------------------测试非循环队列！----------------------------------" << endl;
// 测试 针对非循环队列
    cout << endl;
    glib::Queue<int> queue; // 默认初始化为 10 个空间
    for (size_t i = 0; i < 10; i++) {
        queue.Push(i);
    }
    queue.print_value(); // 0 1 2 3 4 5 6 7 8 9 tail_index: 10 back_value: 9 head_index: 0 front_value: 0
    queue.Push(11);
    cout << "after push 11 number: " << endl;
    queue.print_value(); // 0 1 2 3 4 5 6 7 8 9  tail_index: 10 back_value: 9 head_index: 0 front_value: 0
    cout << "size: " << queue.size() << endl; // size = 10
    cout << "front: " << queue.front() << endl; // front = 0
    cout << "back: " << queue.back() << endl; // back = 9
    queue.Pop();
    queue.Push(11);
    cout << "after Pop and Push : " << endl;
    queue.print_value(); // 1 2 3 4 5 6 7 8 9 11 tail_index: 10 back_value: 11 head_index: 0 front_value: 1
    cout << "size: " << queue.size() << endl; // size = 10
    cout << "front: " << queue.front() << endl; // front = 1
    cout << "back: " << queue.back() << endl; // back = 11
    queue.Pop();
    queue.Pop();
    cout << "after two pop: " << endl;
    queue.print_value(); // 3 4 5 6 7 8 9 11 tail_index: 10 back_value: 11 head_index: 2 front_value: 3
    cout << "size: " << queue.size() << endl; // size = 8
    cout << "front: " << queue.front() << endl; // front = 3
    cout << "back: " << queue.back() << endl; // back = 11
    queue.back() = 12;
    queue.front() = 0;
    cout << "after change back(): " << endl;
    queue.print_value(); // 0 4 5 6 7 8 9 12  tail_index: 10 back_value: 12 head_index: 2 front_value: 0
    // 验证数据搬移操作
    queue.Pop();
    queue.Pop();
    queue.print_value(); // 5 6 7 8 9 12 tail_index: 10 back_value: 12 head_index: 4 front_value: 5
    queue.Push(1);
    queue.print_value(); // 5 6 7 8 9 12 1 tail_index: 7 back_value: 1 head_index: 0 front_value: 5
    cout << "size: " << queue.size() << endl; // size = 7
    cout << "front: " << queue.front() << endl; // front = 5
    cout << "back: " << queue.back() << endl; // back = 1
    cout << "capacity: " << queue.capacity() << endl; // capacity = 10

    // 验证拷贝移动赋值等函数
    cout << "测试非循环队列拷贝函数，移动拷贝函数" << endl;
    glib::Queue<int> queue2 = std::move(queue); // or queue_cycle
    cout << "size: " << queue2.size() << endl; // size = 7
    cout << "front: " << queue2.front() << endl; // front = 5
    cout << "back: " << queue2.back() << endl; // back = 1
    cout << "capacity: " << queue2.capacity() << endl; // capacity = 10

    // 测试队列为空时，队列的状态
    if (queue.empty()) {
        cout << "queue_cycle is empty" << endl;
        cout << "capacity: " << queue.capacity() << endl; // capacity = 0
        // queue.front(); // 测试异常：terminate called after throwing an instance of 'char const*'
                          // 使用标准异常封装时：terminate called after throwing an instance of 'std::out_of_range'
                          //    what():  Fetch data from an empty queue!

    }
    queue.print_value(); // 不打印，此时队列为空。
    queue2.print_value(); // 5 6 7 8 9 12 1 tail_index: 7 back_value: 1 head_index: 0 front_value: 5
    cout << "测试非循环队列赋值，移动赋值函数" << endl;
    glib::Queue<int> queue3(5);
    queue3.print_value(); // 不打印，此时队列为空
    queue3 = std::move(queue2); // or queue_cycle
    queue3.print_value(); // 5 6 7 8 9 12 1 tail_index: 7 back_value: 1 head_index: 0 front_value: 5

    return 0;
}
