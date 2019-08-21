/*
 * CopyRight (c) 2019 gcj
 * File: consistent_hash.hpp
 * Project: algorithm
 * Author: gcj
 * Date: 2019/8/19
 * Description: binary search tree simple implementation
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#ifndef GLIB_BINARY_SEARCH_TREE_H_
#define GLIB_BINARY_SEARCH_TREE_H_

#include "../internal/macros.h" // 一些类中常用宏！
#include <initializer_list>     // 初始化列表使用
#include <iostream>
#include <queue>

//! \brief 实现一个二叉搜索树
//!     外部调用核心函数：
//!         1）查询函数：Find(key)
//!         2）插入数据：Insert(key)
//!         3）删除数据：Delete(key)
//!     外部调用状态函数：
//!         1）前序遍历（递归）：print_by_front_order()
//!         2）中序遍历（递归）：print_by_middle_order()
//!         3）后序遍历（递归）：print_by_back_order()
//!         4）层序遍历（循环）：print_by_level_order()
//!         5）二叉树高度（递归 and 循环）：TreeHeight()
//!         6）树是否为空：Empty()
//!     内部辅助函数：
//!         1）清除树：ClearTree()
//!         2）前序遍历（给定节点）：FrontOrder(node)
//!         3）中序遍历（给定节点）：MiddleOrder(node)
//!         4）后序遍历（给定节点）：BackOrder(node)
//!         5）获得节点高度（递归）：NodeHeight(node)
//!
//! \Note
//!     1）本类仅仅支持数据类型为：常用的内置数据类型、string 类型。
//!     2）不支持重复数据！
//!
//! \TODO
//!     1）支持自定义类类型
//!     2）支持重复数据（Key Value）形式
//!
//! \platform
//!     ubuntu16.04 g++ version 5.4.0

namespace glib {

namespace internal {
enum TreeHeightOption {
    RECURSIVE,
    NO_RECURSIVE
};
} // namespace internal

//! \brief 实现了一个二叉搜索树，不支持重复数据的二叉查找树
template <typename _Key>
class BinarySearchTree {
public:  // type or struct declaration
    using ValueType = _Key;
    struct TreeNode {
        TreeNode *left_child;
        ValueType data; // 如果自己使用类，或者带有键值（Key）和卫星数据，
                        // 那么需要自己在内部实现 == 重载操作符
        TreeNode *right_child;
        TreeNode(ValueType key)
            : data(key), left_child(nullptr), right_child(nullptr) {}
    };

public:  // construct function
    BinarySearchTree() = default;
    BinarySearchTree(std::initializer_list<ValueType> il) {
        for (const auto& it: il) {
            Insert(it);
        }
    }
    // 释放树节点
    ~BinarySearchTree() {
        if (tree_root_ != nullptr) {
            // 递归的释放所有节
            ClearTree(tree_root_);
            tree_root_ = nullptr; // 这里要记得清零操作！
        }
    }
    GLIB_DISALLOW_COPY_AND_ASSIGN_PUBLIC(BinarySearchTree);

public:  // external call function
    // 按照值查询
    TreeNode* Find(const ValueType& key) const;

    // 插入数据（不支持重复数据的插入 Key Value）
    void Insert(const ValueType& key);

    // 按值删除
    void Delete(const ValueType& key);

    // 前序遍历（递归）
    void print_by_front_order()  const;

    // 中序遍历（递归）
    void print_by_middle_order() const;

    // 后序遍历（递归）
    void print_by_back_order()   const;

    // 按层遍历
    void print_by_level_order()  const;

    // 二叉树高度，-1 表示没有树
    int TreeHeight(const internal::TreeHeightOption& option =
                   internal::TreeHeightOption::NO_RECURSIVE) const;

    // 树是否为空
    bool Empty() const { return (tree_root_ == nullptr);}

private: // internal helper function
    // 清除当前树中的所有节点
    void ClearTree(TreeNode* node);

    // 给定节点——前序遍历——递归
    void FrontOrder(const TreeNode* node)  const;

    // 给定节点——中序遍历——递归
    void MiddleOrder(const TreeNode* node) const;

    // 给定节点——后序遍历——递归
    void BackOrder(const TreeNode* node)   const;

    // 获得节点高度
    int NodeHeight(const TreeNode* node)   const;
private: // internal member variable
    TreeNode* tree_root_ = nullptr; // 树根节点

}; // class BinarySearchTree

//------------------------------- external call function---------------------------//
// template <typename _Key>
// using typename BinarySearchTree<_Key>::TreeNode;

// 按照值查询
//! \complexity O(n) ~ O(logn) 之间，分别对应退化成链表形式以及完全二叉树形式
template <typename _Key>
typename BinarySearchTree<_Key>::TreeNode*
BinarySearchTree<_Key>::Find(const ValueType& key) const {
    TreeNode* temp_node = tree_root_;
    while (temp_node != nullptr) {
        if (key > temp_node->data)
            temp_node = temp_node->right_child;
        else if (key < temp_node->data)
            temp_node = temp_node->left_child;
        else
            break;
    }
    return temp_node;
}

// 插入数据（不支持重复数据的插入 Key Value）
template <typename _Key>
void BinarySearchTree<_Key>::Insert(const ValueType& key) {
    // 根节点为空直接插入
    if (tree_root_ == nullptr) {
        tree_root_ = new TreeNode(key);
        return;
    }

    TreeNode* temp_node = tree_root_;
    TreeNode* temp_node_parent = nullptr;
    while (temp_node != nullptr) {
        temp_node_parent = temp_node;
        if (key > temp_node->data)
            temp_node = temp_node->right_child;
        else if (key < temp_node->data)
            temp_node = temp_node->left_child;
        else
            return;
    }
    // 到这里说明树中没有该值的节点，并且找到了合适的插入位置
    // 其实这里 temp_node 应该就是空的，temp_node_parent
    // 指向了当前要插入位置的父亲
    // 没有把下面放在上面 while，是为了减少判断的次数，在这
    // 里只需要一次判断，节省了时间！
    TreeNode* new_tree_node = new TreeNode(key);
    if (key > temp_node_parent->data)
        temp_node_parent->right_child = new_tree_node;
    else
        temp_node_parent->left_child  = new_tree_node;
}

// 按值删除
template <typename _Key>
void BinarySearchTree<_Key>::Delete(const ValueType& key) {
    if (tree_root_ == nullptr) return;
    TreeNode* temp_node = tree_root_;
    TreeNode* temp_node_parent = nullptr;
    while (temp_node != nullptr && temp_node->data != key) {
        temp_node_parent = temp_node;
        if (key > temp_node->data)
            temp_node = temp_node->right_child;
        else
            temp_node = temp_node->left_child;
    }
    // 退出上面循环有两种情况
    // 1) 找到了等于给定值的节点
    // 2) 没有找到给定值节点
    if (temp_node == nullptr) return; // 没找到给定值节点，（其实同样包含了树根为空）

    // 下面分三种情况
    // 1 给定值有两个子节点：需要遍历其右子树，找到最小值，替换最小值节点和当前节点，
    //  也要注意最小值节点有没有子节点
    // 2 给定值有一个子节点：那么直接将父节点指向该节点的儿子节点，并删除当前节点
    // 3 给定值没有子节点：直接删除当前节点，并清空父亲的对应指针
    if (temp_node->left_child != nullptr &&
        temp_node->right_child != nullptr) {
        TreeNode* right_tree_min_node = temp_node->right_child;
        TreeNode* right_tree_min_node_parent = temp_node;
        while (right_tree_min_node->left_child != nullptr) {
            right_tree_min_node_parent = right_tree_min_node;
            right_tree_min_node = right_tree_min_node->left_child;
        }
        // 这里直接交换数据，没有交换节点指针！此时最小节点要么有 1 个右子节点，要么没有子节点
        temp_node->data = right_tree_min_node->data;
        temp_node_parent = right_tree_min_node_parent;
        temp_node = right_tree_min_node;
    }

    // 接下来处理只有一个子节点和没有子节点情况（包含了删除的节点是根节点情况）
    // 先找到该节点的孩子节点
    TreeNode* temp_child = nullptr;
    if (temp_node->left_child != nullptr)
        temp_child = temp_node->left_child;
    if (temp_node->right_child != nullptr)
        temp_child = temp_node->right_child;

    // 1）删除的是根部节点
    if (temp_node_parent == nullptr /*or temp_node == tree_root_*/) { // 找到的节点是根节点
        if (temp_child == nullptr) {
            temp_node->left_child = nullptr;
            temp_node->right_child = nullptr;
        } else if (temp_child == temp_node->left_child)
            temp_node->left_child = nullptr;
        else
            temp_node->right_child = nullptr;
        delete temp_node;
        tree_root_ = temp_child;
        return;
    }
    // 2）删除的节点是其他非根节点
    if (temp_node_parent->left_child == temp_node) {
        temp_node_parent->left_child = temp_child;
        temp_node->left_child  = nullptr;
        temp_node->right_child = nullptr;
        delete temp_node;
        return;
    }
    if (temp_node_parent->right_child == temp_node) {
        temp_node_parent->right_child = temp_child;
        temp_node->left_child  = nullptr;
        temp_node->right_child = nullptr;
        delete temp_node;
        return;
    }
}

