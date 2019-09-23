/*
 * CopyRight (c) 2019 gcj
 * File: dynamic_program.hpp
 * Project: algorithm
 * Author: gcj
 * Date: 2019/9/19
 * Description: dynamic programming algorithm simple implementation examples
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */
#ifndef GLIB_DYNAMIC_PROGRAM_HPP_
#define GLIB_DYNAMIC_PROGRAM_HPP_

#include <vector>
#include <iostream>
#include <algorithm> // std::distance
#include <deque>

//! \brief 简单实现动态规划典型问题：o-1 背包问题（最大价值、最大重量）
//!     外部调用函数：
//!         1）0-1 背包最大重量：BackpackMaxWeight()
//!                           可以输出所有背包的装载情况
//!         3）0-1 背包最大重量：BackpackMaxWeight1()
//!                           同上，但是空间复杂度降低，不能输出所有背包装载情况
//!         4）0-1 背包最大价值：BackpackMaxValue()
//!                           可以输出所有背包的装载情况
//!         5）0-1 背包最大价值：BackpackMaxValue2()
//!                           同上，但是空间复杂度降低，不能输出所有背包装载情况
//!     内部辅助核心函数：
//!         1）0-1 背包最大重量类问题输出满足最大值时，所有的物品装载情况：BackTrackMaxWeight()
//!         2）0-1 背包最大价值类问题输出满足最大值时，所有的物品装载情况：BackTrackMaxValue()
//!
//! \TODO
//!     1）最后一个优惠券问题，应该扩展成有多个不限量满 200 减 50，然后 n 个物品全部买下，
//!        找到最少的花费方案
//!     2）课后思考题之杨辉三角形，钱币找零问题
//!
//! \platform
//!     ubuntu16.04 g++ version 5.4.0

