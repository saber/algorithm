/*
 * CopyRight (c) 2019 gcj
 * File: backtracking.hpp
 * Project: algorithm
 * Author: gcj
 * Date: 2019/9/17
 * Description: backtracking algorithm simple implementation
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */
#ifndef GLIB_BACKTRACKING_H_
#define GLIB_BACKTRACKING_H_

#include <unordered_map>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm> // for_each
#include <utility>   // std::pair
#include <cassert>  // assert
#include <deque>

//! \brief 利用回溯算法解决 0-1 背包（最大价值、最大重量）、8 皇后问题和
//!        leetcode 上 980 题：不同路径III（链接：https://leetcode-cn.com/problems/unique-paths-iii/）
//!     外部调用核心函数：
//!         1）N 皇后问题：EightQueens()
//!         2）0-1 背包最大重量问题：ZeroOrOneBackpack()
//!         3）0-1 背包最大价值量问题：ZeroOrOneBackpackValue()
//!         4）不同路径 III：GridPath()
//!
//! \Note
//!     1）0-1 背包问题，输入的测试数据不要太大，50 个数据跑的就非常慢了
//!     2）在不同路径问题上，经过 leetcode 的测试，虽然结果全部正确，
//!       但是运行的速度和内存消耗都比较高
//!
//! \platform
//!      ubuntu16.04 g++ version 5.4.0

namespace glib {
namespace internal {
// 网格路径 helper
template <typename T>
struct Point {
    T x;
    T y;
    Point() : x(T(0)), y(T(0)) {}
    Point(T _x, T _y) : x(_x), y(_y) {}
};// struct Point

// 背包要装的物品清单
template <typename T1, typename T2>
struct ListOfItems {
    const T1 weight;
    const T2 value;
    ListOfItems(T1 _weight, T2 _value) : weight(_weight), value(_value) {}
}; // struct Backpack

// 回溯算法需要的参数
struct BacktrackParameterBase {
    // 回溯问题解空间相关参数
    const
    int solve_space_dim; // 解空间维度
    const
    int element_dim;     // 每一维解空间对应的维度
    int result_num = 0;  // 表示满足条件的所有可能情况数量
    BacktrackParameterBase(int _solve_space_dim, int _element_dim,
        int _result_num = 0)
        : solve_space_dim(_solve_space_dim),
          element_dim(_element_dim),
          result_num(_result_num) {}
}; // struct BacktrackParameterBase

// 8 皇后回溯参数
struct EightQueensParameter: public BacktrackParameterBase {
    typedef std::vector<std::vector<int>> ResultType;
    std::vector<ResultType> result_record; // 记录皇后放置的网格，网格元素为 2 表示皇后的位置
    EightQueensParameter(int _solve_space_dim, int _element_dim, int _result_num = 0)
        : BacktrackParameterBase(_solve_space_dim, _element_dim, _result_num) {}

}; // struct EightQueensParameter

// 0-1 背包回溯参数
template <typename T>
struct ZeroOrOneBackpackParameter: public BacktrackParameterBase {
    // 加速 0-1 背包回溯算法（）
    typedef std::unordered_multimap<int, std::pair<T, T>> StateType;
    StateType state; // 保存回溯算法已经走过的状态（index 当前背包重量、当前背包价值）

    T weight_up_limit;    // 背包重量上限
    T max_weight = -T(1); // 最大背包重量，0-1 背包的简单变种
    T max_value  = -T(1); // 最大背包价值量，0-1 背包常规形式
    std::vector<bool> result_record; // 记录 0-1 背包问题，哪个物品
                                     // 最后装载到背包中，true 表示
                                     // 对应物品需要放在背包中

    ZeroOrOneBackpackParameter(T _weight_up_limit, T _max_weight, T _max_value,
        int _solve_space_dim, int _element_dim, int stuff_num, int _result_num = 0)
        : BacktrackParameterBase(_solve_space_dim, _element_dim, _result_num),
          weight_up_limit(_weight_up_limit), max_weight(_max_weight),
          max_value(_max_value), result_record(stuff_num, false) {}
}; // struct ZeroOrOneBackpackParameter

// 网格路径 helper
struct GridPathParameter: public BacktrackParameterBase {
    // 起始点和终止点
    const Point<int> start;
    const Point<int> end;

