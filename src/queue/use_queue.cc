/*
 * File: use_queue.cc
 * Project: algorithm
 * Author: gcj
 * Date: 2019/4/15
 * Description: verify correctness implement of queue  
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#include "./queue.hpp"
#include <iostream>
using namespace std;

int main(int argc, char const *argv[]) {
#ifdef CIRCLE_QUEUE
    // 测试 针对循环队列
    // Push Pop front back
    glib::Queue<int> queue; // 默认初始化为 10 个空间
    for (size_t i = 0; i < 10; i++) {
        queue.Push(i);
    }
    queue.print_value(); // 0 1 2 3 4 5 6 7 8 9 tail: 10 head: 0
    queue.Push(11);
    cout << "after push 11 number: " << endl;
    queue.print_value(); // 0 1 2 3 4 5 6 7 8 9 tail: 10 head: 0
    cout << "size: " << queue.size() << endl; // size = 10
    cout << "front: " << queue.front() << endl; // front = 0
    cout << "back: " << queue.back() << endl; // back = 9
    queue.Pop();
    queue.Push(11);
    cout << "after Pop and Push : " << endl;
    queue.print_value(); // 1 2 3 4 5 6 7 8 9 11 tail = 0 head_ = 1
    cout << "size: " << queue.size() << endl; // size = 10
    cout << "front: " << queue.front() << endl; // front = 1
    cout << "back: " << queue.back() << endl; // back = 11
    queue.Pop();
    queue.Pop();
    cout << "after two pop: " << endl;
    queue.print_value(); // 3 4 5 6 7 8 9 11 tail: 0 head: 3
    cout << "size: " << queue.size() << endl; // size = 8
    cout << "front: " << queue.front() << endl; // front = 3
    cout << "back: " << queue.back() << endl; // back = 11
    queue.back() = 12;
    queue.front() = 0;
    cout << "after change back(): " << endl;
    queue.print_value(); // 0 4 5 6 7 8 9 12  tail: 0 head: 3
#else
    // 测试 针对非循环队列
    glib::Queue<int> queue; // 默认初始化为 10 个空间
    for (size_t i = 0; i < 10; i++) {
        queue.Push(i);
    }
    queue.print_value(); // 0 1 2 3 4 5 6 7 8 9 tail: 10 head: 0
    queue.Push(11);
    cout << "after push 11 number: " << endl;
    queue.print_value(); // 0 1 2 3 4 5 6 7 8 9 tail: 10 head: 0
    cout << "size: " << queue.size() << endl; // size = 10
    cout << "front: " << queue.front() << endl; // front = 0
    cout << "back: " << queue.back() << endl; // back = 9
    queue.Pop();
    queue.Push(11);
    cout << "after Pop and Push : " << endl;
    queue.print_value(); // 1 2 3 4 5 6 7 8 9 11 tail = 10 head_ = 0
    cout << "size: " << queue.size() << endl; // size = 10
    cout << "front: " << queue.front() << endl; // front = 1
    cout << "back: " << queue.back() << endl; // back = 11
    queue.Pop();
    queue.Pop();
    cout << "after two pop: " << endl;
    queue.print_value(); // 3 4 5 6 7 8 9 11 tail: 10 head: 2
    cout << "size: " << queue.size() << endl; // size = 8
    cout << "front: " << queue.front() << endl; // front = 3
    cout << "back: " << queue.back() << endl; // back = 11
    queue.back() = 12;
    queue.front() = 0;
    cout << "after change back(): " << endl;
    queue.print_value(); // 0 4 5 6 7 8 9 12  tail: 10 head: 2
#endif

    return 0;
}