// 前序遍历
template <typename _Key>
void BinarySearchTree<_Key>::print_by_front_order() const {
    if (tree_root_ == nullptr) return;
    const TreeNode* temp_node = tree_root_;
    FrontOrder(temp_node);
}

// 中序遍历
template <typename _Key>
void BinarySearchTree<_Key>::print_by_middle_order() const {
    if (tree_root_ == nullptr) return;
    const TreeNode* temp_node = tree_root_;
    MiddleOrder(temp_node);
}

// 后序遍历
template <typename _Key>
void BinarySearchTree<_Key>::print_by_back_order()   const {
    if (tree_root_ == nullptr) return;
    const TreeNode* temp_node = tree_root_;
    BackOrder(temp_node);
}

// 按层遍历
template <typename _Key>
void BinarySearchTree<_Key>::print_by_level_order()  const {
    if (tree_root_ == nullptr) return;
    std::queue<TreeNode*> node_queue;
    node_queue.push(tree_root_);
    while (!node_queue.empty()) {
        TreeNode* temp_node = node_queue.front();
        node_queue.pop();
        if (temp_node->left_child != nullptr)
            node_queue.push(temp_node->left_child);
        if (temp_node->right_child != nullptr)
            node_queue.push(temp_node->right_child);
        std::cout << temp_node->data << " ";
    }
}

