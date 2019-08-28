/*
 * CopyRight (c) 2019 gcj
 * File: graph.test.cc
 * Project: algorithm
 * Author: gcj
 * Date: 2019/8/24
 * Description: test graph and BFS DFS algorithm
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

 #include "graph.hpp"
 #include <iostream>
 #include <vector>

 //! \brief 简单测试堆及应用
 //! \run
 //!     g++ graph.test.cc -std=c++11 && ./a.out

using namespace std;
int main(int argc, char const *argv[]) {
    glib::Graph<int> graph(8);
    graph.AddEdge(0, 1);
    graph.AddEdge(0, 3);

    graph.AddEdge(1, 0);
    graph.AddEdge(1, 2);
    graph.AddEdge(1, 4);

    graph.AddEdge(2, 1);
    graph.AddEdge(2, 5);

    graph.AddEdge(3, 0);
    graph.AddEdge(3, 4);

    graph.AddEdge(4, 1);
    graph.AddEdge(4, 3);
    graph.AddEdge(4, 5);
    graph.AddEdge(4, 6);

    graph.AddEdge(5, 2);
    graph.AddEdge(5, 4);
    graph.AddEdge(5, 7);

    graph.AddEdge(6, 4);
    graph.AddEdge(6, 7);

    graph.AddEdge(7, 5);
    graph.AddEdge(7, 6);
    cout << "添加边完毕！" << endl;

    cout << "广度优先搜索" << endl;
    vector<int> path_bfs = graph.Bfs(0, 6);
    cout << "     ";
    for (size_t i = 0; i < path_bfs.size(); ++i)
        cout << path_bfs[i] << " ";
    cout << endl;

    cout << "深度优先搜索（递归）" << endl;
    vector<int> path_dfs = graph.Dfs(0, 6);
    cout << "     ";
    for (size_t i = 0; i < path_dfs.size(); ++i)
        cout << path_dfs[i] << " ";
    cout << endl;

    cout << "深度优先搜索（非递归）" << endl;
    vector<int> path_dfs_circle = graph.DfsCycle(0, 6);
    for (size_t i = 0; i < path_dfs_circle.size(); ++i)
        cout << path_dfs_circle[i] << " ";
    cout << endl;
}
