/*
 * CopyRight (c) 2019 gcj
 * File: hash_table.hpp
 * Project: algorithm
 * Author: gcj
 * Date: 2019/5/30
 * Description: hash_table simple implementation
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#ifndef GLIB_HASH_TABLE_HPP_
#define GLIB_HASH_TABLE_HPP_
#include <iostream>
#include <string>
#include <functional> // 使用 std::hash 函数
#include <typeinfo>   // 用来判断类型

//! \brief 简单实现哈希表——拉链法
//!     外部调用核心函数：
//!         1）往哈希表中添加一个数据：Insert()，两种插入方法
//!         2）从哈希表中删除一个数据：Delete()
//!         3）在哈希表中查找一个数据：Find()
//!         4）设置 LRU 缓存大小：set_size()
//!     外部调用状态函数：
//!         1）打印哈希表数据：print_value()
//!         2）哈希表状态：size()、empty()、capacity()、max_load_factor()、min_load_factor()
//!     内部辅助核心函数：
//!         1）调节底层哈希容量：AdjustCapacity()
//!         2）查询函数：FindInertial()
//!
//! \Note
//!     1）仅适用于内置数据类型，比如 string int ...
//!     2）只能通过 Insert() 函数来修改 key 对应的 value。参考如下 example
//!
//! \TODO
//!     1）扩容底层哈希表规的则需要修改，下面使用的是一次性扩容-低效。
//!
//! \platform
//!     ubuntu16.04 g++ version 5.4.0
//!
//! \reference
//!         1）std::hash--->https://en.cppreference.com/w/cpp/utility/hash
//!
//! example
//!     1）通过 Insert() 函数进行修改内部元素
//!         HashTable<string, string> hash_table;
//!         hash_table.Insert(std::make_pair("1","a"));
//!         hash_table.Insert(std::make_pair("2", "b"));
//!         auto target = hash_table.Find("2");
//!         if (target.first)
//!             hash_table.Insert(std::make_pair("2", 修改想要的值));

namespace glib {

template <typename _Key, typename _Value, typename _Hash = std::hash<_Key> >
class HashTable {
public: // 类型、结构声明
    using KeyType    = _Key;
    using MappedType = _Value;
    using Hasher     = _Hash;

    // 哈希表内部存储的数据
    struct HashData {
        KeyType key;        // 键值
        MappedType value;   // 映射值
    };

    struct HashNode {
        HashData data;
        HashNode *h_next;
    };

public: // 构造函数相关
    // 默认容量是 2 的指数次幂
    explicit
    HashTable(size_t capacity = 8, double max_load_factor = 0.75f, double min_load_factor = 0.125f)
        : capacity_(capacity), max_load_factor_(max_load_factor),
          min_load_factor_(min_load_factor), current_size_(0) {
        if (capacity_ < min_capacity_)
            capacity_ = min_capacity_;
        array_ = new HashNode*[capacity_];
        for (size_t i = 0; i < capacity_; i++) {
            array_[i] = nullptr;
        }
    }

    ~HashTable() {
        if (nullptr != array_) {
            for (size_t i = 0; i < capacity_; i++) {
                if (nullptr != array_[i]) {
                    // 对每个桶的节点释放空间
                    HashNode *head = array_[i];
                    while (nullptr != head->h_next) {
                        HashNode *temp = head->h_next;
                        delete head;
                        head = temp;
                    }
                    delete head;
                }
            }
            delete[] array_; // 释放数组
        }
    }

    HashTable(const HashTable &other) = delete;
    HashTable(HashTable &&other) = delete;
    HashTable& operator=(const HashTable &other) = delete;
    HashTable& operator=(HashTable &&other) = delete;

public: // 外部调用核心函数
    //! \brief 按照键值查询给定数据，不可修改内部数据
    //! \complexity O(1)
    //! \return 查询信息，first:是否成功找到，second:成功找到后，根据 second.data.value 查询对应值
    std::pair<bool, MappedType> Find(const KeyType key) const {
        auto result = FindInertial(key);
        return std::make_pair(result.first,
                                (result.first? result.second->data.value : MappedType())
                             );
    }

    //! \brief 在哈希表中插入指定数据，按照键值对进行插入
    //! \complexity O(1)
    void Insert(const std::pair<KeyType, MappedType> &data) {
        size_t hash_index = hasher_(data.first) & (capacity_ - 1);
        bool find_flag = false;
        HashNode *head = array_[hash_index];
        HashNode *pre_head = array_[hash_index];
        while (nullptr != head) {
            if (data.first == head->data.key) {  // 找到对应的 key，那么此时直接替换相应的键值
                find_flag = true;
                head->data.value = data.second;
                break;
            }
            pre_head = head;
            head = head->h_next;
        }

        // 在拉链中没有找到该关键值 key，那么在拉链尾部插入新节点
        if (!find_flag) {
            current_size_++;
            HashNode *new_node = new HashNode;
            new_node->data.key = data.first;
            new_node->data.value = data.second;
            new_node->h_next = nullptr;          // 尾部节点要指向空
            if (nullptr == array_[hash_index]) { // 刚开始，该索引对应的桶没有链，直接把新节点插入即可
                array_[hash_index] = new_node;
            } else {
                pre_head->h_next = new_node;
            }
        }

        // 动态扩充底层容量-一次性扩容
        if (load_factor() > max_load_factor_) {
            expand_or_shrink_ = true;
            AdjustCapacity();
        }
    }
    // 同上另一种插入方法
    void Insert(const KeyType &key, const MappedType &value) {
        Insert(std::make_pair(key, value));
    }

    //! \brief 在哈希表中删除指定值
    //! \complexity average case O(1)
    void Delete(const KeyType &key) {
        auto find_result = FindInertial(key);
        if (find_result.first) {
            // 表示哈希表中存在该数据，直接遍历删除即可
            current_size_--;
            auto hash_index = hasher_(key) & (capacity_ - 1);
            HashNode *head = array_[hash_index];
            HashNode *pre_head = head;
            while (nullptr != head) {
                if (head->data.key == key) {
                    // 正式删除该节点
                    if (head == array_[hash_index]) {
                        array_[hash_index] = head->h_next;
                    } else {
                        pre_head->h_next = head->h_next;
                    }
                    delete head;
                    break;
                }
                pre_head = head;
                head = head->h_next;
            }
        }

        // 判断是否需要缩容
        if (load_factor() <= min_load_factor_) {
            expand_or_shrink_ = false;
            AdjustCapacity();
        }
    }

    // 打印哈希表内容（无序打印）
    void print_value() const {
        std::cout << "print start:" << std::endl;
        if (current_size_ > 0) {
            for (size_t i = 0; i < capacity_; i++) {
                HashNode *head = array_[i];
                while (nullptr != head) {
                    std::cout << "key: " << head->data.key << " "
                              << "index: " << (hasher_(head->data.key) & (capacity_ - 1)) << " "
                              << "value: " << head->data.value << std::endl;
                    head = head->h_next;
                }
            }
        } else {
            std::cout << "哈希表为空!" << std::endl;
        }
        std::cout << "print end." << std::endl;
    }

    size_t size()            const { return current_size_;                            } // 当前哈希表数据量
    size_t capacity()        const { return capacity_;                                } // 当前哈希表底层数组容量
    bool   empty()           const { return (current_size_ > 0 ? false : true);       } // 当前哈希表是否为空
    double load_factor()     const { return static_cast<double>(size()) / capacity(); } // 返回当前装载因子大小
    double max_load_factor() const { return max_load_factor_;                         } // 最大装载因子
    double min_load_factor() const { return min_load_factor_;                         } // 最小装载因子

private: // helper functions
    //! \brief 动态扩充底层哈希表容量
    //! \note 这里装载因子定义为（当前哈希表已存量/哈希表容量）
    //! \complexity average case:O(1) 空间复杂度 O(2*capacity_ or capacity_/2) = O(capacity_)
    //! \TODO
    //!     1）修改当前低效的扩容方法——一次性扩容，变为专栏中高效扩容
    void AdjustCapacity() {
        // 容量太大需要动态扩容，容量小需要缩减容量。都是按照 2 的倍数扩容和缩容。缩容不能小于 min_capacity_
        HashNode ** temp;
        decltype(capacity_) origin_capacity = capacity_;
        if (expand_or_shrink_) {
            capacity_ *= 2;
            temp = new HashNode*[capacity_];
        } else {
            capacity_ /= 2;
            if (capacity_ < min_capacity_)
                capacity_ = min_capacity_;
            if (capacity_ == origin_capacity)
                return;
            temp = new HashNode*[capacity_];
        }

        for (size_t i = 0; i < capacity_; i++) {
            temp[i] = nullptr;
        }

        // 将原来所有数据从新计算进行搬移到新的底层哈希表
        for (size_t i = 0; i < origin_capacity; i++) {
            HashNode *head = array_[i];
            while (nullptr != head) {
                auto hash_index = hasher_(head->data.key) & (capacity_ - 1);
                HashNode *next_hash_node = head->h_next;
                if (nullptr == temp[hash_index]) {
                    temp[hash_index] = head;
                    head->h_next = nullptr;
                } else {
                    HashNode *temp_head = temp[hash_index];
                    while (nullptr != temp_head->h_next) {
                        temp_head = temp_head->h_next;
                    }
                    temp_head->h_next = head;
                    head->h_next = nullptr;
                }
                head = next_hash_node;
            }
        }
        delete[] array_;
        array_ = temp;
    }

    //! \brief 按照键值在哈希表中进行查询
    //! \complexity O(1)
    //! \note 通过返回的指针可以查看和修改对应节点值
    //! \param key 目标数据
    //! \return 查询信息，first:是否成功找到，second:成功的节点地址。进而可以修改指向的内容
    std::pair<bool, const HashNode*> FindInertial(const KeyType &key) const {
        size_t hash_index = hasher_(key) & (capacity_ - 1);
        bool find_flag = false;
        HashNode *head = array_[hash_index];
        while (nullptr != head) {
            if (key == head->data.key) {
                find_flag = true;
                break;
            }
            head = head->h_next;
        }
        return std::make_pair(find_flag, (find_flag? head: nullptr));
    }

    // // 处理 string 对象哈希码的原理，目前内部设计原理不清楚
    // size_t HashCode(const std::string &key) const {
    //     size_t string_code = 0;
    //     if (key.size() > 0) {
    //         for (size_t i = 0; i < key.size(); i++) {
    //             string_code = string_code * 31 + key[i];
    //         }
    //     }
    //     return string_code;
    // }
    //
    // //! \brief 一个散列函数，返回底层数组索引
    // size_t HashFunction(const KeyType &key) const {
    //     size_t hash = HashCode(key);
    //     return (hash ^ (hash >> 16)) & (capacity_ - 1);
    // }

private:
    const size_t min_capacity_ = 8;        // 默认最小容量
    const Hasher hasher_       = Hasher(); // 默认构造一个哈希对象，使用 stl 提供的计算哈希值
    bool  expand_or_shrink_    = false;    // true: 表示扩容， false 表示缩容
    HashNode** array_;            // 哈希表底层数组，保存链表头
    double     max_load_factor_;  // 最大装载因子 0.75
    double     min_load_factor_;  // 最小装载因子为 0.125
    size_t     capacity_;         // 哈希表的容量 默认 16
    int        current_size_;     // 当前哈希表中元素的数量

}; // class HashTable

} // namespace glib

#endif // GLIB_HASH_TABLE_HPP_
