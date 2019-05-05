/*
 * CopyRight (c) 2019 gcj
 * File: glib_exception.h
 * Project: algorithm
 * Author: gcj
 * Date: 2019/5/2
 * Description: some exception implementation
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#ifndef GLIB_GLIB_EXCEPTION_H_
#define GLIB_GLIB_EXCEPTION_H_
#include <exception>             // 标准库异常

//! \brief 一些自定义异常
//!
//! \Note
//!      1）还没有实现一个通用的异常框架，当前该文件还不能使用
//!
//! \TODO
//!      1）准备实现一个通用的异常框架，可以用一个类 or 可调用对象，比如 Lambda 表达式
//!
//! \platform
//!      ubuntu16.04 g++ version 5.4.0


namespace glib {
namespace utils {

//! \from 队列为空时操作就会报异常
//! \Note 在使用该宏时，需要内部函数 empty() 函数，想办法如何把该函数变通用
#define REQUIRES_NONEMPTY (                                                            \
    [&]() { if (empty()) throw std::out_of_range("Fetch data from an empty queue!"); } \
)

} // namespace utils
} // namespace glib

#endif // GLIB_GLIB_EXCEPTION_H_
