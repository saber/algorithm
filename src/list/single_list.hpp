/*
 * CopyRight (c) 2019 gcj
 * File: single_list.hpp
 * Project: algorithm
 * Author: gcj
 * Date: 2019/4/8
 * Description: single linked list simple implementation
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#ifndef GLIB_SINGLE_LIST_HPP_
#define GLIB_SINGLE_LIST_HPP_

#include <iostream>
#include <vector>
#include <assert.h>

//! \brief 实现了单链表的功能
//!        基本功能：
//!           1）插入：包含头部、尾部、任意位置插入。
//!               对应函数：InsertHead、InsertTail、Insert
//!           2）删除：删除头部节点、尾部节点、按值删除（删除全部值或者删除第一个值）、按指针删除。
//!               对应函数：DeleteHead、DeleteTail、Delete(data, flag)、Delete(node*)
//!           3）查找函数：按数据值查找返回对应节点、按指针查找是否节点在当前链表中。
//!               对应函数：Find(data)、Find(node*)
//!           4）移动：交换两个指定节点、指定节点移动到第一个节点处、
//!               对应函数：Exchange(node1*, node2 *) 、MoveHead(node*)
//!           5）其他普通函数：返回当期链表被引用数量、链表对应节点数量、链表是否为空、打印链表值、
//!                    返回头尾指针、返回第一个节点指针、
//!               对应函数： reference_count、 node_count、empty、print_value、
//!                   head_ptr、tail_ptr、first_node_ptr、
//!
//!        高级功能：
//!           1）单链表反转：Reserve
//!           2）求取单链表中间节点：MiddleNode
//!           3）链表检测环：CheckCircleInSingleList
//!
//!        高级应用：
//!           1）LRU(最近最少使用)缓存淘汰算法单链表实现：LRUBySingleList
//!           2）判断字符串是否是回文串（假定是单链表存储）：IsPalindromeString
//!             包含两种方法：递归实现 & 快慢指针法实现
//!
//! \Note
//!     1）不支持移动赋值、移动构造
//!     2）该单链表的实现利用哨兵——带头结点，简化了处理边界条件。默认构造时，头指针指向头结点
//!     3）使用 SingleList 提供的函数时，可以通过上面 \brief 中提供的函数名，对应查看下面函数实现处。
//!        在相应功能的实现处，已经标注了函数的具体使用方法和注意事项。或者直接参照 use_single_list.cc
//!        文件提供的测试例子，对应测试代码，可以快速明白相应函数接口的使用。
//!     4）该单链表的实现中，仿照了智能指针的共享功能，也可以用来多个单链表的拷贝构造以及赋值构造。不会造成内存泄露
//!     5）本链表只测试基本的内置类型可以通过，没有完全测试 string、vector 是否也能通过。如果失败，麻烦请告知！
//!
//! \TODO
//!      1）支持移动赋值、移动构造
//!      2）链表的排序功能、双链表及其循环链表的基本实现
//!      3）融合两个有序的单链表，单链表排序算法。—— 参考数组实现那里，实现思路其实与 array.hpp 那里一致
//!      4）删除链表倒数第 n 个节点、删除序号为 i 的节点
//!      5）leetCode: 206、141、21、19、876
//!
//! \platform
//!      ubuntu16.04 g++ version 5.4.0

namespace glib {
using namespace std;

template <typename T = int>
class SingleList {
public:
    typedef T type;
    // 节点
    struct Node {
        T data;
        Node *next;
    };

    // 空节点
    SingleList() : node_count_(0), head_(nullptr), tail_(nullptr){
        reference_count_ = new size_t(1);
        head_ = new Node;
        head_->data = T();
        head_->next = nullptr;
    }

    //! \brief 将一组相同类型数据存储到链表中，
    //! \param i :节点数量
    //! \param values: 存储节点值
    SingleList(const size_t i, const vector<T> &values)
        : node_count_(i), head_(nullptr), tail_(nullptr) {
        assert(values.size() == i); // 初始值与给定节点数量相同
        reference_count_ = new size_t(1);
        head_ = new Node;           // 头指针指向头结点
        head_->data = T();
        head_->next = nullptr;      // 保证不建立节点也安全
        Node *p = head_;
        for (size_t idx = 0; idx != i; idx++) {
            p->next = new Node;
            p->next->data = static_cast<T>(values[idx]);
            p = p->next;
            p->next = nullptr;      // 保证最后一个节点指向空
        }
        if (i != 0)
            tail_ = p; // 保留尾指针指向最后一个节点
    }

    // 拷贝构造
    SingleList(const SingleList &obj) : head_(obj.head_), tail_(obj.tail_),
                                        node_count_(obj.node_count_) {
        if (this != &obj) {
            reference_count_ = obj.reference_count_;
            (*reference_count_)++;
        }
    }

    // 赋值
    SingleList& operator=(const SingleList &obj) {
        if (this == &obj)
            return *this;
        (*reference_count_)--;      // 左值需要递减计数器
        if (!(*reference_count_)) { // 需要清理资源
            //清理资源
            Node *p = head_->next;  // 第一个节点
            Node *temp = p;
            while (p != nullptr) {
                temp = p;
                p = p->next;
                delete temp;
            }
            tail_ = nullptr;
            node_count_ = 0;
            delete head_;
            delete reference_count_;
        }

        // 引用 obj 资源
        head_ = obj.head_;
        tail_ = obj.tail_;
        node_count_ = obj.node_count_;
        reference_count_ = obj.reference_count_;
        (*reference_count_)++;
        return *this;
    }

    // 析构
    ~SingleList() {
        (*reference_count_)--;
        if (!(*reference_count_)) {
            // 清理资源
            Node *p = head_->next; // 第一个节点
            Node *temp = p;
            while (p != nullptr) {
                temp = p;
                p = p->next;
                delete temp;
            }
            // 删除头结点
            delete head_;
            delete reference_count_;
            node_count_ = 0;
        }
    }

    size_t reference_count() const { return *reference_count_; } // 返回链表被引用数量
    int    node_count()      const { return node_count_;       } // 返回当前链表节点数量
    bool   empty()           const { return !node_count_;      } // 判断链表是否为空

    //! \brief 打印链表节点数据
    //! \complexity: O(n)
    void print_value() {
        Node *p = head_;
        while (p->next != nullptr) {
            cout << p->next->data << " ";
            p = p->next;
        }
        cout << endl;
    }

    //! \brief 在头部插入数据
    //! \complexity: O(1)
    //! \return 返回节点地址
    Node* InsertHead(const T &data) {
        Node *p = head_->next;
        head_->next = new Node;
        head_->next->next = p;
        head_->next->data = static_cast<T>(data);

        // 链表原来没有节点，需要改变尾指针
        if (node_count_ == 0 && tail_ == nullptr)
            tail_ = head_->next; // 不要忘记保留尾部指针
        node_count_++;
        return head_->next;
    }

    //! \brief 在尾部插入数据
    //! \complexity: O(1)
    Node* InsertTail(const T &data) {
        // 链表中没有节点
        if (node_count_ == 0 && head_->next == nullptr && tail_ == nullptr) {
            head_->next = new Node;
            head_->next->data = static_cast<T>(data);
            head_->next->next = nullptr;
            tail_ = head_->next; // 不要忘记保留尾部指针
        } else {
            tail_->next = new Node;
            tail_->next->data = static_cast<T>(data);
            tail_ = tail_->next;
        }
        node_count_++;
        return tail_;
    }

    //! \brief 在指定位置前插入数据(默认在头部插入)[1,n]
    //! \note 如果插入位置超出索引，那么默认从头部插入数据且返回值为 pair< bool = false，Node*=head_->next>
    //!      正常插入 bool=true, Node* 是插入节点的地址。
    //! \complexity: best case: O(1) worst case: O(n) average case: O(n)
    pair<bool, Node*> Insert(const T &data, const size_t idx = 1) {
        assert(idx > 0);
        if (idx > node_count_) {
            return make_pair<bool, Node*>(false, InsertHead(data));
        }

        // 此时 node_count_>0
        Node *p = head_;
        for (size_t i = 0; i < idx-1; i++) {
            p = p->next;
        }
        Node *after = p->next;
        p->next = new Node;
        p->next->data = static_cast<T>(data);
        p->next->next = after;
        node_count_++;
        return make_pair(true, p->next); // make_pair<bool, Node*> 用法错误！
    }

    //! \brief 链表中删除节点---By 值
    //! \note 链表内部多个节点对应该值时，默认全部删除，可以通过下面的选择进行切换
    //! \complexity: best case: O(1) worst case: O(n) average case: O(n)
    //! \param data 在链表中按给定值进行查找
    //! \param flag  true:  表示仅仅删除第一个值为 data 的节点
    //!              false: 删除全部值为 data 的节点
    //! \return 如果该链表中含有该值，那么删除对应的节点后返回真。
    //!         如果链表中没有该值，则表示删除失败，返回假
    bool Delete(const T &data, const bool flag = false) {
        bool return_flag = false;
        if (node_count_ == 0 && head_->next == nullptr && tail_ == nullptr)
            return return_flag;
        Node *p = head_->next;
        Node *prev = head_;
        for (size_t i = 0; i < node_count_ && p != nullptr; i++) {
            if (p->data == static_cast<T>(data)) {
                return_flag = true;

                // 删除 p 指向的节点
                prev->next = p->next;
                node_count_--;

                // 修改尾部节点指针
                if (node_count_ == 0) // 此时满足链表初状态化状态！
                    tail_ = nullptr;  // tail_ =nullptr,node_count_=0,head_->next=nullptr
                else if(p == tail_)   // 恰好删除尾部节点
                    tail_ = prev;

                // 清理资源
                delete p;
                p = prev->next;
                if (flag == true)
                    break;
            } else {
                prev = p;
                p = p->next;
            }
        }
        return return_flag;
    }

    //! \brief 删除节点---By 指针
    //! \complexity: best case: O(1) worst case: O(n) average case: O(n)
    bool Delete(const Node *node) {
        if (node == nullptr)
            return false;
        bool delete_flag = false;
        if (node_count_ == 0)
            return delete_flag;
        if (node == tail_ && node_count_ == 1) {
            head_->next = nullptr;
            tail_ = nullptr;
            node_count_--;
            delete node;
            return true;
        }
        Node *p = head_;
        while (p->next != nullptr) {
            if (p->next == node) {
                if (node == tail_) // 修改尾部节点指针
                    tail_ = p;
                p->next = p->next->next;
                node_count_--;
                delete node;
                delete_flag = true;
                break;
            } else {
                p = p->next;
            }
        }
        return delete_flag;
    }

    bool  DeleteHead()           { return Delete(head_->next); } // 头部删除节点 O(1)
    bool  DeleteTail()           { return Delete(tail_);       } // 尾部删除节点 O(n)
    Node* head_ptr()       const { return head_;               } // 返回头指针 O(1)
    Node* tail_ptr()       const { return tail_;               } // 返回尾部指针 O(1)
    Node* first_node_ptr() const { return head_->next;         } // 返回第一个节点指针 O(1)

    //! \brief 在链表中寻找指定数据对应的节点
    //! \complexity: O(n)
    //! \return vector<Node*> 数据对应的所有节点(vector<Node*>::size() 包含了节点个数)
    vector<Node*> Find(const T &data) const {
        vector<Node*> return_nodes;
        Node *p = head_;
        while (p->next != nullptr) {
            if (p->next->data == data) {
                return_nodes.push_back(p->next);
            }
            p = p->next;
        }
        return return_nodes;
    }

    //! \brief 查询节点是否存在当前链表中
    //! \complexity: best case: O(1) worst case: O(n) average case: O(n)
    bool Find(const Node *node) const {
        if (node == nullptr)
            return false;
        Node *p = head_;
        while (p->next != nullptr) {
            if (p->next == node) {
                return true;
            }
            p = p->next;
        }
        return false;
    }

    //! \brief 任意交换两个节点位置（不需要自己指定顺序）
    //! \return false: 两个节点至少一个不在链表中 true:交换成功
    //! \complexity: best case: O(1) worst case: O(n) average case: O(n)
    bool Exchange(Node *node1, Node *node2) {
        // 检查两个节点是否存在链表中，保证交换安全
        if (!Find(node1) || !Find(node2))
            return false;
        if (node1 == node2)
            return true;

        // 注意交换(相邻节点交换和不相邻节点交换)过程中尾节点指针
        Node *p = head_;
        Node *node1_before = nullptr;    // 节点1 前一个节点
        Node *node2_before = nullptr;
        size_t which_before = 0;         // 记录哪个节点在前面
        while (p->next != nullptr) {
            if (p->next == node1) {
                if (!which_before)
                    which_before = size_t(1);
                node1_before = p;
            } else if (p->next == node2) {
                if (!which_before)
                    which_before = size_t(2);
                node2_before = p;
            }
            p = p->next;
        } // end while

        // 开始调换节点顺序且注意尾部指针,默认节点 1 在前
        if (which_before == size_t(2)) { // 节点2在前时需要调整顺序
            Node *temp = node1;
            Node *temp_before = node1_before;
            node1 = node2;
            node2 = temp;
            node1_before = node2_before;
            node2_before = temp_before;
        }
        if (node2_before == node1) {
            if (tail_ == node2)
                tail_ = node1;
            node1_before->next = node2;
            Node *temp = node2->next;
            node2->next = node1;
            node1->next = temp;
        } else {                         // 两个节点至少相隔 1 个节点
            if (tail_ == node2)
                tail_ = node1;
            node1_before->next = node2;
            Node *temp2_next = node2->next;
            node2->next = node1->next;
            node2_before->next = node1;
            node1->next = temp2_next;
        }
        return true;
    }

    //! \brief 将指定节点移到第一个节点处
    //! \complexity: best case: O(1) worst case: O(n) average case: O(n)
    bool MoveHead(Node *node) {
        bool find_flag = false;
        if (node == nullptr)
            return find_flag;
        Node *p = head_;
        while (p->next != nullptr) {
            if (p->next == node) {
                find_flag = true;
                break;
            }
            p = p->next;
        }

        // 此时 p 是 node 的前驱节点
        if (find_flag) {
            if (node != head_->next) {
                if (tail_ == node) {
                    tail_ = p;
                }
                p->next = node->next;
                node->next = head_->next;
                head_->next = node;
            }
        }
        return find_flag;
    }

    //! \brief 单链表反转。
    //! \note 至少保证有两个结点才能正常翻转，否则直接返回
    //! \complexity: O(n)
    //! \method 依次遍历所有节点进行翻转。
    //! TODO
    //!     1）递归实现
    void Reserve() {
        if (node_count_ <= 1) // 0 或 1 个节点直接返回
            return;
        // std::cout << "node_count_" << node_count_<< '\n';
        // 从第二个节点开始依次翻转。最后翻转第一个节点
        Node *prev_node = head_->next;
        Node *curr_node = head_->next->next; // second node
        Node *temp = nullptr;
        while (nullptr != curr_node) {
            temp = curr_node->next;
            curr_node->next = prev_node;
            prev_node = curr_node;
            curr_node = temp;
        }
        // now prev_node 就是指向了尾节点
        // 转换尾节点
        tail_ = head_->next; // 第一个节点变为尾节点
        tail_->next = nullptr;
        head_->next = prev_node;
    }

    //! \brief 求解单链表的中间节点(同理 1/3 节点等等)。
    //! \note 当前实现，可以直接用本类的成员变量 node_count_/2
    //!       如果链表中没有 node_count_ 可以用快慢指针法。两种方法都是 O(n/2) 复杂度
    //! \complexity: O(n)
    //! \method 快慢指针法
    Node* MiddleNode() {
        // 快慢指针法实现获取链表中间节点
        Node *fast_node = head_->next;
        Node *slow_node = head_->next;
        while (nullptr != fast_node && nullptr != fast_node->next) {
            fast_node = fast_node->next->next;
            if (fast_node != nullptr) // 保证偶数个节点时，慢节不需要移动一次
                slow_node = slow_node->next;
        }
        // 到此时，如果节点个数是奇数那么 fast_node == tail_
        return slow_node;
    }

    //! \brief 链表排序(默认：小-->大),如果是类变量，则内部必须重载 ">"
    //! TODO 完成该函数
    void Sort(bool sort_flag = true) {}

    //! \brief 融合另一个有序链表进行排序，保证链表已经是排序状态
    //! TODO 完成该函数
    template <typename Type>
    static void Merge(SingleList<Type> single_list1, SingleList<Type> single_list2) {}

private:
    //! \note: 初始化状态 head_->next == nullptr, node_count_ == 0, tail_ == nullptr
    Node *head_;            // 头指针，指向头结点
    Node *tail_;            // 尾指针，指向最后一个节点
    size_t node_count_ = 0; // 有效节点数量(不包含头节点)
    size_t *reference_count_;

}; // end class SingleList

//! \brief 检测单链表中是否有环
//! \method 快慢指针法，快指针移动两次，满指针移动一次，直到两个指针相遇，则表示有环，否则没有环
//! \TODO 测试该函数
template <typename _T>
bool CheckCircleInSingleList(typename SingleList<_T>::Node* first_node) {
    if (nullptr == first_node or nullptr == first_node->next) {
        return false;
    }
    decltype(first_node) slow_node = first_node, fast_node = first_node;
    while (nullptr != fast_node and nullptr != fast_node->next) {
        slow_node = slow_node->next;
        fast_node = fast_node->next->next;
        if (slow_node == fast_node) {
            return true;
        }
    }
    return false;
}

// 一些利用单链表的应用程序
namespace single_list_app {
/*----------------------------------------------------------------------------------
**   实用用例 LRU 算法
**---------------------------------------------------------------------------------*/
//! \brief 简单实现 LRU 算法
//! \note 暂无
//! \complexity: O(n)
//! \param single_list:将要维护的单链表结构
//! \param data: LRU 缓存中将要保留的数据
//! \param max_node_count: 缓存中允许的最大节点数量
//! \return void
//! \method: 当前实现利用了单链表结构。
//! \description google 搜索 LRU 缓存淘汰策略
//! \reference 无
//! \TODO 暂无
//! \example:
//!     glib::SingleList<int> single_list;
//!     glib::LRUBySingleList(single_list, int(data), 12);
template <typename T>
void LRUBySingleList(SingleList<T> &single_list, const T &data,
                     size_t max_node_count = 10) {
    const auto vec_nodes = single_list.Find(data);

    // 当前给定的 data 在链表中存在，需要把对应节点移到第一个节点的位置,然后后续节点依次向后移动
    // 不存在该数据，需要在没达到最大节点数量的前提下插入节点。
    // 已经达到最大节点，需要删除尾部节点(最早进来的数据)
    if (vec_nodes.size()) {
        single_list.MoveHead(vec_nodes[0]);               // O(n)
    } else if (single_list.node_count() >= max_node_count) {
        // 这个实现效率有点低，应该利用尾部将要删除的节点，然后数据替换成新来的 data
        // single_list.DeleteTail();
        // single_list.InsertHead(data);
        if (single_list.MoveHead(single_list.tail_ptr())) // 把尾部节点移到第一个节点，然后替换数据
            single_list.first_node_ptr()->data = data;
    } else {
        single_list.InsertHead(data);
    }
}