namespace glib {
namespace internal {
// 背包参数
struct BackpackParam {
    int goods_state_num; // 物品的状态数（放入还是不放入）
    int goods_num;       // 物品个数
    int weight_up_limit; // 背包最大重量限制
    std::vector<std::vector<bool>> states; // 动态规划状态
    BackpackParam(int _goods_state_num,
                  int _goods_num,
                  int _weight_up_limit)
                  : goods_state_num(_goods_state_num),
                    goods_num(_goods_num),
                    weight_up_limit(_weight_up_limit),
                    states(_goods_num, std::vector<bool>(_weight_up_limit + 1, false)) {}
}; // struct BackpackParam

// o-1 max weight helper
//! \brief 根据传递进来的状态矩阵，返推断符合条件的所有路径
//! \note 这里的实现仅仅适用于 0-1 背包最大重量问题
void BackTrackMaxWeight(const std::vector<std::vector<bool>>& states,
                        const std::vector<int>& goods,
                        std::vector<std::vector<bool>>& record_all_path,
                        std::vector<bool> record,
                        int index, int cur_weight) {
    if (index == 0 || cur_weight == 0 ) {
        if (cur_weight != 0)
            record[0] = true;
        record_all_path.push_back(record);
        return;
    }
    // 这里每次都会判断两种情况，其中之一满足，那么就能继续探索
    for (int i = 0; i < 2; ++i) {
        // 判断第一种情况，当前重量是由前一个时刻直接复制的
        if (i == 0 && states[index - 1][cur_weight] == true) {
            BackTrackMaxWeight(states, goods, record_all_path, record, index - 1, cur_weight);
        }

        // 判断第二种情况，当前重量是加上当前 index 对应的物品后才达到的
        if (i == 1 && cur_weight - goods[index] >= 0 && // note 这里的索引一定要保证有效！
            states[index - 1][cur_weight - goods[index]] == true) {
            record[index] = true;
            BackTrackMaxWeight(states, goods, record_all_path, record, index - 1, cur_weight - goods[index]);
            record[index] = false;
        }
    }
}

// 0-1 max value helper
//! \brief 根据传递进来的状态矩阵，返推断符合条件的所有路径
void BackTrackMaxValue(const std::vector<std::vector<int>>& states,
                       const std::vector<std::pair<int, int>>& goods,
                       std::vector<std::vector<bool>>& record_all_path,
                       std::vector<bool> record,
                       int index, int cur_weight) {
    if (index == 0 || cur_weight == 0 ) {
        if (cur_weight != 0)
            record[0] = true;
        record_all_path.push_back(record);
        return;
    }
    // 这里每次都会判断两种情况，其中之一满足，那么就能继续探索
    for (int i = 0; i < 2; ++i) {
        // 判断第一种情况，当前重量是由前一个时刻直接复制的
        if (i == 0 && states[index - 1][cur_weight] == states[index][cur_weight]) {
            BackTrackMaxValue(states, goods, record_all_path, record, index - 1, cur_weight);
        }

        // 判断第二种情况，当前重量是加上当前 index 对应的物品后才达到的
        if (i == 1 && cur_weight - goods[index].first >= 0 &&
            states[index - 1][cur_weight - goods[index].first] ==
            states[index][cur_weight] - goods[index].second) {
            record[index] = true;
            BackTrackMaxValue(states, goods, record_all_path, record, index - 1,
                              cur_weight - goods[index].first);
            record[index] = false;
        }
    }
}

} // namespace internal

//! param goods 所有物品
//! param weight_up_limit 重量上限
//! \complexity O(物品量 * 最大限制重量)，空间复杂度 O(物品重量 * 最大限制重量)
//! \return 返回最大重量
int BackpackMaxWeight(const std::vector<int>& goods, int weight_up_limit) {
    // 这里的 2 是根据问题来决定的，比如背包装还是不装，就是代表 2 种状态
    internal::BackpackParam param(2, goods.size(), weight_up_limit);

    // 初始化 0 状态（两种情况）
    for (int i = 0; i < param.goods_state_num; ++i) {
        if (i == 0)
            param.states[0][0] = true;
        if (i == 1 && goods[0] <= weight_up_limit)
            param.states[0][goods[0]] = true;
    }

    // 从第 1 个开始，依次从前一时刻推断出当前时刻的状态，
    for (int i = 1; i < goods.size(); ++i) {
        // 遍历前一时刻的状态矩阵，推导出当前时刻背包的状态

        // 当前背包状态是由当前物品装还是不装来决定，因此有两种情况（两个叉）
        // 遍历每个物品对应的所有状态，实际上与回溯算法对应的操作差不多
        for (int ii = 0; ii < param.goods_state_num; ++ii) {
            // 当前物品不装，只需要继承前一时刻的状态即可
            if (ii == 0) {
                for (int j = 0; j <= weight_up_limit; ++j) {
                    if (param.states[i - 1][j] == true)
                        param.states[i][j] = true;
                }
            }

            // 当前物品装，那么需要在上一时刻基础上，继续添加当前物品的重量
            if (ii == 1) {
                for (int j = 0; j <= weight_up_limit - goods[i]; ++j) {
                    if (param.states[i - 1][j] == true)
                        param.states[i][j + goods[i]] = true;
                }
            }
        }
    }

    // 输出状态矩阵
    // for (int i = 0; i < param.states.size(); ++i) {
    //     for (int j = 0; j < param.states[i].size(); ++j) {
    //         std::cout << param.states[i][j] << " ";
    //     }
    //     std::cout << std::endl;
    // }
    // 找到最大的重量
    int max_weight = 0;
    auto iter_begin = param.states[goods.size() - 1].rbegin();
    auto iter_end   = param.states[goods.size() - 1].rend();
    for (; iter_begin != iter_end; ++iter_begin) {
        if (*iter_begin) {
            max_weight = std::distance(iter_begin, iter_end - 1);
            break;
        }
    }

    // 根据状态矩阵，反向推断满足最大条件的物品装载情况，这里仅仅找到其中一条路径
    int j = max_weight;
    std::vector<bool> record(goods.size(), false);
    std::vector<std::vector<bool>> record_all;

    // 找到满足最大重量的其中一种情况

    // 下面两种情况，都能够从状态矩阵中推导出结果，所以中间可能会有多条路径
    // 这里的做法，仅仅选择了一条路径，其实可以把状态矩阵传递到一个函数中，
    // 在哪个函数中利用回溯算法，找到所有满足条件的路径，因为满足条件的路径很少，
    // 所以回溯算法也能很快的找到所有路径
    for (int i = goods.size() - 1; i >= 1; --i) { // 从后向前遍历按每个解空间元素
        // 这里表示优先选择不装当前物品
        if (param.states[i - 1][j]) {

        } else if (j - goods[i] >= 0 && param.states[i - 1][j - goods[i]]) {
            record[i] = true;
            j = j - goods[i];
        }

        // 这里表示优先选择装载当前物品
        // if (j - goods[i] >= 0 && param.states[i - 1][j - goods[i]]) {
        //     record[i] = true;
        //     j = j - goods[i];
        // } else if (param.states[i - 1][j]) { // 另一条路径也可能发生
        //     record[i] = false;
        // }
    }
    // 处理第一个物品是否装载，实际上只要第一个物品不为 0，这里就要把第一个物品装进去
    if (j != 0) record[0] = true;

    // 输出满足条件的其中一种情况（1 对应物品需要装进背包）
    std::cout << "输出满足条件的其中一种情况: " << std::endl;
    for (auto ele : record)
        std::cout << ele << " ";
    std::cout << std::endl;

    // 下面的算法可以找到满足条件的所有情况！
    std::cout << "输出满足条件的背包装载物品情况:" << std::endl;
    std::vector<std::vector<bool>> record_all_path;
    std::vector<bool> temp_record(goods.size(), false);
    int index = goods.size() - 1;
    int cur_weight = max_weight;
    internal::BackTrackMaxWeight(param.states, goods, record_all_path,
                                 temp_record, index, cur_weight);
    // 输出满足条件的所有的情况
    for (int i = 0; i < record_all_path.size(); ++i) {
        int init_weight = 0;
        for (int j = 0; j < record_all_path[i].size(); ++j) {
            std::cout << record_all_path[i][j] << " ";
            if (record_all_path[i][j] == true)
                init_weight += goods[j];
        }
        std::cout << "验证" << i << ": " << init_weight << std::endl;
    }

    return max_weight;
}

//! \note 这种做法无法推断出满足条件的物品装载情况，只能找到最大值
//!       节省了空间，但是重要的状态变化信息丢失，不能输出背包装载详细情况
//! param goods 所有物品
//! param weight_up_limit 重量上限
//! \complexity O(物品量 * 最大限制重量)，空间复杂度 O(最大限制重量)
//! \return 返回最大重量
int BackpackMaxWeight2(const std::vector<int>& goods, int weight_up_limit) {
    int dim = 2;
    std::vector<bool> states(weight_up_limit + 1, false);

    // 初始化零状态
    for (int i = 0; i < dim; ++i) {
        if (i == 0)
            states[0] = false;
        if (i == 1 && goods[0] <= weight_up_limit)
            states[goods[0]] = true;
    }

    // 仍然遍历每个背包物品，然后每次更新就用一个数组来记录即可
    // 这里没有遍历两个状态，是因为对于 0 来说，状态不会改变，
    // 所以我们只需要在装入当前物品时才有机会改变，
    // 并且仅仅改变当前状态的最大重量即可
    // 注意这里是从后向前遍历，否则会出现重复！
    for (int i = 1; i < goods.size(); ++i) {
        for (int ii = 0; ii < dim; ++ii) {
            if (ii == 0) {
                // 继承前一个状态，所以这里不需要写代码
            }
            if (ii == 1) {
                for (int j = weight_up_limit - goods[i]; j >= 0; --j) {
                    if (states[j])
                        states[j + goods[i]] = true;
                }
            }
        }
    }

    // 找到最大值
    for (auto iter = states.rbegin(); iter != states.rend(); ++iter)
        if (*iter) return std::distance(iter, --states.rend());
}

//! \note 可以输出所有背包装载情况！
//! \param goods 内部 <重量，价值>
//! param weight_up_limit 重量上限
//! \complexity 时间复杂度 O(物品个数 * 重量上限) 空间复杂度 O(物品个数 * {重量上限 + 1})
//! \return 返回最大价值
int BackpackMaxValue(const std::vector<std::pair<int, int>>& goods,
                     int weight_up_limit) {
    std::vector<std::vector<int>> states(goods.size(),
                                         std::vector<int>(weight_up_limit + 1, -1));
    int dim = 2;
    int goods_num = goods.size();

    // 初始化 0 状态
    for (int i = 0; i < goods_num; ++i) {
        if (i == 0)
            states[0][0] = 0;
        if (i == 1 && goods[0].first <= weight_up_limit)
            states[0][goods[0].first] = goods[0].second;
    }

    // 从第 1 个开始，依次推断出各种不同的状态，
    for (int i = 1; i < goods_num; ++i) {
        for (int ii = 0; ii < dim; ++ii) {
            // 当前物品不装，只需要继承前一时刻的状态即可
            if (ii == 0) {
                for (int j = 0; j <= weight_up_limit; ++j) {
                    if (states[i - 1][j] >=0)
                        states[i][j] = states[i - 1][j];
                }
            }

            // 当前物品装，那么需要在上一时刻基础上，继续添加当前物品的重量
            if (ii == 1) {
                for (int j = 0; j <= weight_up_limit - goods[i].first; ++j) {
                    if (states[i - 1][j] >= 0) {
                        states[i][j + goods[i].first] =
                            std::max(states[i][j + goods[i].first], states[i - 1][j] + goods[i].second);
                    }

                }
            }
        }
    }

    // 输出状态矩阵
    // for (int i = 0; i < states.size(); ++i) {
    //     for (int j = 0; j < states[i].size(); ++j) {
    //         std::cout << states[i][j] << " ";
    //     }
    //     std::cout << std::endl;
    // }
    // 找到最大的重量
    auto iter_begin = states[goods.size() - 1].rbegin();
    auto iter_end   = states[goods.size() - 1].rend();
    int max_value = 0;
    int max_weight = -1;
    for (; iter_begin != iter_end; ++iter_begin) {
        if (*iter_begin > max_value) {
            max_value = *iter_begin;
            max_weight = std::distance(iter_begin, iter_end - 1);
        }
        // if (*iter_begin) return std::distance(iter_begin, --iter_end);
    }

    // 推断哪些些物品需要装载到背包中
    int j_value = max_value;
    int j_weight = max_weight;
    std::vector<bool> record(goods.size(), false);
    for (int i = goods.size(); i >= 1; --i) {
        if (j_weight - goods[i].first >= 0 &&
            states[i - 1][j_weight - goods[i].first] == j_value - goods[i].second) {
            record[i] = true;
            j_value  -= goods[i].second;
            j_weight -= goods[i].first;
        } // 优先判断是不是前一个状态传递的
    }
    // 处理第一个物品
    if (j_weight == goods[0].first) record[0] = true;

    // 输出那些物品需要装（1 对应物品需要装进背包）
    std::cout << "找到其中一个满足条件的路径信息" << std::endl;
    for (const auto& ele : record)
        std::cout << ele << " ";
    std::cout << std::endl;

    // 找到所有满足条件的路径信息
    std::cout << "找到所有满足最大值条件的路径信息" << std::endl;
    std::vector<std::vector<bool>> record_all_path;
    std::vector<bool> temp_record(goods.size(), false);
    int index = goods.size() - 1; // 从最后一个物品开始从后向前推
    int cur_weight = max_weight;
    internal::BackTrackMaxValue(states, goods, record_all_path, temp_record, index, cur_weight);

    // 输出满足条件的所有的情况
    for (int i = 0; i < record_all_path.size(); ++i) {
        int init_value = 0;
        for (int j = 0; j < record_all_path[i].size(); ++j) {
            std::cout << record_all_path[i][j] << " ";
            if (record_all_path[i][j] == true)
                init_value += goods[j].second;
        }
        std::cout << "验证" << i << ": " << init_value << std::endl;
    }
    return max_value;
}

//! \brief 同上
//! \note 该函数无法推断出哪些物品需要装，那些不需要装，降低空间复杂度，时间复杂度不变
int BackpackMaxValue2(const std::vector<std::pair<int, int>>& goods,
                      int weight_up_limit) {
    std::vector<int> states(weight_up_limit + 1, -1);
    int dim = 2;
    int goods_num = goods.size();

    // 初始化 0 状态
    for (int i = 0; i < goods_num; ++i) {
        if (i == 0)
            states[0] = 0;
        if (i == 1 && goods[0].first <= weight_up_limit)
            states[goods[0].first] = goods[0].second;
    }

    // 从第 1 个开始，依次推断出各种不同的状态，
    for (int i = 1; i < goods_num; ++i) {
        for (int ii = 0; ii < dim; ++ii) {
            // 当前物品不装，只需要继承前一时刻的状态即可
            if (ii == 0) {
                // for (int j = 0; j <= weight_up_limit; ++j) { // 保持值不变
                //     if (states[j] >=0)
                //         states[j] = states[j];
                // }
            }

            // 当前物品装，那么需要在上一时刻基础上，继续添加当前物品的重量
            if (ii == 1) {
                for (int j = weight_up_limit - goods[i].first; j >=0 ; --j) {
                    if (states[j] >= 0) {
                        states[j + goods[i].first] =
                            std::max(states[j + goods[i].first], states[j] + goods[i].second);
                    }

                }
            }
        }
    }

    // 找到最大的重量
    auto iter_begin = states.rbegin();
    auto iter_end   = states.rend();
    int max_value = -1;
    for (; iter_begin != iter_end; ++iter_begin) {
        if (*iter_begin > max_value)
            max_value = *iter_begin;
        // if (*iter_begin) return std::distance(iter_begin, --iter_end);
    }
    return max_value;
}

//! \Description 满 200 减 50 最大程度接近满减条件
//! \note 会输出所有购买的物品信息
//! \param goods 内部 <价值>
//! \complexity 时间复杂度 O(物品个数 * 重量上限) 空间复杂度 O(物品个数 * {总价值上限 + 1})
//! \return 返回最大价值
int DoubleOneOne(const std::vector<int>& goods, int value_down_limit) {
    int value_up_limit = std::accumulate(goods.begin(), goods.end(), 0);
    if (value_down_limit > value_up_limit) {
        std::cout << "所有物品加起来也无法达到满减条件!" << std::endl;
        return -1;
    }
    if (value_down_limit == value_up_limit) {
        std::cout << "直接购买所有即可达到满减条件!" << std::endl;
        return -1;
    }
    std::vector<std::vector<bool>> states(goods.size(),
                                          std::vector<bool>(value_up_limit + 1, 0));
    int dim = 2;
    int goods_num = goods.size();

    // 初始化 0 状态
    for (int i = 0; i < goods_num; ++i) {
        if (i == 0)
            states[0][0] = true;
        if (i == 1)
            states[0][goods[0]] = true;
    }

    // 从第 1 个开始，依次推断出各种不同的状态，
    for (int i = 1; i < goods_num; ++i) {
        for (int ii = 0; ii < dim; ++ii) {
            // 当前物品不装，只需要继承前一时刻的状态即可
            if (ii == 0) {
                for (int j = 0; j <= value_up_limit; ++j) {
                    if (states[i - 1][j])
                        states[i][j] = states[i - 1][j];
                }
            }

            // 当前物品装，那么需要在上一时刻基础上，继续添加当前物品的价值
            if (ii == 1) {
                for (int j = 0; j <= value_up_limit - goods[i]; ++j) {
                    if (states[i - 1][j]) {
                        states[i][j + goods[i]] = true;
                    }
                }
            }
        }
    }

    // 输出状态矩阵
    // for (int i = 0; i < states.size(); ++i) {
    //     for (int j = 0; j < states[i].size(); ++j) {
    //         std::cout << states[i][j] << " ";
    //     }
    //     std::cout << std::endl;
    // }
    // 找到接近 200 的最大的重量
    auto iter_begin = states[goods.size() - 1].begin() + value_down_limit;
    auto iter_end   = states[goods.size() - 1].end();
    int max_value = -1;
    for (; iter_begin != iter_end; ++iter_begin) {
        if (*iter_begin) {
            max_value = std::distance(states[goods.size() - 1].begin(), iter_begin);
            break;
        }
    }

    // 找到最接近 200 的物品装载情况
    std::cout << "找到满足 200 减 50 的其中一种装载情况" << std::endl;
    std::vector<bool> record(goods.size(), false);
    int j = max_value;
    for (int i = goods.size() - 1; i >= 1; --i) {
        if (j - goods[i] >= 0 && states[i - 1][j -goods[i]]) {
            record[i] = true;
            j -= goods[i];
        }
    }
    if (j != 0) record[0] = true; // 表示第一个物品必须要装

    // 输出背包哪个物品需要装（1 对应物品需要装进背包）
    int init = 0;
    for (int i = 0; i < record.size(); ++i) {
        if (record[i]) init += goods[i];
        std::cout << record[i] << " ";
    }
    std::cout << std::endl << "验证: " << init << std::endl;

    // 找到所有满足 200 减 50 的情况
    std::cout << "找到满足 200 减 50 的所有装载情况" << std::endl;
    std::vector<std::vector<bool>> record_all_path;
    std::vector<bool> temp_record(goods.size(), false);
    int index = goods.size() - 1;
    int cur_value = max_value;
    internal::BackTrackMaxWeight(states, goods, record_all_path,
                                 temp_record, index, cur_value);

    // 输出满足条件的所有的情况
    for (int i = 0; i < record_all_path.size(); ++i) {
        int init_value = 0;
        for (int j = 0; j < record_all_path[i].size(); ++j) {
            std::cout << record_all_path[i][j] << " ";
            if (record_all_path[i][j] == true)
                init_value += goods[j];
        }
        std::cout << "验证" << i << ": " << init_value << std::endl;
    }

    return max_value;
}

//! 从左上角 00 网格，到右下角 33 网格的最短路径
//! \note 这里仅仅示例一个简单的回溯算法，从该回溯算法中
//!       可以发现，在学习回溯算法时，我们可以求解解空间维度
//!       也可以按照下面方式不需要求解解空间维度，只要最后
//!       的索引是我们要求解的 33 位置即可。根据题目的不同，
//!       需要找到回溯的终止条件
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

// 该状态转移表是根据回溯算法来确定状态表的
// 但是内部会包含状态转移方程，也就是问题的
// 本质
void StateTable(std::vector<std::vector<int>>& grid) {
    std::vector<std::vector<int>> states = std::vector<std::vector<int>>(4, std::vector<int>(4, 0));
    // 初始化 0 行 0 列
    int sum = 0;
    for (int i = 0; i < grid.size(); ++i) {
        sum += grid[i][0];
        states[i][0] = sum;
    }

    sum = 0;
    for (int j = 0; j < grid[0].size(); ++j) {
        sum += grid[0][j];
        states[0][j] = sum;
    }

    for (int i = 1; i < grid.size(); ++i) {
        for (int j = 1; j < grid[0].size(); ++j) {
            states[i][j] = grid[i][j] + std::min(states[i - 1][j], states[i][j - 1]); // 这里就是状态转移方程
        }
    }
    std::cout << states[3][3] << std::endl;
}

// 状态转移方程法，直接根据状态转移方程书写递推公式，外加备忘录方法避免出现重复状态
// 备忘录的方法是同样是根据回溯算法中推导出的状态有关
int StateTransitionEquation(std::vector<std::vector<int>>& grid,
                            std::vector<std::vector<int>>& mem, int i, int j) {
    if (i == 0 && j == 0) return grid[0][0];
    if (mem[i][j] > 0) return mem[i][j];
    int left_value = std::numeric_limits<int>::max();
    if (j - 1 >= 0)
        left_value = StateTransitionEquation(grid, mem, i, j - 1);
    int up_value = std::numeric_limits<int>::max();
    if (i - 1 >= 0)
        up_value = StateTransitionEquation(grid, mem, i - 1, j);

    int cur_min_dis = grid[i][j] + std::min(left_value, up_value);
    mem[i][j] = cur_min_dis;
    return cur_min_dis;
}

} // namespace glib

#endif // GLIB_DYNAMIC_PROGRAM_HPP_
