/*
 * CopyRight (c) 2019 gcj
 * File: binary_search.hpp
 * Project: algorithm
 * Author: gcj
 * Date: 2019/5/8~12
 * Description: binary_search simple implementation
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#ifndef GLIB_BINARY_SEARCH_HPP_
#define GLIB_BINARY_SEARCH_HPP_
#include <vector>
#include <iostream>
#include <assert.h>
#include <math.h>
#include <iomanip>    // cout 输出小数点
#include <functional> // 使用 std::function<> 可调用对象类型
#include <map>
#include <algorithm>  // 使用 distance

//! \brief 实现了二分搜索的基本功能
//!         基本功能：
//!             1）查找第一个值等于给定值的元素
//!             2）查找最后一个值等于给定值的元素
//!             3）查找第一个大于等于给定值的元素
//!             4）查找最后一个小于等于给定值的元素
//!           对应函数：BinarySearch。根据不同的选项来进行不同功能的切换
//!             具体选项可以参照 BinarySearchOption 枚举类
//!
//!         高级应用
//!             1）求取一个正整数的任意次方且保留指定精度{二分法/Newton迭代法}：Sqrt()
//!                 进行切换方法参考枚举类：SqrtMethod
//!             2）循环有序数组查询指定值问题。leetcode 33 题：SearchRotatedSortedArray
//!                 该题目已经经过 leetcode 测试通过且目前速度排名第一。
//!
//! \note
//!     1）上述基本功能中，还有一个递归的实现，但是递归版本只能用于无重复元素的升序数组。
//!        实际使用时不建议
//!     2）目前所有函数仅仅能够适用于升序数据。没有将降序数据考虑进去
//!
//! \TODO
//!     1）支持降序数据
//!     2）完善递归代码，使其能够与循环形式的代码有一致的功能
//!
//! \platform
//!     ubuntu16.04 g++ version 5.4.0

namespace glib {
using namespace std;

//! \brief 二分搜索模式
//! \note 前四个是非递归实现，最后一个选项是递归方法
enum BinarySearchOption { NON_RECURSION_FIRST_EQUAL,         // 第一个等于指定值
                          NON_RECURSION_LAST_EQUAL,          // 最后一个等于指定值
                          NON_RECURSION_FIRST_GREATER_EQUAL, // 第一个大于等于指定值
                          NON_RECURSION_LAST_LESS_EQUAL,     // 最后一个小于等于指定值
                          RECURSION // 普通递归模式。不能实际应用
                        };

// 二分搜索{递归 + 非递归}实现细节
namespace binary_search_details {

//! \brief 二分查找非递归实现
//! \note
//!      1）适用于调用该函数时，数据是有序的（小--->大），
//!      2）如果数据有重复的，此时默认返回第一个值等于指定值的迭代器索引
//!      3）对于数据的类型，目前只支持内置数值类型，其元素能够直接比较大小。
//1         如果某个类本身重载了 > < <= >= == 这些符号的，其元素也可以是这种类型的 vector
//! \complexity O(log(n))
//! \param array 保留数据的容器
//! \param target 将要查询的值
//! \param search_option 按照哪种模式进行搜索值的查询。见 BinarySearchOption 选项说明
//! \TODO
//!     1）适应降序排序的数据
template <typename T>
typename vector<T>::const_iterator BinarySearchLoop(
                                    const vector<T> &array,
                                    const T target,
                                    const BinarySearchOption search_option =
                                    BinarySearchOption::NON_RECURSION_FIRST_EQUAL) {
    // 首先要保证是有序的，且是从小到大排序
    assert(array.size() >= 1);
    auto low = array.begin();
    auto high = array.end() - 1;
    vector<function<bool(decltype(array.begin()))>> predicates; // 保留二分搜索的判断条件{退出条件、改变区间 low-high 起点}

    if (search_option == BinarySearchOption::NON_RECURSION_FIRST_EQUAL) {       // 查找第一个值等于给定值的元素
         predicates.push_back(
             [&](decltype(array.begin()) middle) {
                 return (*middle == target) and
                        (array.begin() == middle or *decltype(middle)(middle-1) < target);
             }
         );
    } else if (search_option == BinarySearchOption::NON_RECURSION_LAST_EQUAL) { // 查找最后一个值等于给定值的元素
        predicates.push_back(
            [&](decltype(array.begin()) middle) {
                return (*middle == target) and
                (array.end()-1 == middle or *decltype(middle)(middle+1) > target);
            }
        );
    } else if (search_option == BinarySearchOption::NON_RECURSION_FIRST_GREATER_EQUAL) { // 查找第一个大于等于给定值的元素
        predicates.push_back(
            [&](decltype(array.begin()) middle) {
                return (*middle >= target) and
                       (array.begin() == middle or *decltype(middle)(middle-1) < target);
            }
        );
    } else if (search_option == BinarySearchOption::NON_RECURSION_LAST_LESS_EQUAL) { // 查找最后一个小于等于给定值的元素
        predicates.push_back(
            [&](decltype(array.begin()) middle) {
                return (*middle <= target) and
                       (array.end()-1 == middle or *decltype(middle)(middle+1) > target);
            }
        );
    }

    // 第二个判断条件
    if (search_option == BinarySearchOption::NON_RECURSION_FIRST_EQUAL or
        search_option == BinarySearchOption::NON_RECURSION_FIRST_GREATER_EQUAL) {
        predicates.push_back(
            [&](decltype(array.begin()) middle) {
                return (*middle < target); // 小于
            }
        );
    } else if (search_option == BinarySearchOption::NON_RECURSION_LAST_EQUAL or
               search_option == BinarySearchOption::NON_RECURSION_LAST_LESS_EQUAL) {
       predicates.push_back(
           [&](decltype(array.begin()) middle) {
               return (*middle <= target); // 小于等于
           }
       );
    }

    while (low <= high) {
        auto middle = low + (high-low)/2.0;
        // cout << "middle:" << *middle << endl;
        // cout << "distance: " << distance(array.begin(), middle) << endl;
        if (predicates[0](middle)) {        // 退出条件
            return middle;
        } else if (predicates[1](middle)) { // 改变区间 low high 起点
            low = middle + 1;
        } else {
            high = middle - 1;
        }
    }
    return array.end();
}

//! \brief 二分查找递归方法实现（不建议使用）
//! \note
//!      1）适用于数据有序（小--->大），且无重复元素。对于重复元素，此时只能返回其中一个代表值
//!      2）这里的 end 是无效的，保证外部看起来的接口与常用的方法一致
//!      3）仅支持元素能够使用 > < <= >= == 关系元素符，来判断元素之间关系的数据
//! \complexity O(log(n)) 空间复杂度 O(1)
//! \param target 要查询的值
//! \param array_end 代表整个查询数组的结尾。
//!        这里开始用默认参数来记录这个值，然后在递归调用时，手动传递 array_end
//! \return 如果找到了，那么返回 vector 的迭代器索引，如果没有找到，那么返回有序数组的结尾
//! \TODO
//!     1）使当前的递归方法能够像上面的循环方法一样，能够实现多模式，适合重复元素的情况
template <typename T>
typename vector<T>::const_iterator BinarySearchByRecursion(
                                   const typename vector<T>::const_iterator begin,
                                   const typename vector<T>::const_iterator end,
                                   const T target,
                                   const typename vector<T>::const_iterator array_end) {
    // 终止条件
    if (begin > end) {
        return array_end;
    }
    auto middle = begin + (end-begin)/2.0;
    // cout << "middle: " << *middle;
    if (target == *middle) {
        return middle;
    } else if (target <= *middle) {
        return BinarySearchByRecursion(begin, middle-1, target, array_end); // 手动加入数组结尾
    } else {
        return BinarySearchByRecursion(middle+1, end, target, array_end);
    }
}

} // namespace binary_search_details


//! \brief 利用二分搜索的思想解决的某个数的指定次方根问题
namespace binary_search_app {

// 分别表示二分搜索方法、牛顿迭代法
enum SqrtMethod { BINARY_SEARCH, NEWTON };

//! \brief 利用二分搜索思路，求取一个正整数的任意次方根（默认平方根），且保留指定精度（默认精度小数点后三位）
//! \note
//!      1）这个相当于变步长，也就是普通二分搜搜的步长是 1，因此 n 确定的方法其实就是数据的个数
//!      如果开根的次数导致有多个解。那么仅仅能够得到一个正解
//!      2）当前实现的版本，仅仅适用于整数，对于小于 1 的小数时，精度无法达到
//! \complexity (precision-1）*log(n) + log(num)，
//!             n 在这里其实是 10。因为每次的步长是 10^(-i) ，
//!             首先分析刚开始步长为 0.1 的情况，此时如果  num > 1，在 low,high 上，
//!             low 最低为 xx.1，high 最高为 xx.9 此时相差 9 个数，因此 n = 10
//!             小数点依次向下，直到 precision 位，此时 n 都是 10，但是当 n 为整数，且
//!             那么搜索到小数点第一位前，就是 log(num)。
//! \param num 要开方的数
//! \param sqrt_num 开几次方
//! \param precision 保留的精度到小数点后几位
//! \iter_threshold 表示指定要迭代的次数，默认为 1000，表示不限制迭代次数，直到满足条件才会退出
//! \return 最后开根的结果且是正解
//! \TODO
//!     1）调整使其也能适应小数的情况。
double SqrtByBinarySearch(const int num,           const int sqrt_num = 2,
                          const int precision = 3, const int iter_threshold = 1000) {
    assert(num >= 0);
    assert(sqrt_num >= 1);
    assert(precision >=0);
    if (0 == num or 1 == num or 1 == sqrt_num)
        return num;

    double low = 0;
    double high = num;
    double sqrt_value = 0;
    if (num < 1 and num > 0) {
        low = num;
        high = 1;
    }
    int binary_search_count = 0;
    for (int i = 0; i <= precision; i++) {
        if (binary_search_count >= iter_threshold)
            break;
        while (true) {
            binary_search_count++;
            double threshold = pow(10, -i);
            double middle = low + (high-low)/2.0;
            double pow_middle = pow(middle, sqrt_num);
            // cout << "pow_middle: " << pow_middle << endl;
            if ((pow_middle <= num and pow(middle+threshold, sqrt_num) > num)
                or (binary_search_count >= iter_threshold)) {
                sqrt_value = middle;
                low = middle;
                high = middle + threshold;
                // cout << setiosflags(ios::fixed) << setprecision(10)
                //      << "sqrt_value: " << sqrt_value << " " << low << " " << high << endl;
                break;
            }
            if (pow_middle > num) {
                high = middle - threshold;
            } else {
                low = middle + threshold;
            }
        }
    }
    cout << "共计搜索次数: " << binary_search_count <<endl;

    return sqrt_value;
}

//! \brief 二分方法求解一个数的指定次开方（默认平方根），且保留指定精度（默认精度小数点后三位）。
//! \note 在给定小数的时候，优先使用下面的方法。如果开根的次数导致有多个解。那么仅仅能够得到一个正解
//! \complexity 待定！
//! \param num 要开方的数
//! \param sqrt_num 开几次方
//! \param precision 保留的精度到小数点后几位
//! \iter_threshold 表示指定要迭代的次数，默认为 1000，表示不限制迭代次数，直到满足条件才会退出
//! \return 最后开根的结果且是正解
//! \TODO
//!     1）补充当前实现的复杂度
double SqrtByBinarySearch(const double num,        const int sqrt_num = 2,
                          const int precision = 3, const int iter_threshold = 1000) {
    assert(num >= 0);
    assert(sqrt_num >= 1);
    assert(precision >=0);

    if (0 == num or 1 == num or 1 == sqrt_num)
        return num;

    double low = 0;
    double high = num;
    double sqrt_value = 0;

    if (num < 1 and num > 0) {
        low = num;
        high = 1;
    }
    double threshold = pow(10, -precision);
    int binary_search_count = 0;
    while (true) {
        binary_search_count++;
        double middle = low + (high - low)/2;
        double pow_middle = pow(middle, sqrt_num);
        if (fabs(pow_middle - num) <= threshold *threshold
            or  (binary_search_count >= iter_threshold)) {
            sqrt_value = middle;
            break;
        }
        if (pow_middle > num) {
            high = middle - threshold;
        } else {
            low = middle + threshold;
        }
    }
    cout << "共计搜索次数: " << binary_search_count <<endl;
    return sqrt_value;
}

//! \brief 利用 Netown 方法求解。xk1 = xk - f(xk)/f'(xk) 依次次迭代直到 |x_k1 - x_k| < epsilon
//! \complexity O()
//! \param num 要开方的数
//! \param sqrt_num 开几次方
//! \param precision 保留的精度到小数点后几位
//! \return 最后开根的结果且是正解
//! \method 首先调用二分搜索的方法迭代 5~10 次左右（根据 n 动态调整 == 1000 对应 9次, 100万，需要 20 次），
//!         求取一个初始值，然后再用 Newton 方法
//! \reference 数值分析教材，非线性方程求根——Newton 迭代法
//! \TODO
//!     1）分析 Netwon 方法的复杂度。数学书上是说平方收敛。但是不知道这里如何表示。可能是接近常数收敛
template <typename T>
double SqrtByNetwon(const T num, const int sqrt_num = 2, const int precision = 3) {
    assert(num >= 0);
    assert(sqrt_num >= 1);
    assert(precision >=0);
    if (0 == num or 1 == num or 1 == sqrt_num)
        return num;
    // 根据给定的值计算初始迭代次数，为牛顿迭代提供初始值
    int iter_num = 0;
    if (num <= 0.1)
        iter_num = 5;
    else if (num <= 1000)
        iter_num = 10;
    else if (num <= 10000)
        iter_num = 15;
    else if (num <= 1000000)
        iter_num = 20;
    else
        iter_num = 30;

    // 利用二分搜索方法提供初始值
    double init_sqrt_value = SqrtByBinarySearch(num, sqrt_num, precision, iter_num);
    // cout << "init_sqrt_value: " << init_sqrt_value << endl;
    double x_k = init_sqrt_value;
    double x_k1 = x_k + 1;
    double threshold = pow(10, -precision);
    double f_x_k = pow(x_k, sqrt_num) - num;
    double fd_x_k = sqrt_num * pow(x_k, sqrt_num - 1);
    while (true) {
        x_k1 = x_k - f_x_k/fd_x_k;
        if (fabs(x_k1 - x_k) > threshold)
            break;
        x_k = x_k1;
        f_x_k = pow(x_k, sqrt_num);
        fd_x_k = sqrt_num * pow(x_k, sqrt_num - 1);
    }

    return x_k1;
}

} // namespace binary_search_app


//------------------------------------二分搜索外部调用接口-------------------------------

//! \brief 二分搜索的调用接口，在一个有序数组中查找目标值
//! \note
//!     1）仅支持数据是有序的（小--->大）
//!     2）支持四种模式的非递归实现。可以到 BinarySearchLoop 枚举处处查看具体功能
//!     3）在实际处理数据时，不能用递归选项。目前递归选项代码还有一些 bug 和未实现的功能
//!     4）仅支持元素能够使用 > < <= >= == 关系运算符，来判断元素之间关系的数据
//! \TODO
//!     1）完善二分递归代码的功能（参照二分循环代码已经实现好的功能）
//!     2）处理降序数据（由大到小排列）。
template <typename T>
typename vector<T>::const_iterator BinarySearch(
                                    const vector<T> &array,
                                    const T target,
                                    const BinarySearchOption option =
                                    BinarySearchOption::NON_RECURSION_FIRST_EQUAL) {
    if (0 == array.size()) { // 或者直接报错！
        cerr << "数据集合至少有一个元素！" << '\n';
        return array.end();
    }
    if (option == BinarySearchOption::RECURSION) // 实际应用时，不要使用这个选项
        return binary_search_details::BinarySearchByRecursion(
                                    array.begin(), array.end()-1,
                                    target, array.end()
                                );
    else
        return binary_search_details::BinarySearchLoop(array, target, option);
}

//! \brief 利用二分搜索，简单实现一个数的开根运算（默认平方根），且保留指定精度（默认精度小数点后三位）
//! \note 1）此时仅仅能够得到一个正解
//!       2）可以根据 sqrt_method 来切换不同的方法。需要参考 SqrtMethod 枚举值
//! \complexity  O(log(n))
//! \param num 被开方操作数
//! \param sqrt_num 指定开几次方
//! \param precision 保留的精度到小数点后几位
//! \return 最后开根的结果且是正解
template <typename T>
double Sqrt(const T num, const int sqrt_num = 2, const int precision = 3,
            const binary_search_app::SqrtMethod sqrt_method =
                binary_search_app::SqrtMethod::BINARY_SEARCH) {
    if (sqrt_method == binary_search_app::SqrtMethod::BINARY_SEARCH) {
        cout << "solve sqrt by binary search method." << endl;
        return binary_search_app::SqrtByBinarySearch(num, sqrt_num, precision);
    } else { // newton method
        cout << "solve sqrt by newton method." << endl;
        return binary_search_app::SqrtByNetwon(num, sqrt_num, precision);
    }
}

//! \brief 针对有序数组是一个循环有序数组，比如 4 5 6 1 2 3 求取一个值等于给定值的二分查找算法
//! \note
//!     1）仅仅针对数据是升序，且是 int 类型
//!     2）函数内部所说的分界点 pivot 表示最大值。此时是针对升序排序的数据
//! \complexity O(logn) = O(查找分界点) + O(正式查找目标值)，两部分都是 log(n)
//! \return 返回目标值所在下标。-1：表示没有找到
//! \method 先找分界点将其区间分成两个有序部分，判定给定值所属哪个区间，就在哪个区间再次进行二分查找
//!         二分选择 pivot（最大值） 思路：分界点的性质：大于右边值。详细过程看下面代码求解分界点处
int SearchRotatedSortedArray(vector<int>& nums, int target) {
    if (nums.size() < 1)
        return -1;

    // 快速检验头尾元素
    if (nums[0] == target)
        return 0;
    else if (nums[nums.size()-1] == target)
        return nums.size() - 1;
    if (nums.size() <= 2)
        return -1;

    // 1 先找 pivot 点（此时对应最大值）
    // 下面实现方法，对于整个数组是有序时，是不能找到分界点的，此时 pivot 仍然保留原来的值 -1
    int pivot = -1;
    int low = 0;
    int high = nums.size() - 1;
    while (low <= high) {
        int middle = low + (high - low)/2;
        if ((middle+1) < int(nums.size()) and nums[middle] > nums[middle+1]) {
            pivot = middle;
            break;
        } else if (nums[low] <= nums[middle]) {
            low  = middle + 1;
        } else {
            high = middle - 1;
        }
    }
    cout << "pivot: " << pivot << endl;

    // 退出上面的循环有两种情况：
    //  1）数组本身整体就是循环的，那么无法找到 pivot ，此时 pivot = -1
    //  2）数组是循环有序的，那么可以找到最大值点，分界点即为 pivot 值

    // 2 从两边有序（升序）数组中选择一个区间，按照二分方式进行查找
    // 1）选定二分区间
    if (pivot < 0) { // 整个数组是有序的，在整个区间进行二分
        low  = 0;
        high = nums.size() - 1;
    } else if (nums[0] > target) { // 目标值在右区间
        low  = pivot + 1;
        high = nums.size() - 1;
    } else { // 目标值在左区间
        low  = 0;
        high = pivot;
    }

    // 2）进行二分查找
    while (low <= high) {
        int middle = low + (high - low)/2;
        if (nums[middle] == target) {
            return middle;
        } else if (nums[middle] < target) {
            low = middle + 1;
        } else {
            high = middle - 1;
        }
    }
    return -1;
}

} // namespace glib

#endif