/*----------------------------------------------------------------------------------
**   实用用例 字符串是否是回文，假定用单链表存储了字符串，比如 aba、abba 是回文，正序和倒续是一致的
** method: 1、直接用递归方法 O(n) 2、两个字符数组存储前一半正序，后一半倒序存储，之后一一比较 O(n)
**---------------------------------------------------------------------------------*/

//! \note: 保证输入的链表不为空。且处理完毕后原链表会改变
//! \complexity: best case:O(1) worst case: O(n^2) average case: O(n)
//! \method: 递归方法：依次比较第一个和最后一个节点
bool IsPalindromeString(SingleList<char> &string) {
    if (string.node_count() <=1)
        return true;
    if (string.first_node_ptr()->data != string.tail_ptr()->data)
        return false;
    string.DeleteHead(); // O(1)
    string.DeleteTail(); // O(n)
    return IsPalindromeString(string);
}

//! \note: 保证链表不为空。函数内部实现没有使用尾指针以及已知的节点。默认假设链表就是最简单的单链表
//! \complexity: o(n)
//! \method: 快慢指针法找到中间节点 ：两个字符数组存储前一半正序，后一半倒序存储，之后一一比较
//! \return: true: 是回文串 false: 不是回文串
bool PalingromeString(SingleList<char> &string) {
    if (string.empty())
        return true;

    // 快慢指针法实现获取链表中间节点
    size_t node_count = 1; // 一半节点数量
    SingleList<char>::Node *fast_node = string.first_node_ptr();
    SingleList<char>::Node *slow_node = fast_node;
    while (nullptr != fast_node && nullptr != fast_node->next) {
        fast_node = fast_node->next->next;
        if (fast_node != nullptr) {        // 保证偶数个节点时，慢节不需要移动一次
            slow_node = slow_node->next;
            node_count++;
        }
    } // slow_node 即为中间节点

    // 到此时，如果节点个数是奇数那么 fast_node == tail_，偶数： fast_node = nullptr
    SingleList<char>::Node **prev_half_node = nullptr; // 存储前一半节点
    SingleList<char>::Node **last_half_node = nullptr; // 存储后一半节点
    size_t array_num = 0;                   // 表示前一半节点和后一半节点需要申请空间的数量
    if (nullptr == fast_node) {             // 偶数个节点
        prev_half_node = new SingleList<char>::Node*[node_count];
        last_half_node = new SingleList<char>::Node*[node_count];
        array_num = node_count;
        size_t temp_num = 0;                           // 临时计数器，存储节点指针用
        // node_count *= 2;                            // 总节点数量
        SingleList<char>::Node *p = string.first_node_ptr();
        while (p != slow_node->next) { // 包含慢节点
            prev_half_node[temp_num] = p;
            temp_num++;
            p = p->next;
        }
        temp_num = array_num - 1;
        while (p != nullptr) {              // 倒序存储
            *(last_half_node + temp_num) = p;
            temp_num--;
            p = p->next;
        }
    } else {
        node_count = 2 * node_count - 1;    // 总节点个数
        if (1 == node_count)                // 1 个节点肯定是回文
            return true;

        // 至少 3 个节点
        array_num = (node_count - 1)/2;
        prev_half_node = new SingleList<char>::Node *[array_num];
        last_half_node = new SingleList<char>::Node *[array_num];
        SingleList<char>::Node *p = string.first_node_ptr();
        size_t temp_num = 0;
        while (p != slow_node) {
            prev_half_node[temp_num] = p;
            temp_num++;
            p = p->next;
        }
        temp_num = array_num - 1;
        p = p->next;
        while (p != nullptr) {
            last_half_node[temp_num] = p;
            temp_num--;
            p = p->next;
        }
    }

    // 比较两个数组
    for (size_t i = 0; i < array_num; i++) {
        if (prev_half_node[i]->data != last_half_node[i]->data)
            return false;
    }
    delete prev_half_node;
    delete last_half_node;
    return true;

}

} // namespace single_list_app
} // namespace glib

#endif // GLIB_SINGLE_LIST_HPP_
