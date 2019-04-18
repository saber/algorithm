/*
 * File: queue.hpp
 * Project: algorithm
 * Author: gcj
 * Date: 2019/4/11
 * Description: queue simple implementation
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#ifndef GLIB_QUEUE_HPP_
#define GLIB_QUEUE_HPP_
#include <iostream>
#include <string>

// #define CIRCLE_QUEUE
namespace glib {
using namespace std;
//! 备注： stl 中的 队列使用方法可参考: https://blog.csdn.net/wangshihui512/article/details/8930652
//! \brief 实现一个循环队列
//! TODO REQUIRES_NONEMPTY() 默认非 debug 时这里是空的，如果 debug 时，这里自动替换成检查是不是空！TODO 待实现！
//! TODO 并发队列以及阻塞队列的实现！
//! TODO 并发队列之无锁队列： CAS 实现循环队列的无锁 无锁队列的实现--->https://coolshell.cn/articles/8239.html
//!      https://blog.csdn.net/yutianzuijin/article/details/51649543
//!  面试必问的CAS，你懂了吗？---> https://zhuanlan.zhihu.com/p/34556594
//! Java之多线程锁的10点整理和建议---> https://www.jianshu.com/p/2926d9003ded
//! 加锁和锁的粒度的思考--->https://blog.csdn.net/renwotao2009/article/details/52537421
//! TODO 什么是阻塞队列？如何使用阻塞队列来实现生产者-消费者模型? ---> https://juejin.im/post/5aa8dcd1f265da237e096af6
#define DEFAULT_QUEUE_SIZE 10 // 表示有效队列大小
#define REQUIRES_NONEMPTY()
template <typename T>
class Queue {
public:
    Queue() : capacity_(DEFAULT_QUEUE_SIZE), size_(0), head_(0), tail_(0) {
        // 类似的后面加上 () 表示值初始化。但是对于类来说无意义（因为即使不加 () 编译器也会调用类的默认构造函数。
        // 所以 new 后面的类必须要有默认构造函数）。对于内置类型是有必要的！
        queue_ = new (nothrow) T[capacity_ + 1]();
        if (nullptr == queue_) {
            cerr << "new fail bad_alloc" << endl;
            exit(-1);
        }
    }
    Queue(size_t capacity) : capacity_(capacity),
                             size_(0), head_(0), tail_(0) {
        if (!capacity_) // 保证至少有 5 个空间
            capacity_ = 5;
        queue_ = new T[capacity_+1](); // 构建并默认初始化.
    }
    Queue(const Queue &obj) = delete;
    Queue& operator=(const Queue &obj) = delete;

    size_t capacity() const { return capacity_; } // 返回队列容量
    size_t size() const { return size_; } // 返回当前队列使用的大小
    bool empty() const { return (head_ == tail_); } // 判断当前队列是否为空

#ifdef CIRCLE_QUEUE
    void print_value() { // 打印有效数据按照先进先打印！ 循环队列适用！
        size_t i = head_ % (capacity_+1);
        while (i != tail_) {
            cout << queue_[i] << " ";
            i = (i+1) % (capacity_+1);
        }

        cout << " tail: " << tail_;
        cout << " head: " << head_ << endl;
    }
#else
    // 非循环队列打印
    void print_value() {
        for (size_t i = head_; i < tail_; i++) {
            cout << queue_[i] << " ";
        }
        cout << " tail: " << tail_;
        cout << " head: " << head_ << endl;
    }
#endif

    //! note 一般使用前需要调用 empty() 保证队列不为空
    // 返回队列头部数据
    const T& front() const {
        // if (head_ == tail_)
        //     return T(); // 表示一个默认初始值
        REQUIRES_NONEMPTY();
        return queue_[head_];
    }
    T& front() {
        // if (head_ == tail_)
        //     return T(); // 表示一个默认初始值
        REQUIRES_NONEMPTY();
        return queue_[head_];
    }

    // 返回尾部数据 不可修改
    const T& back() const {
        // if (head_ == tail_)
        //     return T();
        REQUIRES_NONEMPTY();
        if (0 == tail_)
            return queue_[capacity_];
        return queue_[tail_-1];
    }
    T& back() {
        // if (head_ == tail_)
        //     return T();
        REQUIRES_NONEMPTY();
        if (0 == tail_)
            return queue_[capacity_];
        return queue_[tail_-1];
    }

#ifdef CIRCLE_QUEUE
    // 循环队列实现
    // 将数据从队列尾部插入
    // 如果队列已经满了，则返回 false，否则返回 true
    bool Push(const T &data) {
        const size_t fake_tail = (tail_ + 1) % (capacity_+1);
        if (fake_tail == head_) {
            return false;
        }
        queue_[tail_] = data;
        tail_ = fake_tail;
        ++size_;
        return true;
    }
    bool Pop() {
        if (head_ == tail_) // 队列为空
            return false;
        head_ = (head_ + 1) % (capacity_+1);
        --size_;
    }
#else
    // 非循环队列实现 可以注释掉！
    bool Push(const T &data) {
        // 此时数组最大容量为 capacity_ 。与循环链表一样，牺牲了一个空间。
        // 目的是与循环链表写在一起
        if (tail_ == capacity_) {
            if (0 == head_) { // 表示队列满了
                return false;
            }
            // 队列没有满，此时要搬移数据
            for (size_t i = head_; i < tail_; i++) {
                queue_[i-head_] = queue_[i];
            }
            tail_ -= head_;
            head_ = 0; // 注意这里不要忘！！！！！
        }

        ++size_;
        queue_[tail_++] = data;
        return true;
    }
    bool Pop() {
        if (head_ == tail_)
            return false;
        ++head_;
        --size_;
        return true;
    }
#endif

private:
    // queue full condition: (tail_+1)%(capacity_+1) == head_; //capacity_ 是实际有效容量
    T *queue_;    // 队列空间
    size_t head_; // 指向队列头部
    size_t tail_; // 指向队列尾部
    size_t capacity_; // 队列总容量（不包含浪费的那块内存空间）这里表示的是真实能够容纳的容量！
    size_t size_; // 当前队列使用容量: 没有用这个数来作为队列满的条件！进而可以不浪费一块内存。
}; // class Queue

} // namespace glib

#endif
