/*
 * CopyRight (c) 2019 gcj
 * File: types.hpp
 * Project: algorithm
 * Author: gcj
 * Date: 2019/4/15
 * Description: some basic type alias
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#ifndef GLIB_TYPES_H_
#define GLIB_TYPES_H_
#include <unordered_map> // 无序地图，内部实现是哈希表
#include <unordered_set>
#include <map>           // map 内部实现是红黑树
#include <set>
#include <cstddef>       // 定义了 size_t 类型
#include <assert.h>
#include <memory>        // std::shared_ptr

//! \brief 一些自定义标准库别名
//!     包含内容：HashMap、HashSet、KeyMap、Set、SharePtr
//!
//! \TODO
//!      1）加入更多的别名
//!
//! \platform
//!      ubuntu16.04 g++ version 5.4.0

namespace glib {

    // 哈希表---无序地图 输出是无序的
    template <typename _Key, typename _Value>
    using HashMap = std::unordered_map<_Key, _Value>;

    // map
    template <typename _Key, typename _Value>
    using KeyMap = std::map<_Key, _Value>;

    // 哈希 set
    template <typename _Key>
    using HashSet = std::unordered_set<_Key>;

    // set
    template <typename _Key>
    using Set = std::set<_Key>;

    // 共享智能指针
    template <typename _T>
    using SharePtr = std::shared_ptr<_T>;

} // namespace glib

#endif // GLIB_TYPES_H_
