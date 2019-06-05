/*
 * CopyRight (c) 2019 gcj
 * File: lru_hash.hpp
 * Project: algorithm
 * Author: gcj
 * Date: 2019/5/30
 * Description: lru by hash simple implementation
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#ifndef GLIB_LRU_HASH_HPP_
#define GLIB_LRU_HASH_HPP_
#include <iostream>
#include <functional> // std::hash<>
#include "assert.h"   // assert()

//! \brief 利用哈希表和双链表实现 LRU 缓存淘汰算法
//!     外部调用核心函数：
//!         1）往缓存中添加一个数据：Insert()
//!         2）从缓存中删除一个数据：Delete()
//!         3）在缓存中查找一个数据：Find()
//!         4）设置 LRU 缓存大小：set_size()
//!     外部调用状态函数：
//!         1）Debug 函数：debug_print_value()
//!         2）打印 LRU 内部数据：print_value()
//!         3）LRU 状态：size()、empty()、capacity()
//!         4）底层哈希表状态：max_load_factor()、min_load_factor()、hash_capacity()、load_factor()
//!     内部辅助核心函数：
//!         1）调节底层哈希容量：AdjustCapacity()
//!         2）将指定节点移到双链表头：MoveListHead()
//!         3）查询函数：FindInertial()
//!     内部辅助状态函数：
//!         1）按照哈希表顺序进行打印：print_value_by_hash()
//!         2）按照双链表顺序进行打印：print-value_by_list()
//!
//! \Note
//!     1）底层哈希存储的数据依然是 key 和 value。并且 key == value，这样会浪费点空间。
//!     2）仅适用于内置数据类型，比如 string int ...
//!
//! \TODO
//!     1）扩容底层哈希表规的则需要修改，下面使用的是一次性扩容-底层。
//!     2）在设置底层哈希表容量时，需要找到最接近 2 的幂次值。保证索引均匀分布。有如下两部分需要修改处：
//!        1.在初始构造哈希时 2.在调节哈希表容量时，扩容和缩容，都需要调节容量。
//!
//! \platform
//!     ubuntu16.04 g++ version 5.4.0

namespace glib {

template <typename _Key, typename _Hash = std::hash<_Key> >
class LruHash {
public: // 类型声明
    using _Value     = _Key;
    using KeyType    = _Key;
    using MappedType = _Value;
    using Hasher     = std::hash<_Key>;

    // 哈希表内部存储的数据，包含了键值及其对应的数据
    //! \note 此时 key 和 value 是一样的值！
    struct HashData {
        KeyType key;
        MappedType value;
    };

    struct HashNode {
        HashNode *pre;    // 双链表的前一个节点
        HashData data;
        HashNode *next;   // 双链表的下一个节点
        HashNode *h_next; // 拉链中的下一个节点
    };

public: // 构造函数相关
    // 默认容量是 2 的指数次幂
    explicit
    LruHash(size_t max_lru_size = 8)
        : max_lru_size_(max_lru_size), current_size_(0),
          list_head_(nullptr), list_tail_(nullptr), expand_or_shrink_(false) {
        assert(max_lru_size >= 1);
        if (max_lru_size_ <= max_load_factor_*min_capacity_)
            hash_capacity_ = min_capacity_; // 8
        else
            hash_capacity_ = max_lru_size_ * 2; //TODO 这里需要做一下修改，变为 2 的整数次幂
        array_ = new HashNode*[hash_capacity_];
        for (size_t i = 0; i < hash_capacity_; i++) {
            array_[i] = nullptr;
        }
    }

    ~LruHash() {
        // 通过哈希表来释放空间，也可以通过修改成双链表释放空间
        if (nullptr != array_) {
            for (size_t i = 0; i < hash_capacity_; i++) {
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

    LruHash(const LruHash &other) = delete;
    LruHash(LruHash &&other) = delete;
    LruHash& operator=(const LruHash &other) = delete;
    LruHash& operator=(LruHash &&other) = delete;

public: // 外部调用函数
    //! \brief 按照键值在哈希表中进行查询
    //! \complexity O(1)
    //! \param key 目标数据
    //! \return 查询信息，first:是否成功找到，second:成功找到后，根据 second.data.value 查询对应值
    std::pair<bool, MappedType> Find(const KeyType &key) const {
        auto result = FindInertial(key);
        return std::make_pair(result.first,
                                (result.first? result.second->data.value : MappedType())
                             );
    }

    //! \brief LRU 插入
    //! \note 新来的数据放到双链表头部
    //! \complexity O(1)
    //! \param key 目标数据
    void Insert(const KeyType &key) {
        auto find_result = FindInertial(key);

        if (find_result.first) {                     // 链表中存在该数据，此时只需要将该节点移动到双链表头部即可
            MoveListHead(find_result.second);
            return;
        } else if (current_size_ == max_lru_size_) { // 达到最大节点数量，需要删除（哈希表和双链表）最后一个节点，之后在头部插入新节点
            Delete(list_tail_->data.key);
        }

        // 此时没有在哈希表中找到给定的数据，需要插入数据：同时在哈希表结构和双链表结构进行更新
        // 这里有两种情况：
        //      1）达到了缓存的上限，此时需要在头部插入新节点。
        //      2）没有找到给定数据，同时没有达到上限，直接插入新节点。
        size_t hash_index = hasher_(key) & (hash_capacity_ - 1);
        bool find_flag = false;
        HashNode *head = array_[hash_index];
        HashNode *pre_head = array_[hash_index];
        while (nullptr != head) {
            if (key == head->data.key) {             // 此时这里不会找到！上面已经用 Find() 查找过了
                find_flag = true;
                break;
            }
            pre_head = head;
            head = head->h_next;
        }

        // 在拉链中没有找到该关键值 key，那么在拉链尾部和双链表尾部插入新节点
        if (!find_flag) {
            HashNode *new_node = new HashNode;
            new_node->data.key = key;                // 这里对于缓存结构来说，存储的关键字和关键值是一样的
            new_node->data.value = key;
            new_node->h_next = nullptr;              // 尾部节点要指向空

            // 在拉链中插入新节点
            if (nullptr == array_[hash_index]) {     // 刚开始，该索引对应的桶没有链，直接把新节点插入即可
                array_[hash_index] = new_node;
            } else {
                pre_head->h_next = new_node;
            }

            // 在双向链表中插入新节点
            new_node->next = list_head_;             // 如果 current_size_=0，此时这里直接是 nullptr
            new_node->pre = nullptr;
            if (0 == current_size_) {
                list_head_ = new_node;
                list_tail_ = new_node;
            } else {
                list_head_->pre = new_node;
                list_head_ = new_node;
            }

            // 更新当前节点数
            current_size_++;
        }
    }

    //! \brief LRU 删除某个数据（在双向链表和哈希表中同时删除）
    //! \complexity O(1)
    //! \param key 目标数据
    void Delete(const KeyType &key) {
        auto find_result = FindInertial(key);
        if (find_result.first) {
            // 哈希表中存在该数据，直接遍历删除即可
            auto hash_index = hasher_(key) & (hash_capacity_ - 1);
            HashNode *head = array_[hash_index];
            HashNode *pre_head = head;
            while (nullptr != head) {
                if (head->data.key == key) {
                    // 正式删除该节点——哈希表中删除，这里处理单链表时搞错过！
                    if (head == array_[hash_index]) {
                        array_[hash_index] = head->h_next;
                    } else {
                        pre_head->h_next = head->h_next;
                    }

                    // 在双向链表中进行删除
                    if (1 == current_size_) {
                        list_head_->next = nullptr;
                        list_head_->pre = nullptr;
                        list_head_ = nullptr;
                        list_tail_ = nullptr;
                    } else if (find_result.second == list_head_) { // 至少有两个节点
                        list_head_ = list_head_->next;
                        list_head_->pre = nullptr;
                    } else if (find_result.second == list_tail_) { // 至少有两个节点
                        list_tail_ = list_tail_->pre;
                        list_tail_->next = nullptr;
                    } else { // 至少三个节点，且不在头尾
                        find_result.second->pre->next = find_result.second->next;
                        find_result.second->next->pre = find_result.second->pre;
                    }

                    // 在内存中删除该节点
                    delete head;
                    break;
                }
                pre_head = head;
                head = head->h_next;
            }
            current_size_--;
        }
    }

    //! \brief 动态设置 LRU 大小，并且适当扩容和缩容哈希容量
    //! \note 如果当前缓存中现有数据大于设置后的缓存容量上限，那么需要删除一些老的数据
    //! \complexity average case:O(1) 空间复杂度 O(2*hash_capacity) = O(hash_capacity)
    //! \param lru_size 指定缓存大小
    void set_size(size_t lru_size) {
        max_lru_size_ = lru_size;
        std::cout << "set lru size: " << max_lru_size_ << std::endl;
        if (current_size_ > max_lru_size_) {
            int max_index = current_size_ - max_lru_size_;
            // 需要遍历删除双向链表的后几位
            for (int i = 0; i < max_index; i++) {
                Delete(list_tail_->data.key);
            }
        }

        // 判断是否需要调节哈希表容量
        if (static_cast<double>(max_lru_size_)/hash_capacity_ <= min_load_factor()) {
            expand_or_shrink_ = false;
            AdjustCapacity();
        } else if (static_cast<double>(max_lru_size_)/hash_capacity_ > max_load_factor()) {
            expand_or_shrink_ = true;
            AdjustCapacity();
        }
    }

    // 调试打印输出，分别按照哈希表和双链表顺序
    void debug_print_value() const {
        std::cout << "debug_print_value start:" << std::endl;
        std::cout << "按照哈希表打印：" << std::endl;
        print_value_by_hash();

        std::cout << "按照双链表顺序打印：" << std::endl;
        print_value_by_list();

        std::cout << "双链表头尾数据:" << std::endl;
        std::cout << list_head_->data.key << " " << list_head_->data.value << std::endl;
        std::cout << list_tail_->data.key << " " << list_tail_->data.value << std::endl;
        std::cout << "debug_print_value end." << std::endl;
    }

    // 打印 lru 数据，顺序打印！
    void print_value() { print_value_by_list(); }

    // lru 相关
    size_t size()            const { return current_size_;                            } // 当前 lru 数据量
    bool   empty()           const { return (current_size_ > 0 ? false : true);       } // 当前 lru 是否为空
    size_t capacity()        const { return max_lru_size_;                            } // lru 容量

    // 哈希表相关
    double max_load_factor() const { return max_load_factor_;                         } // 最大装载因子
    double min_load_factor() const { return min_load_factor_;                         } // 最小装载因子
    size_t hash_capacity()   const { return hash_capacity_;                           } // 哈希容量
    double load_factor()     const { return static_cast<double>(hash_capacity_)/capacity();} // 返回当前装载因子大小：lru大小/哈希大小

private: // helper functions
    //! \brief 动态扩充底层哈希表容量
    //! \note 这里装载因子定义为（LRU 容量/哈希表容量）且这里扩容仅仅对底层的哈希表起作用，双链表不需要修改
    //! \complexity average case:O(1) 空间复杂度 O(2*hash_capacity or hash_capacity/2) = O(hash_capacity)
    //! \TODO
    //!     1）修改哈希容量时，需要其值为最接近 2 的整数次幂
    //!     2）修改当前低效的扩容方法——一次性扩容，变为专栏中高效扩容
    void AdjustCapacity() {
        std::cout << "AdjustCapacity()->";
        // 容量太大需要动态扩容，容量小需要缩减容量。都是按照 2 的倍数扩容和缩容。缩容不能小于 min_capacity_
        HashNode ** temp;
        decltype(hash_capacity_) origin_capacity = hash_capacity_;
        if (expand_or_shrink_) {
            hash_capacity_ = max_lru_size_*2;
            temp = new HashNode*[hash_capacity_];
            std::cout<< "扩容" << std::endl;
        } else {
            if (max_lru_size_ <= max_load_factor_*min_capacity_)
                hash_capacity_ = min_capacity_; // 8
            else
                hash_capacity_ = max_lru_size_ * 1.5; // > 4/3 TODO 需要改为最接近 2 的整数次幂
            if (hash_capacity_ == origin_capacity)
                return;
            temp = new HashNode*[hash_capacity_];
            std::cout << "缩绒" << std::endl;
        }

        for (size_t i = 0; i < hash_capacity_; i++) {
            temp[i] = nullptr;
        }

        // 将原来所有数据从新计算进行搬移到新的底层哈希表
        for (size_t i = 0; i < origin_capacity; i++) {
            HashNode *head = array_[i];
            while (nullptr != head) {
                auto hash_index = hasher_(head->data.key) & (hash_capacity_ - 1);
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

    //! \brief 将指定节点移动到双链表头部
    //! \complexity O(1)
    //! \param target 将要移动的目标节点
    void MoveListHead(HashNode * target) {
        // 分为 3 种情况，1）只有一个节点，或者找到的就是头节点 2）找到的节点是尾部节点 3）找到的节点是中间某个节点
        if (1 == current_size_ or target == list_head_)
            return;

        if (target == list_tail_) { // 此时至少 2 个节点
            // 交换头尾节点
            HashNode *temp1 = list_tail_->pre;
            temp1->next = nullptr;
            // 反向连接头尾节点
            list_head_->pre = list_tail_;
            list_tail_->next = list_head_;
            // 改变头尾指针
            list_head_ = list_tail_;
            list_tail_ = temp1;
            list_head_->pre = nullptr;
            return;
        }

        // 此时至少 3 个节点
        // 目标节点前后连接
        target->pre->next = target->next;
        target->next->pre = target->pre;
        // 头和目标节点连接
        target->next = list_head_;
        list_head_->pre = target;
        // 改变头指针
        list_head_ = target;
        list_head_->pre = nullptr;
    }

    //! \brief 按照键值在哈希表中进行查询
    //! \complexity O(1)
    //! \note 通过返回的指针可以查看和修改对应节点值
    //! \param key 目标数据
    //! \return 查询信息，first:是否成功找到，second:成功的节点地址，进而可以修改指向的内容
    std::pair<bool, HashNode*> FindInertial(const KeyType &key) const {
        size_t hash_index = hasher_(key) & (hash_capacity_ - 1);
        bool find_flag = false;
        HashNode *head = array_[hash_index];
        while (nullptr != head) {
            if (key == head->data.key) { // 这里可以用比较函数
                find_flag = true;
                break;
            }
            head = head->h_next;
        }
        return std::make_pair(find_flag, (find_flag? head : nullptr));
    }

    // 按照哈希表顺序打印
    void print_value_by_hash () const {
        if (current_size_ > 0) {
            for (size_t i = 0; i < hash_capacity_; i++) {
                HashNode *head = array_[i];
                while (nullptr != head) {
                    std::cout << "key: " << head->data.key << " "
                              << "index: " << (hasher_(head->data.key) & (hash_capacity_ - 1)) << " "
                              << "value: " << head->data.value << std::endl;
                    head = head->h_next;
                }
            }
        } else {
            std::cout << "哈希表为空!" << std::endl;
        }
    }

    // 按照双链表顺序打印
    void print_value_by_list() const {
        HashNode *temp = list_head_;
        while (nullptr != temp) {
            std::cout << " " << temp->data.value;
            temp = temp->next;
        }
        std::cout << std::endl;
    }

private:
    // 底层哈希表参数
    const size_t min_capacity_    = 8;        // 默认最小容量
    const double min_load_factor_ = 0.125;    // 最小装载因子为 0.125
    const double max_load_factor_ = 0.75;     // 最大装载因子 0.75
    const Hasher hasher_          = Hasher(); // 默认构造一个哈希对象，使用 stl 提供的计算哈希值
    bool  expand_or_shrink_;      // true: 表示扩容， false 表示缩容
    HashNode** array_;            // 哈希表底层数组，保存链表头
    size_t     hash_capacity_;    // 哈希表的容量 = max_lru_size_*2
    int        current_size_;     // 当前哈希表中元素的数量，也是 LRU 当前有效元素个数

    // 底层双向链表参数
    HashNode*  list_head_; // 记录双链表头节点
    HashNode*  list_tail_; // 记录双链表尾部节点

    // Lru 参数
    size_t max_lru_size_;  // 设置 LRU 大小，进而调节哈希表相关变量
}; // LruHash

} // namespace glib

#endif // GLIB_LRU_HASH_HPP_