// 二叉树高度
//! \return -1: 没有树 其他值：树的高度
template <typename _Key>
int BinarySearchTree<_Key>::TreeHeight(const internal::TreeHeightOption& option) const {
    if (option == internal::NO_RECURSIVE) {
        if (tree_root_ == nullptr) return -1; // 表示没有树
        std::queue<TreeNode*>node_queue;
        node_queue.push(tree_root_);
        int tree_height = 0;
        int curr_level_node_count = 1; // 记录当前层节点数量
        int next_level_node_count = 0; // 记录下一层节点数量
        while (!node_queue.empty()) {
            TreeNode* temp_node = node_queue.front();
            node_queue.pop();
            curr_level_node_count--;
            if (temp_node->left_child != nullptr) {
                node_queue.push(temp_node->left_child);
                next_level_node_count++;
            }
            if (temp_node->right_child != nullptr) {
                node_queue.push(temp_node->right_child);
                next_level_node_count++;
            }
            if (curr_level_node_count == 0) {
                if (next_level_node_count != 0)
                    tree_height++;
                curr_level_node_count = next_level_node_count;
                next_level_node_count = 0; // 之前忘记清空了！
            }
        }
        return tree_height;
    }
    if (option == internal::RECURSIVE) {
        if (tree_root_ == nullptr) return -1; // 表示没有树
        return NodeHeight(tree_root_);
    }
}


//------------------------------internal helper function---------------------------//
// 清除当前树中的所有节点
template <typename _Key>
void BinarySearchTree<_Key>::ClearTree(TreeNode* node) {
    if (node->left_child == nullptr && node->right_child == nullptr) {
        delete node;
        return;
    }
    if (node->left_child != nullptr)
        ClearTree(node->left_child);
    if (node->right_child != nullptr)
        ClearTree(node->right_child);
    node->left_child  = nullptr;
    node->right_child = nullptr;
    delete node;
}

// 前序遍历——递归
template <typename _Key>
void BinarySearchTree<_Key>::FrontOrder(const TreeNode* node)  const {
    if (node == nullptr) return;
    std::cout << node->data << " ";
    FrontOrder(node->left_child);
    FrontOrder(node->right_child);
    return;
}

// 中序遍历——递归
template <typename _Key>
void BinarySearchTree<_Key>::MiddleOrder(const TreeNode* node) const {
    if (node == nullptr) return;
    MiddleOrder(node->left_child);
    std::cout << node->data << " ";
    MiddleOrder(node->right_child);
    return;
}

// 后序遍历——递归
template <typename _Key>
void BinarySearchTree<_Key>::BackOrder(const TreeNode* node)   const {
    if (node == nullptr) return;
    BackOrder(node->left_child);
    BackOrder(node->right_child);
    std::cout << node->data << " ";
    return;
}

// 树的高度——递归
template <typename _Key>
int BinarySearchTree<_Key>::NodeHeight(const TreeNode* node) const {
    if (node == nullptr) return -1;
    return std::max(NodeHeight(node->left_child), NodeHeight(node->right_child)) + 1;
}

} // namespace glib

#endif // GLIB_BINARY_SEARCH_TREE_H_
