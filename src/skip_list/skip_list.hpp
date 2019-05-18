/*
 * CopyRight (c) 2019 gcj
 * File: skip_list.hpp
 * Project: algorithm
 * Author: gcj
 * Date: 2019/5/15
 * Description: skip list simple implementation
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#ifndef GLIB_SKIP_LIST_HPP_
#define GLIB_SKIP_LIST_HPP_
#include <iostream>
#include <random>
#include <initializer_list>

//! \brief 实现了跳表
//!     基本功能：
//!          1）插入：Insert
//!          2）删除：Delete
//!          3）查找；Find
//!          4）打印跳表内的值：print_value
//!
//! \Note
//!     1）目前只支持内置数值类型的数据
//!     2）不支持拷贝赋值
//!     3）该实现非理论上的跳表实现，每个节点对应有几层索引，在数据插入时就已经确定，
//!        不会随着数据增多而变大或缩小
//! \TODO
//!     1）支持其他数据类型
//!     2）能否变为理论上的跳表实现
//!     3）动态修改索引层，根据数据插入和删除的次数，动态更新跳表索引层，实现理论跳表。
//!
//! \platform
//!     ubuntu16.04 g++ version 5.4.0

namespace glib {
using namespace std;

template <typename _Scalar, int MAX_LEVEL = 16>
class SkipList {
public:
    struct Node {
        _Scalar data;
        Node * forwards[MAX_LEVEL] = {nullptr}; // 每个索引对应的值，代表跳表相应层指向的下一个节点
                                                // 这个 16 个数组值，本质上就是指向不同的底层跳表节点
        int max_level;                          // 表示当前节点所在最大层数
        Node() : data(-1), max_level(0) {}
    };

    SkipList() : current_max_level_(1) {
        head_ = new Node;
    }

    SkipList(std::initializer_list<_Scalar> il) : current_max_level_(1) {
        head_ = new Node;
        for (const auto &iter: il)
            Insert(iter);
    }

    ~SkipList() {
        if (nullptr != head_) {
            if (head_->forwards[0] != nullptr) {
                Node *p = head_->forwards[0];
                while (p->forwards[0] != nullptr) {
                    Node *next = p->forwards[0];
                    delete p;
                    p = next;
                }
                delete p;
            }
            delete head_;
        }
    }
    SkipList(const SkipList& other) = delete;
    SkipList& operator=(const SkipList& other) = delete;

    // 给定值找到在链表中的索引
    //! \complexity 时间复杂度 O(log(n)) 空间复杂度 O(1)
    Node* Find(int value) {
        Node *p = head_;
        // 对每一层节点进行遍历
        for (int i = current_max_level_ - 1; i >= 0; --i) {
            // 这里 i 对应该跳表的遍历层数
            while (p->forwards[i] != nullptr && p->forwards[i]->data < value) {
                p = p->forwards[i];
            }
        }
        if (p->forwards[0] != nullptr && p->forwards[0]->data == value) {
            return p->forwards[0];
        } else {
            return nullptr;
        }
    }

    // 一边插入（按照数据值从小到大）数据，一边建立索引层
    //! \complexity 时间复杂度 O(log(n)) 空间复杂度 O(1)
    void Insert(int value) {
        int level = Random(); // 每次插入一个数据 都会得到一个当前数据能够达到的跳表层
        Node *new_node = new Node;
        new_node->data = value;
        new_node->max_level = level;
        Node *temp[level] = {nullptr}; // 保留将要插入的数据对应每层的位置，方便后面把数据插入到相应的位置，然后构建 level 层索引

        // 遍历整个跳表，找到该值对应的每一层的位置
        Node *p = head_;
        for (int i = level - 1; i >= 0; --i) {
            while (p->forwards[i] != nullptr && p->forwards[i]->data < value) {
                p = p->forwards[i];
            }
            temp[i] = p; // 保留每一层的前驱节点（值小于 value）
        }

        // 上面找到了每一层的位置后，把将要插入的值 value 放置到对应位置
        for (int i = level - 1; i >= 0; --i) {
            new_node->forwards[i] = temp[i]->forwards[i];
            temp[i]->forwards[i] = new_node; // 每一层都指向跳表底层新建立的节点
        }

        // 更新跳表中最大层数
        if (current_max_level_ < level) current_max_level_ = level;
    }

    // 根据给定值，删除跳表中对应的节点。本质上仅仅删除了底层链表上节点。
    // 如果该值含有重复数据，这里仅仅删除其中一个数据
    //! \complexity 时间复杂度 O(logn) 空间复杂度 O(1)
    void Delete(int value) {
        Node *temp[current_max_level_] = {nullptr};
        Node *p = head_;

        // 找到给定值对应链表节点的前驱节点
        for (int i = current_max_level_ - 1; i >=0; --i) {
            while (p->forwards[i] != nullptr && p->forwards[i]->data < value) {
                p = p->forwards[i];
            }
            temp[i] = p;
        }

        Node *find_value = nullptr;
        if (p->forwards[0] != nullptr && p->forwards[0]->data == value) {
            find_value = p->forwards[0];
        }

        // 跳表中确实含有该值，那么删除其值等于给定值 value 的底层根节点，且改变前驱节点的指向
        if (nullptr != find_value) { // 找到了要删除的节点值
            for (int i = current_max_level_ - 1; i >= 0; --i) {
                // 改变每层索引的指向
                if (temp[i]->forwards[i] != nullptr && temp[i]->forwards[i]->data == value)
                    temp[i]->forwards[i] = find_value->forwards[i];
            }
            // 最后删除底层的根节点
            delete find_value;
        }
    }

    // 产生一个随机跳表高度
    int Random() {
        int level = 1;
        std::random_device random_device_; // 随机数生成器
        for (int i = 0; i < max_level_ - 1; ++i) {
            if (random_device_() % 2 == 1)
                level++;
        }
        return level;
    }

    void print_value() {
        Node *p = head_;
        while (p->forwards[0] != nullptr) {
            std::cout << " " << p->forwards[0]->data;
            p = p->forwards[0];
        }
        cout << endl;
    }

private:
    Node *head_; // 底层链表的头结点
    const int max_level_ = MAX_LEVEL; // 即 [0 - 16)
    int current_max_level_; // 当前跳表内最大索引层数，有效层数是其 - 1
}; // class SkipList

} // namespace glib

#endif
