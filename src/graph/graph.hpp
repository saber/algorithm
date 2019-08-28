/*
 * CopyRight (c) 2019 gcj
 * File: graph.hpp
 * Project: algorithm
 * Author: gcj
 * Date: 2019/8/24
 * Description: graph and BFS DFS algorithm simple implementation
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#ifndef GLIB_GRAPH_H_
#define GLIB_GRAPH_H_

#include "../internal/macros.h"
#include "assert.h"
#include <iostream>
#include <vector>
#include <queue> // 广度优先用
#include <stack> // 深度优先用
#include <map>   // 图底层临界表用

//! \brief 简单实现图以及图的深度优先搜索和广度优先搜索
//!     外部调用核心函数：
//!         1）向图中添加边：AddEdge()
//!         2）广度优先搜索：Bfs()
//!         3）深度优先搜索（递归）：Dfs()
//!         4）深度优先搜索（非递归）：DfsCycle()
//!     外部调用状态函数：
//!     内部辅助核心函数：
//!         1）保存搜索路径：ReservePath()
//!         2）深度优先搜索递归：RecursiveDfs()
//! \Note
//!     1）目前仅仅支持无向图，邻接表存储图结构，且内部顶点抽象成了编号 1~n
//!
//! \TODO
//!     1）支持有向图、带权重的有向图、带权重的无向图
//!     2）调整使得邻接矩阵也适用
//!     3）key 自动利用 hash 抽象到内部的顶点 0~n，进而支持任意的顶点表达
//!
//! \platform
//!     ubuntu16.04 g++ version 5.4.0

namespace glib {
namespace internal {
    enum GraphOption {
        // DIRECTED_GRAPH,           // 有向图（不带权重）
        UNDIRECTED_GRAPH,         // 无向图
        // WEIGHTED_DIRECTED_GRAPH,  // 带权重的有向图
        // WEIGHTED_UNDIRECTED_GRAPH // 带权重的无向图
    }; // enum GraphOption

} // namespace internal

using namespace internal;

template <typename _Scalar, GraphOption _Option = UNDIRECTED_GRAPH>
class Graph {
public:  // type or struct define and declaration
    using ValueType = _Scalar;
    using Key       = _Scalar;
    using Value     = _Scalar;
    using KeyMap    = std::map<Key, Value>;
    using KeyMapValueType = typename KeyMap::value_type;

public:  // construct function
    explicit
    Graph(size_t vertex_count) : vertex_count_(vertex_count) {
        adjacency_table_ = new KeyMap[vertex_count_];
    }
    GLIB_DISALLOW_IMPLICIT_CONSTRUCTORS_PUBLIC(Graph);

public:  // external call function
    // 添加图中顶点 i 和 j 之间的边（只能从 0 开始编号！）
    void AddEdge(int i, int j);

    // 广度优先搜索，是一种最短路径（当然最短路径不只有一条）
    // \note 适用于图的边和边之间无权重，同样适合网格搜索！
    std::vector<_Scalar> Bfs(int source, int target);

    // 深度优先算法（递归版）
    std::vector<_Scalar> Dfs(int source, int target);

    // 深度优先算法（非递归版）
    std::vector<_Scalar> DfsCycle(int source, int target);

private: // internal helper function
    // 保存路径信息
    void PrintPaths(int* path, int source, int target) {
        if (source != target) {
            PrintPaths(path, source, path[target]);
        }
        std::cout << target << " "; // 先打印其他的路径，最后在打印 target
    }
    void ReservePath(int* path, int source, int target) {
        if (source != target) {
            ReservePath(path, source, path[target]);
        }
        bfs_dfs_paths_.push_back(target);
    }

    // 深度优先算法递归函数
    void RecursiveDfs(int source, int target,
                      std::vector<bool>& has_visited,
                      std::vector<int>& reserve_paths);
private: // internal member variable
    size_t  vertex_count_    = 0;
    KeyMap* adjacency_table_ = nullptr; // 适合有权重的！可以有序如何实现？
    bool    dfs_found_       = false;   // 广度优先算法使用的变量
    std::vector<int> bfs_dfs_paths_;    // 保存 dfs 或者 bfs 实现的路径结果！
                                        // 每次获取要从新调用 Dfs 和 Bfs 的代码
}; // class Graph

//----------------------- external call function------------------------------//
// 添加图中顶点链接的边
// \complexity O(1)
template <typename _Scalar, GraphOption _Option>
void Graph<_Scalar, _Option>::AddEdge(int i, int j) {
    assert(i >= 0 && j < vertex_count_ && "index over graph range!");
    adjacency_table_[i].insert(KeyMapValueType(ValueType(j), ValueType(j)));
    adjacency_table_[j].insert(KeyMapValueType(ValueType(i), ValueType(i)));
}

// 广度优先搜索，返回搜索的路径信息
//！ \complexity 时间复杂度为 O(边)，空间复杂度 O(顶点)
template <typename _Scalar, GraphOption _Option>
std::vector<_Scalar>
Graph<_Scalar, _Option>::Bfs(int source, int target) {
    assert(source >= 0 && target < vertex_count_ && "index over graph range!");
    bfs_dfs_paths_.clear(); // 保存路径信息的，在 PrintPaths 生成的，这里进行清空！
    if (source == target) return std::vector<_Scalar>(source);
    bool has_visited[vertex_count_] = {0};
    int reserve_paths[vertex_count_]; // 保留过往的路径信息，方便后期恢复路径
    for (int i = 0; i < vertex_count_; ++i) reserve_paths[i] = -1;
    std::queue<int> access_vertex; // 保存将要访问的顶点

    // 初始化
    access_vertex.push(source);
    has_visited[source] = true;

    // 正式 BFS 全局范围搜索
    while (!access_vertex.empty()) {
        int vertex_index = access_vertex.front();
        access_vertex.pop();
        for (const auto& adjacency: adjacency_table_[vertex_index]) {
            int matched_vertex_index = adjacency.first;
            if (!has_visited[matched_vertex_index]) {
                reserve_paths[matched_vertex_index] = vertex_index;
                if (matched_vertex_index == target) {
                    PrintPaths(reserve_paths, source, target);
                    ReservePath(reserve_paths, source, target);
                    return bfs_dfs_paths_;
                }
                access_vertex.push(matched_vertex_index);
                has_visited[matched_vertex_index] = true;
            }
        }
    }
}

// 深度优先算法，找到的不是最优路径
//！ \complexity 时间复杂度为 O(边)，空间复杂度 O(顶点)
template <typename _Scalar, GraphOption _Option>
std::vector<_Scalar>
Graph<_Scalar, _Option>::Dfs(int source, int target) {
    assert(source >= 0 && target < vertex_count_ && "index over graph range!");
    bfs_dfs_paths_.clear(); // 保存路径信息的，在 PrintPaths 生成的，这里进行清空！
    if (source == target) return {source};
    std::vector<bool> has_visited(vertex_count_, 0);
    std::vector<int> reserve_paths(vertex_count_, -1);

    dfs_found_ = false;
    RecursiveDfs(source, target, has_visited, reserve_paths);
    // 需要保存路径信息
    int temp_paths[vertex_count_];
    for (int i = 0; i < vertex_count_; ++i)
        temp_paths[i] = reserve_paths[i];
    if (dfs_found_) {
        PrintPaths(temp_paths, source, target);
        ReservePath(temp_paths, source, target);
        return bfs_dfs_paths_;
    }
}


// 深度优先算法（非递归版）
//！ \complexity 时间复杂度为 O(边)，空间复杂度 O(顶点)
template <typename _Scalar, GraphOption _Option>
std::vector<_Scalar> Graph<_Scalar, _Option>::DfsCycle(int source, int target) {
    assert(source >= 0 && target < vertex_count_ && "index over graph range!");
    bfs_dfs_paths_.clear(); // 每次使用不同的方法，这里都要清除！
    if (source == target) return {source};

    // 初始化 1
    // 一些记录变量，比如访问过的顶点，保留的路径信息
    int has_visited[vertex_count_] = {0};
    int reserve_paths[vertex_count_];
    for (int i = 0; i < vertex_count_; ++i) reserve_paths[i] = -1;
    std::stack<std::pair<int, int>> vertex; // 第一个数字代表顶点序号，
                                            // 第二个代表该序号对应的上次的 source

    // 初始化 2
    // 将当前节点的邻接节点保存起来，按照递归（回溯思想）的思路，
    // 显示用栈来模仿函数栈
    vertex.push(std::make_pair(adjacency_table_[source].begin()->first, source));
    has_visited[source] = true;
    for (const auto& adjacency: adjacency_table_[source]) {
        int matched_index = adjacency.first;
        vertex.push(std::make_pair(matched_index, source));
    }

    while (!vertex.empty()) {
        // 先从栈顶弹出一个顶点，然后判断当前顶点是不是符合要求，
        // 符合要求，我们就直接返回，不符合要求，我们要把它的邻
        // 接节点压入栈中，方便回溯时来访问其他邻接节点。
        // 这里就是模仿上面的递归函数栈的方法
        auto index = vertex.top();
        vertex.pop();
        if (has_visited[index.first])
            continue;
        reserve_paths[index.first] = index.second;
        if (index.first == target) {
            // PrintPaths(reserve_paths, source, target);
            ReservePath(reserve_paths, source, target);
            return bfs_dfs_paths_;
        }

        has_visited[index.first] = true;

        // 将当前节点的邻接节点全部压入栈
        for (const auto& adjacency: adjacency_table_[index.first]) {
            int matched_index = adjacency.first;
            vertex.push(std::make_pair(matched_index, index.first));
        }
    }
}

//-------------------------internal helper function---------------------------//
// 深度优先算法递归函数
template <typename _Scalar, GraphOption _Option>
void Graph<_Scalar, _Option>::RecursiveDfs(int source, int target,
                                           std::vector<bool>& has_visited,
                                           std::vector<int>& reserve_paths) {
    has_visited[source] = true;
    if (dfs_found_ == true) return;
    if (source == target) {dfs_found_ = true; return;}

    // 遍历以当前 source 为起点的所有链接的路径
    // 已经假定前面的路径已经确定
    for (const auto& adjacency: adjacency_table_[source]) {
        int matched_index = adjacency.first;
        if (!has_visited[matched_index]) {
            reserve_paths[matched_index] = source;
            RecursiveDfs(matched_index, target, has_visited, reserve_paths);
        }
    }
}

} // namespace glib

#endif // GLIB_HEAP_H_
