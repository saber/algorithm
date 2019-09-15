/*
 * CopyRight (c) 2019 gcj
 * File: divide_and_conquer.hpp
 * Project: algorithm
 * Author: gcj
 * Date: 2019/9/15
 * Description: divide and conquer algorithm simple implementation
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#ifndef GLIB_DIVIDE_AND_CONQUER_H_
#define GLIB_DIVIDE_AND_CONQUER_H_

#include <functional> // std::less_equal 模板函数
#include <algorithm>  // std::copy std::distance
#include <iterator>
#include <vector>

//! \brief 利用分治算法实现求取一组数组的逆序对（正序对）个数
//!     外部调用核心函数：
//!         1）求取逆序对数，并且容器元素归并排序：MergeSortCount()
//!         2）对数据进行归并排序：MergeSort()
//!
//! \Note
//!     1）这两个函数只能操作这几个容器： vector deque array 普通数组
//!
//! \platform
//!     ubuntu16.04 g++ version 5.4.0

namespace glib {
namespace internal { // helper function
//! 两个有序数组的合并，并且求出逆序对个数
template <typename RandomAccessIterator,
          typename OutputIterator,
          typename T = int,
          typename BinaryPred =
            std::less_equal<typename std::iterator_traits<RandomAccessIterator>::value_type>
          >
void Merge(RandomAccessIterator first1, RandomAccessIterator last1,
           RandomAccessIterator first2, RandomAccessIterator last2,
           OutputIterator output,
           T& count,
           BinaryPred comp = BinaryPred()) {
    for (; first1 != last1 && first2 != last2; ) {
        if (comp(*first1, *first2)) { // 默认这里是 <=
            *output = *first1;
            ++first1;
        } else {
            count += T(std::distance(first1, last1));
            *output = *first2;
            ++first2;
        }
        ++output;
    }
    while (first1 != last1) {
        *output = *first1;
        ++first1;
        ++output;
    }
    while (first2 != last2) {
        *output = *first2;
        ++first2;
        ++output;
    }
}

// 这里单纯是归并排序合并操作
template <typename RandomAccessIterator,
          typename OutputIterator,
          typename BinaryPred =
            std::less_equal<typename std::iterator_traits<RandomAccessIterator>::value_type>
          >
void MergeSortHelper(RandomAccessIterator first1, RandomAccessIterator last1,
           RandomAccessIterator first2, RandomAccessIterator last2,
           OutputIterator output, BinaryPred comp = BinaryPred()) {
    for (; first1 != last1 && first2 != last2; ) {
        if (comp(*first1, *first2)) { // 默认这里是 <=
            *output = *first1;
            ++first1;
        } else {
            *output = *first2;
            ++first2;
        }
        ++output;
    }
    while (first1 != last1) {
        *output = *first1;
        ++first1;
        ++output;
    }
    while (first2 != last2) {
        *output = *first2;
        ++first2;
        ++output;
    }

}

} // namespace internal

//! \brief 求出一组数据的有序对数
//! \note 区间是左闭右开，并且这里只能用随机获取迭代器（数组、vector、queue）
//! \complexity O(nlogn) 空间复杂度为 O(n)
template <typename RandomAccessIterator,
          typename T = int,
          typename BinaryPred =
            std::less_equal<typename std::iterator_traits<RandomAccessIterator>::value_type>
          >
void MergeSortCount(RandomAccessIterator first, RandomAccessIterator last,
               T& count, BinaryPred comp = BinaryPred()) {
    auto len = std::distance(first, last);
    if (1 >= len) { return; } // 这里之前 len < 1 然后出现了错误！
    auto mid = first + len / 2; // 这里的实现，导致只能用随机获取迭代器的容器！
    MergeSortCount(first, mid, count, comp);
    MergeSortCount(mid, last, count, comp);
    std::vector<typename std::iterator_traits<RandomAccessIterator>::value_type> merged_temp; // 临时存放合并和的结果
    internal::Merge(first, mid, mid, last, std::back_inserter(merged_temp), count, comp);
    std::copy(merged_temp.begin(), merged_temp.end(), first);
}

//! \brief 求出一组数据的有序对数
//! \note 区间是左闭右开，并且这里只能用随机获取迭代器（数组、vector、queue）
//! \complexity O(nlogn) 空间复杂度 O(n)
template <typename RandomAccessIterator,
          typename BinaryPred =
            std::less_equal<typename std::iterator_traits<RandomAccessIterator>::value_type>
          >
void MergeSort(RandomAccessIterator first, RandomAccessIterator last,
               BinaryPred comp = BinaryPred()) {
    auto len = std::distance(first, last);
    if (1 >= len) { return; } // 这里之前 len < 1 然后出现了错误！
    auto mid = first + len / 2; // 这里的实现，导致只能用随机获取迭代器的容器！
    MergeSort(first, mid, comp);
    MergeSort(mid, last, comp);
    std::vector<typename std::iterator_traits<RandomAccessIterator>::value_type> merged_temp; // 临时存放合并和的结果
    internal::MergeSortHelper(first, mid, mid, last, std::back_inserter(merged_temp), comp);
    std::copy(merged_temp.begin(), merged_temp.end(), first);
}

} // namespace glib

#endif // GLIB_DIVIDE_AND_CONQUER_H_
