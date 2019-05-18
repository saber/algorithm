/*
 * CopyRight (c) 2019 gcj
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
#include <exception> // 使用标准异常 std::out_of_range

//! \brief 实现了基于数组的循环队列 QueueCycle 类、非循环队列 Queue 类。
//!        基本功能：
//!           1）入队、出队： Push、Pop
//!           2）状态函数：
//!             获取/修改头部数据、尾部数据、队列容量、队列是否为空、队列使用大小、打印队列元素
//!                 对应函数：front、back、capacity、empty、size、print_value
//!           3）支持移动构造、移动赋值操作
//! \Note
//!     1）当前队列默认构造是 10 个内存空间。可以通过修改下面的宏 DEFAULT_QUEUE_CAPACITY 来改变默认值
//!     2）队列满条件
//!         1）循环队列: (tail_+1)%(capacity_) == head_;
//!         2）非循环队列：head_ = 0 and tail_ = capacity_
//!       队列空条件
//!         循环队列 & 非循环 ：head_ == tail_
//! \TODO
//!     1）无锁并发队列（基于 CAS 的数组形式）、阻塞队列的实现
//!     2）记录使用标准库队列的使用方法
//!
//! \TODO Reference
//!     1）CAS 实现循环队列的无锁 无锁队列的实现--->https://coolshell.cn/articles/8239.html
//!        https://blog.csdn.net/yutianzuijin/article/details/51649543
//!     2）面试必问的 CAS，你懂了吗？---> https://zhuanlan.zhihu.com/p/34556594
//!     3）Java之多线程锁的 10 点整理和建议---> https://www.jianshu.com/p/2926d9003ded
//!     4）加锁和锁的粒度的思考--->https://blog.csdn.net/renwotao2009/article/details/52537421
//!     5）什么是阻塞队列？如何使用阻塞队列来实现生产者-消费者模型? ---> https://juejin.im/post/5aa8dcd1f265da237e096af6
//!     6）stl 中的队列使用方法可参考: https://blog.csdn.net/wangshihui512/article/details/8930652
//!
//!     7）最终的参考阻塞队列可以参考 cartographer 中的 common 中的 blocking_queue.h 实现

namespace glib {
using namespace std;

#define DEFAULT_QUEUE_CAPACITY 10 // 表示默认队列容量

// 队列为空时操作就会报异常
#define REQUIRES_NONEMPTY (                                                            \
    [&]() { if (empty()) throw std::out_of_range("Fetch data from an empty queue!"); } \
)

// 自定义简单异常
// #define REQUIRES_NONEMPTY (                                             \
//         [&]() { if (empty()) throw "Fetch data from an empty queue!"; } \
// )

// 非循环队列——基于数组的
template <typename T>
class Queue {
public:
    // 默认构造函数，默认有 10 个存储空间
    Queue() : capacity_(DEFAULT_QUEUE_CAPACITY), size_(0), head_(0), tail_(0) {
        // 类似的后面加上 () 表示值初始化。但是对于类来说无意义（因为即使不加 () 编译器也会调用类的默认构造函数。
        // 所以 new 后面的类必须要有默认构造函数）。对于内置类型是有必要的！
        queue_ = new (nothrow) T[capacity_]();
        if (nullptr == queue_) {
            cerr << "new fail, bad_alloc" << endl;
            exit(-1);
        }
    }

    // 带参数的构造函数
    Queue(const size_t capacity) : capacity_(capacity),
                                   size_(0), head_(0), tail_(0) {
        queue_ = new T[capacity_](); // 构建并默认初始化
    }

    // 析构函数，注意释放内存
    ~Queue() {
        if (nullptr != queue_) {
            delete[] queue_;
            queue_    = nullptr;
            capacity_ = 0;
            size_     = 0;
            head_     = 0;
            tail_     = 0;
        }
    }

    // 拷贝构造函数
    Queue(const Queue &other) : capacity_(other.capacity_),
                                size_(other.size_),
                                head_(other.head_),
                                tail_(other.tail_) {
        queue_ = new T[capacity_]();

        // 非循环队列元素的拷贝
        for (size_t i = other.head_; i < other.tail_; i++) {
            queue_[i] = other.queue_[i];
        }
    }

    // 移动拷贝构造函数
    Queue(Queue &&other) : queue_(other.queue_),
                           capacity_(other.capacity_),
                           size_(other.size_),
                           head_(other.head_),
                           tail_(other.tail_) {
        other.queue_    = nullptr;
        other.capacity_ = 0;
        other.size_     = 0;
        other.head_     = 0;
        other.tail_     = 0;
    }

    // 赋值函数
    Queue& operator=(const Queue &other) {
        if (&other != this) {
            delete[] queue_;
            queue_    = new T[other.capacity_];
            capacity_ = other.capacity_;
            size_     = other.size_;
            head_     = other.head_;
            tail_     = other.tail_;

            for (size_t i = other.head_; i < other.tail_; i++) {
                queue_[i] = other.queue_[i];
            }
        }
        return *this;
    }

    // 移动赋值函数
    Queue& operator=(Queue &&other) {
        if (&other != this) {
            delete[] queue_;
            queue_          = other.queue_;
            capacity_       = other.capacity_;
            size_           = other.size_;
            head_           = other.head_;
            tail_           = other.tail_;

            other.queue_    = nullptr;
            other.capacity_ = 0;
            other.size_     = 0;
            other.head_     = 0;
            other.tail_     = 0;
        }
        return *this;
    }

    size_t capacity() const { return capacity_;        } // 返回队列容量
    size_t size()     const { return size_;            } // 返回当前队列使用的大小
    bool   empty()    const { return (head_ == tail_); } // 判断当前队列是否为空


    //! note 一般使用前需要调用 empty() 保证队列不为空
    // 返回队列头部数据
    const T& front() const {
        // if (head_ == tail_)
        //     throw "Fetch data from an empty queue!";
        REQUIRES_NONEMPTY();
        return queue_[head_];
    }

    // 返回队列头，可修改该值
    T& front() {
        // if (head_ == tail_)
        //     throw "Fetch data from an empty queue!";
        REQUIRES_NONEMPTY();
        return queue_[head_];
    }

    // 返回尾部数据，不可修改该值
    const T& back() const {
        // if (head_ == tail_)
        //     throw "Fetch data from an empty queue!";
        REQUIRES_NONEMPTY();
        return queue_[tail_-1];
    }

    // 返回队列尾，可以修改该值
    T& back() {
        // if (head_ == tail_)
        //     throw "Fetch data from an empty queue!";
        REQUIRES_NONEMPTY();
        return queue_[tail_-1];
    }

    //! \brief 非循环队列入队实现
    //! \complexity best case：O(1)， worst case：O(n), average case：O(1)
    bool Push(const T &data) {
        if (tail_ == capacity_) {
            if (0 == head_) { // 表示队列满了
                return false;
            }
            // 队列没有满，此时要搬移数据
            for (size_t i = head_; i < tail_; i++) {
                queue_[i-head_] = queue_[i];
            }
            tail_ -= head_;
            head_ = 0;        // 注意这里不要忘！
        }

        ++size_;
        queue_[tail_++] = data;
        return true;
    }

    // 出队
    bool Pop() {
        if (head_ == tail_)
            return false;
        ++head_;
        --size_;
        return true;
    }

    // 非循环队列打印内部有效元素
    void print_value() {
        if (empty()) {
            cout << " " << endl;
            return;
        }
        for (size_t i = head_; i < tail_; i++) {
            cout << queue_[i] << " ";
        }
        cout << " tail_index: " << tail_ << " back_value: " << back();
        cout << " head_index: " << head_ << " front_value: " << front() << endl;
    }

private:
    // 队列满条件：head_ = 0 and tail_ = capacity_
    // 队列空条件：head_ == tail_

    T*     queue_    = nullptr; // 队列空间
    size_t head_     = 0;       // 指向队列头部
    size_t tail_     = 0;       // 指向队列尾部
    size_t capacity_ = 0;       // 队列总容量
    size_t size_     = 0;       // 当前队列使用容量: 没有用这个数来作为队列满的条件！进而可以不浪费一块内存。

}; // class Queue

//! \brief 循环队列——基于数组实现
//! \note 在构造函数时，要多加 1，因为循环队列会有浪费一个空间。
//!       但是对于使用者而言，与上面的非循环队列使用方法是一致的。隐藏了内部转换细节
template <typename T>
class QueueCycle {
public:

    // 默认构造函数，默认有 10 个有效存储空间
    QueueCycle() : capacity_(DEFAULT_QUEUE_CAPACITY + 1),
                   size_(0), head_(0), tail_(0) {
        // 类似的后面加上 () 表示值初始化。但是对于类来说无意义（因为即使不加 () 编译器也会调用类的默认构造函数。
        // 所以 new 后面的类必须要有默认构造函数）。对于内置类型是有必要的！
        queue_ = new (nothrow) T[capacity_]();
        if (nullptr == queue_) {
            cerr << "new fail, bad_alloc" << endl;
            exit(-1);
        }
    }

    // 带参数的构造函数
    QueueCycle(const size_t capacity) : capacity_(capacity + 1),
                                        size_(0), head_(0), tail_(0) {
        queue_ = new T[capacity_](); // 构建并默认初始化.
    }

    // 析构函数，注意释放内存
    ~QueueCycle() {
        if (nullptr != queue_) {
            delete[] queue_;
            queue_    = nullptr;
            size_     = 0;
            head_     = 0;
            tail_     = 0;
            capacity_ = 0;
        }
    }

    // 拷贝构造函数
    QueueCycle(const QueueCycle &other) : capacity_(other.capacity_),
                                          size_(other.size_),
                                          head_(other.head_),
                                          tail_(other.tail_) {
        queue_ = new (nothrow) T[capacity_]();

        // 循环队列元素拷贝
        size_t idx = other.head_ % other.capacity_;
        for ( ; idx != other.tail_ ; idx = (idx + 1) % other.capacity_) {
            queue_[idx] =  other.queue_[idx];
        }
    }

    // 移动拷贝构造
    QueueCycle(QueueCycle &&other) : queue_(other.queue_),
                                     capacity_(other.capacity_),
                                     size_(other.size_),
                                     head_(other.head_),
                                     tail_(other.tail_) {
        other.queue_    = nullptr;
        other.capacity_ = 0;
        other.size_     = 0;
        other.head_     = 0;
        other.tail_     = 0;
    }

    // 赋值函数
    QueueCycle& operator=(const QueueCycle &other) {
        if (&other != this) {
            delete[] queue_;
            queue_    = new (nothrow) T[other.capacity_]();
            size_     = other.size_;
            head_     = other.head_;
            tail_     = other.tail_;
            capacity_ = other.capacity_;

            // 循环队列元素拷贝
            size_t idx = other.head_ % other.capacity_;
            for ( ; idx != other.tail_ ; idx = (idx + 1) % other.capacity_) {
                queue_[idx] =  other.queue_[idx];
            }
        }
        return *this;
    }

    // 移动赋值函数
    QueueCycle& operator=(QueueCycle &&other) {
        if (&other != this) {
            delete[] queue_;
            queue_          = other.queue_;
            size_           = other.size_;
            head_           = other.head_;
            tail_           = other.tail_;
            capacity_       = other.capacity_;

            other.queue_    = nullptr;
            other.size_     = 0;
            other.head_     = 0;
            other.tail_     = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    size_t capacity() const { return (capacity_? capacity_ - 1: 0); } // 返回队列容量
    size_t size()     const { return size_;                         } // 返回当前队列使用的大小
    bool   empty()    const { return (head_ == tail_);              } // 判断当前队列是否为空


    //! note 一般使用前需要调用 empty() 保证队列不为空
    // 返回队列头部数据
    const T& front() const {
        // if (head_ == tail_)
        //     throw "Fetch data from an empty queue!";
        REQUIRES_NONEMPTY();
        return queue_[head_];
    }

    // 返回队列头，可修改该值
    T& front() {
        // if (head_ == tail_)
        //     throw "Fetch data from an empty queue!";
        REQUIRES_NONEMPTY();
        return queue_[head_];
    }

    // 返回尾部数据，不可修改该值
    const T& back() const {
        // if (head_ == tail_)
        //     throw "Fetch data from an empty queue!";
        REQUIRES_NONEMPTY();
        if (0 == tail_)
            return queue_[capacity_ - 1];
        return queue_[tail_-1];
    }

    // 返回队列尾，可以修改该值
    T& back() {
        // if (head_ == tail_)
        //     throw "Fetch data from an empty queue!";
        REQUIRES_NONEMPTY();
        if (0 == tail_)
            return queue_[capacity_ - 1];
        return queue_[tail_-1];
    }

    // 将数据从队列尾部插入
    // 如果队列已经满了，则返回 false，否则返回 true
    //! \complexity O(1)
    bool Push(const T &data) {
        const size_t fake_tail = (tail_ + 1) % capacity_;
        if (fake_tail == head_) {
            return false;
        }
        queue_[tail_] = data;
        tail_         = fake_tail;
        ++size_;
        return true;
    }

    // 出队列
    bool Pop() {
        if (head_ == tail_) // 队列为空
            return false;
        head_ = (head_ + 1) % capacity_;
        --size_;
    }

    // 打印有效数据按照先进先打印！ 循环队列适用！
    void print_value() {
        if (empty()) {
            cout << " " << endl;
            return;
        }
        size_t i = head_ % capacity_;
        while (i != tail_) {
            cout << queue_[i] << " ";
            i = (i+1) % capacity_;
        }
        cout << " tail_index: " << tail_ << " back_value: " << back();
        cout << " head_index: " << head_ << " front_value: " << front() << endl;
    }

private:
    // 队列满条件：(tail_+1)%(capacity_+1) == head_;
    // 队列空条件：head_ == tail_

    T*     queue_    = nullptr; // 队列空间
    size_t head_     = 0;       // 指向队列头部
    size_t tail_     = 0;       // 指向队列尾部
    size_t capacity_ = 0;       // 队列总容量
    size_t size_     = 0;       // 当前队列使用容量: 没有用这个数来作为队列满的条件！进而可以不浪费一块内存。

}; // class QueueCycle

} // namespace glib

#endif // GLIB_QUEUE_HPP_