    // 网格大小
    const int rows;
    const int cols;

    // 网格状态标识，记住这里的标识不能有相等的情况，否则会出错！
    const int idle_flag;
    const int passed_flag;

    // 不同网格路径数量
    int path_num = 0;

    GridPathParameter(const Point<int> _start, const Point<int> _end, int _rows,
                      int _cols, int _idle_flag, int _passed_flag, int _end_flag,
                      int _solve_space_dim, int _element_dim,
                      int _path_num = 0, int _result_num = 0)
                      : BacktrackParameterBase(_solve_space_dim, _element_dim, _result_num),
                       start(_start), end(_end), rows(_rows), cols(_cols), idle_flag(_idle_flag),
                       passed_flag(_passed_flag), path_num(_path_num) {
        if (idle_flag == passed_flag) {
            std::cerr << "网格状态标识不能重复！" << '\n';
        }
    }
}; // struct GridParameter

// solve space number 求解一个问题的解空间个数
// grid path helper
//! \brief 求解二维网格问题解空间维度，网格内容默认 0 是可走路径
//! \param start_flag 表示网格中起点标志
//! \param end_flag 终点点标志
//! \param start 起点
//! \param end 终点
//! \note 这里把终点也作为解空间的一维！
int SolveSpaceDimension(const std::vector<std::vector<int>>& grids,
                        Point<int>& start, Point<int>& end,
                        int start_flag, int end_flag) {
    int solve_space_dim = 0;
    for (size_t i = 0; i < grids.size(); ++i) {
        for (size_t j = 0; j < grids[i].size(); ++j) {
            if (grids[i][j] == 0)         // 记录解空间维度
                solve_space_dim++;
            if (grids[i][j] == start_flag)
                start = Point<int>(j, i); // 注意这里存储的顺序！x 存储列数，y存储行数
            if (grids[i][j] == end_flag) {
                end = Point<int>(j, i);
                solve_space_dim++;        // 这里默认也把终点当做了解空间的一个维度
            }
        }
    }
    return solve_space_dim;
}

// 8 皇后 and o-1 背包
int SolveSpaceDimension(int dimension) {
    return dimension;
}

// eight queens helper
// 判断当前网格位置能否放置皇后
bool IsOkEightQueens(const std::vector<std::vector<int>> grids,
                     int dimension_index, int element_index) {
    // 判断当前放置的皇后是不是满足条件
    if (grids[dimension_index][element_index])
        return false;
    else
        return true;
}

// 按照 8 皇后阻塞条件来填充网格
std::vector<std::vector<int>> FillGrid(std::vector<std::vector<int>> grids,int space_dimension,
            int dimension_index, int element_dimension, int element_index) {
    // 标记网格中不能放置的区域，从当前维度开始到网格结束
    // 下面都是向下进行填充，没有把横线和竖线以及对角线上面填充，是因为，下面的皇后不可能放置
    // 在上面，所以其他的没有填充，节省了计算
    for (int i = dimension_index; i < space_dimension; ++i) // 当前列标记为 1
        grids[i][element_index] = 1;

    // 标记对角线（右下）
    for (int i = dimension_index, j = element_index;        // 当时这里笔误写成了 element_dimension 了
         i < space_dimension && j < element_dimension; ++i, ++j)
        grids[i][j] = 1;

    // 标记对角线（左下）
    for (int i = dimension_index, j = element_index;
         i < space_dimension && j >= 0; ++i, --j)
        grids[i][j] = 1;

    // 标记皇后放置的位置！
    grids[dimension_index][element_index] = 2;              // 表示每行放置的皇后
    return grids;
}

void EightQueensBacktrack(std::vector<std::vector<int>>& grids,
                          int index, EightQueensParameter& param) {
    if (index == param.solve_space_dim) {
        param.result_num++;
        param.result_record.push_back(grids); // 把所有结果记录下来
        return;
    }
    for (int i = 0; i < param.element_dim; ++i) {
        if (IsOkEightQueens(grids, index, i)) {
            auto temp_grids = std::move(
                FillGrid(grids, param.solve_space_dim, index, param.element_dim, i)); // 按照当前的皇后位置，把不能走的地方全部设置为 1
            EightQueensBacktrack(temp_grids, index + 1, param);
        }
    }
}

// o-1 helper
//! \return true 表示该状态之前已经路过
//!         false 表示当前状态还没有被记录过
template <typename T>
bool IsCurStateExist(ZeroOrOneBackpackParameter<T>& param,
                     int index_key , T cur_weight) {
    typedef typename ZeroOrOneBackpackParameter<T>::StateType::value_type ValueType;
    size_t count = param.state.count(index_key); // 判断当前 index 是否之前已经记录过
    if (count == 0) {
        param.state.insert(ValueType(index_key, std::make_pair(cur_weight, T(0))));
        return false;
    }
    bool exist_flag = false;
    const auto range = param.state.equal_range(index_key);
    for_each(
        range.first,
        range.second,
        [&exist_flag, &cur_weight](ValueType& x) {
            if (std::make_pair(cur_weight,T(0)) == x.second)
                exist_flag = true;
        }
    );
    if (!exist_flag) {
        param.state.insert(ValueType(index_key, std::make_pair(cur_weight, T(0))));
        return false;
    }
    return true;

}

// 这个状态其实没必要记录了，主要耗费的空间太大
template <typename T>
bool IsCurStateExist(ZeroOrOneBackpackParameter<T>& param, int index_key,
                     T cur_weight, T cur_value ) {
    typedef typename ZeroOrOneBackpackParameter<T>::StateType::value_type ValueType;

    size_t count = param.state.count(index_key); // 判断当前 index 是否之前已经记录过
    if (count == 0) {
        param.state.insert(ValueType(index_key, std::make_pair(cur_weight, cur_value)));
        return false;
    }
    bool exist_flag = false;
    const auto range = param.state.equal_range(index_key);
    for_each(
        range.first,
        range.second,
        [&exist_flag, &cur_weight, &cur_value](ValueType& x) {
            if (std::make_pair(cur_weight, cur_value) == x.second)
                exist_flag = true;
        }
    );
    if (!exist_flag) {
        param.state.insert(ValueType(index_key, std::make_pair(cur_weight, cur_value)));
        return false;
    }
    return true;
}

// 最大重量问题
template <typename T>
void ZeroOrOneBacktrack(std::vector<T>& backpack, int index,T cur_weight,
                        std::vector<bool> result_record,
                        ZeroOrOneBackpackParameter<T>& param) {
    if (index == param.solve_space_dim || cur_weight == param.weight_up_limit) {
        if (cur_weight > param.max_weight) {
            param.max_weight = cur_weight;
            param.result_record = result_record;
        }
        return;
    }
    // 当前状态如果存在，那么说明接下来的情况走过，没必要继续走了，这里仅仅适用于最大值问题
    // 用来加速
    if (IsCurStateExist(param, index, cur_weight)) return;

    // 遍历每个维度的所有可能情况
    for (int i = 0; i < param.element_dim; ++i) {
        if (i == 0)
            ZeroOrOneBacktrack(backpack, index + 1, cur_weight, result_record, param);
        if (i == 1 && cur_weight + backpack[index] <= param.weight_up_limit) { // 只有满足条件，才能向下走
            result_record[index] = true;
            ZeroOrOneBacktrack(backpack, index + 1, cur_weight + backpack[index], result_record, param);
            result_record[index] = false;
        }
    }
}

// 0-1 max value helper
template <typename T>
void ZeroOrOneBacktrackValue(std::vector<ListOfItems<T, T>>& backpack, int index,
                             T cur_weight, T cur_value,
                             std::vector<bool> result_record,
                             ZeroOrOneBackpackParameter<T>& param) {
    if (index == param.solve_space_dim || cur_weight == param.weight_up_limit) { // 这里需要添加重量的限制
        if (cur_value > param.max_value) {
            param.max_value = cur_value;
            param.result_record = result_record;
        }
        return;
    }
    if (IsCurStateExist(param, index, cur_weight, cur_value)) return;

    // 遍历每个维度的所有可能情况
    for (int i = 0; i < param.element_dim; ++i) {
        if (i == 0)
            ZeroOrOneBacktrackValue(backpack, index + 1, cur_weight, cur_value,
                result_record, param);
        if (i == 1 && cur_weight + backpack[index].weight <= param.weight_up_limit) { // 把不满足条件的直接去掉，剪枝操作
            result_record[index] = true;
            ZeroOrOneBacktrackValue(backpack, index + 1, cur_weight + backpack[index].weight,
                                    cur_value + backpack[index].value,
                                    result_record, param);
            result_record[index] = false;
        }
    }
}

void GridPathBacktrack(std::vector<std::vector<int>> grids,
                       int index, Point<int> cur_pos, // 当前位置
                       GridPathParameter& grid_param) {
    if (index == grid_param.solve_space_dim ||
        grids[grid_param.end.y][grid_param.end.x] == grid_param.passed_flag) { // 这个表示终点已经被走过了

        // 所有解空间维度都过一遍，并且终点是最后一个走过的，才是符合要求的路径
        if (index == grid_param.solve_space_dim &&
            grids[grid_param.end.y][grid_param.end.x] == grid_param.passed_flag) {
            grid_param.result_num++;
            // 打印满足条件的网格
            // 可以在这里保存路径信息，但是需要在该函数中加上一个 vector 来记录走过的坐标
        }
        return;
    }

    for (int i = 0; i < grid_param.element_dim; ++i) {
        if (i == 0 && cur_pos.y - 1 >= 0 &&
            grids[cur_pos.y - 1][cur_pos.x] == grid_param.idle_flag) { // 上
            grids[cur_pos.y - 1][cur_pos.x] = grid_param.passed_flag;  // 标记当前位置已经走过，这里用
            GridPathBacktrack(grids, index + 1,
                Point<int>(cur_pos.x, cur_pos.y - 1), grid_param);
            grids[cur_pos.y - 1][cur_pos.x] = grid_param.idle_flag;    // 空闲标志
        }
        if (i == 1 && cur_pos.y + 1 < grid_param.rows &&
            grids[cur_pos.y + 1][cur_pos.x] == grid_param.idle_flag) { // 下
            grids[cur_pos.y + 1][cur_pos.x] = grid_param.passed_flag;
            GridPathBacktrack(grids, index + 1,
                Point<int>(cur_pos.x, cur_pos.y + 1), grid_param);
            grids[cur_pos.y + 1][cur_pos.x] = grid_param.idle_flag;
        }
        if (i == 2 && cur_pos.x - 1 >= 0 &&
            grids[cur_pos.y][cur_pos.x - 1] == grid_param.idle_flag) { // 左
            grids[cur_pos.y][cur_pos.x - 1] = grid_param.passed_flag;
            GridPathBacktrack(grids, index + 1,
                Point<int>(cur_pos.x - 1, cur_pos.y), grid_param);
            grids[cur_pos.y][cur_pos.x - 1] = grid_param.idle_flag;
        }
        if (i == 3 && cur_pos.x + 1 < grid_param.cols &&
            grids[cur_pos.y][cur_pos.x + 1] == grid_param.idle_flag) {  // 右
            grids[cur_pos.y][cur_pos.x + 1] = grid_param.passed_flag;
            GridPathBacktrack(grids, index + 1,
                Point<int>(cur_pos.x + 1, cur_pos.y), grid_param);
            grids[cur_pos.y][cur_pos.x + 1] = grid_param.idle_flag;
        }
    }
}

} // namespace internal

//! 常规回溯算法解题思路：
//! 根据 8 皇后问题，可以知道解空间的维度是 8x8，也就是每个棋子可以放在 64 个网格中的一个
//! 这样问题的求解其实就太麻烦了，递归深度非常大，因次该问题可以通过给定的条件，可以化简为
//! 一个解空间维度为 8 的问题，可以这样想，8x8 网格上，横竖一条线上不能存在棋子，那么
//! 也就是说每行每列就只能放置一个棋子，那么问题就变成每行放置一个棋子，当 8 行全部放置
//! 上一个棋子，那么问题就解决了，所以该问题的解空间就是 8 维。当然在判断放置棋子时，
//! 检测条件中就不用判断这些棋子是不是在同一行了。只需要判断棋子是不是在同一列和对角线
//! 这其实是条件和解空间的一种转换。通过条件可以把解空间降维。这样在递归时，递归的次数就会减少。
//! \note 需要提前将网格设置 0，放置一个皇后，那么这里就会填充 1
int EightQueens(const int n = 4) {
     // 定义 NXN 的网格，初始化为 0，该网格 2 代表皇后的位置
    std::vector<std::vector<int>> grids(n, std::vector<int>(n, 0));

    int solve_space_dim= internal::SolveSpaceDimension(grids.size());
    int solve_space_element_dim = grids[0].size(); // 因为解空间每一维度的取值范围都是 [0, 网格的列数]
    internal::EightQueensParameter param(solve_space_dim, solve_space_element_dim);

    int sovle_space_dimension_index = 0;           // 解空间起始维度
    internal::EightQueensBacktrack(grids, sovle_space_dimension_index, param);

    // 输出 8 皇后结果
    // for (auto ele : param.result_record) {
    //     for (int i = 0; i < ele.size(); ++i) {
    //         for (int j = 0; j < ele[i].size(); ++j)
    //             std::cout << ele[i][j] << " ";
    //         std::cout << std::endl;
    //     }
    //     std::cout << std::endl;
    // }
    return param.result_num;
}

// 0 1 背包问题
// 在不超过背包所能装载重量的前提下，让背包中物品的总重量最大！
// backpack 中存放的是重量
template <typename T>
int ZeroOrOneBackpack(std::vector<T> backpack, T weight_up_limit) {
    T cur_weight = T(0);
    std::vector<bool> result_record(backpack.size(), false); // 记录获得最大价值量时装载哪些物品

    int solve_space_dim = internal::SolveSpaceDimension(backpack.size());
    int solve_space_element_dim = 2;
    internal::ZeroOrOneBackpackParameter<T> param(weight_up_limit, -1, -1,
        solve_space_dim, solve_space_element_dim, backpack.size());
    internal::ZeroOrOneBacktrack(backpack, 0, cur_weight, result_record, param);

    // 打印当前哪个物品可以放进背包中，1 表示对位的物品可以放进去
    for (int i = 0; i < param.result_record.size(); ++i)
        std::cout << param.result_record[i] << " ";
    std::cout << std::endl;
    return param.max_weight;
}

// 0-1 背包，最大价值量问题
// backpack 中 first 表示重量，second 表示对应价值量
template <typename T>
int ZeroOrOneBackpackValue(std::vector<std::pair<T, T>> backpack, T weight_up_limit) {
    // 将背包内容转化为内部的数据格式
    std::vector<internal::ListOfItems<T, T> > backpacks;
    for (int i = 0; i < backpack.size(); ++i) {
        backpacks.push_back(internal::ListOfItems<T, T>(backpack[i].first, backpack[i].second));
    }
    // 初始化临时量
    T cur_weight = T(0);
    T cur_value = T(0);
    std::vector<bool> result_record(backpack.size(), false); // 记录获得最大价值量时装载哪些物品

    int solve_space_dim = internal::SolveSpaceDimension(backpacks.size());
    int solve_space_element_dim = 2;

    internal::ZeroOrOneBackpackParameter<T> param(weight_up_limit, -1, -1,
        solve_space_dim, solve_space_element_dim, backpacks.size());
    internal::ZeroOrOneBacktrackValue(backpacks, 0, cur_weight,
        cur_value, result_record, param);                    // 0 表示起始背包位

    // 打印当前哪个物品可以放进背包中，1 表示对应位的物品可以放进去
    for (int i = 0; i < param.result_record.size(); ++i)
        std::cout << param.result_record[i] << " ";
    std::cout << std::endl;
    return param.max_value;
}

//! 不同路径问题
//! leetcode: https://leetcode-cn.com/problems/unique-paths-iii/
//! \description 在二维网格上 grid，有 4 种类型的方格
//!             1 表示起始方格，且只有一个起始方格
//!             2 表示结束方格，且只有一个结束方格
//!             0 表示我们可以走过的方格
//!             -1 表示无法跨越的障碍
//!             返回在四个方向（上下左右）上行走时，从起始方格到结束方格的不同路径的数目
//!             保证每一个无障碍方格都要通过一次
//! \method 可以从最后一句话看，每个无障碍方格都要通过一次，那么用回溯算法，就是解空间
//!         大小等于无障碍方格的数量，每一个解空间维度等于
//! \note 如果网格限制路径方向，那么需要修改下面 solve_space_dim，以及内部的走路信息
//!       如果其他类型题目的起始、终止、障碍物、空闲等标识不一致，那么要修改成当前的格式，
int GridPath(std::vector<std::vector<int>>& grids) {
    if (grids.size() == 0) {
        std::cerr << "输入为空" << '\n';
        return 0;
    }
    // 初始化
    // 这几个标识不能重复
    const int idle_flag = 0;
    const int obstacle_flag = -1;
    const int start_flag = 1;
    const int end_flag = 2;
    const int passed_flag = 3;

    int solve_space_element_dim = 4; // 表示只能上下左右，四个方向上走
    internal::Point<int> start;
    internal::Point<int> end;
    int solve_space_dim =
        internal::SolveSpaceDimension(grids, start, end,
                                      start_flag, end_flag);
    grids[end.y][end.x] = 0;         // 这里把终点设置为空闲，表示终点也是解空间的一部分！

    internal::Point<int> cur_pos(start.x, start.y);
    internal::GridPathParameter grid_param(start, end, grids.size(),
        grids[0].size(), idle_flag, passed_flag, end_flag, solve_space_dim,
        solve_space_element_dim);
    internal::GridPathBacktrack(grids, 0, cur_pos, grid_param);

    return grid_param.result_num;
}

//! 从左上角 00 网格，到右下角 33 网格的最短路径
//! \note 这里仅仅示例一个简单的回溯算法，从该回溯算法中
//!       可以发现，在学习回溯算法时，我们可以求解解空间维度
//!       也可以按照下面方式不需要求解解空间维度，只要最后
//!       的索引是我们要求解的 33 位置即可。根据题目的不同，
//!       需要找到回溯的终止条件
//! 这道题是动态规划 42 讲中的例子
int MinDis = std::numeric_limits<int>::max();
std::deque<std::pair<int, int>> Path;
void MinPathBT(std::vector<std::vector<int>>& grid, int i, int j,
               int cur_dis, std::deque<std::pair<int, int>> pos) {
    if (i == grid.size() - 1 && j == grid[0].size() - 1) {
        if (cur_dis < MinDis) {
            MinDis = cur_dis;
            Path = pos;
        }
        return;
    }
    if (i < grid.size() - 1) {
        pos.push_back(std::make_pair(i+1, j));
        MinPathBT(grid, i + 1, j, cur_dis + grid[i + 1][j], pos);
        pos.pop_back();
    }
    if (j < grid[0].size() - 1) {
        pos.push_back(std::make_pair(i, j + 1));
        MinPathBT(grid, i, j + 1, cur_dis + grid[i][j + 1], pos);
        pos.pop_back();
    }
}


} // namespace glib

#endif // GLIB_BACKTRACKING_H_
