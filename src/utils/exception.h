/*
 * CopyRight (c) 2019 gcj
 * File: exception.h
 * Project: algorithm
 * Author: gcj
 * Date: 2019/5/2
 * Description: some exception implementation
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#ifndef GLIB_EXCEPTION_H_
#define GLIB_EXCEPTION_H_
#include <exception> // 标准库异常

// TODO 实现自己的异常库
//! \from 队列为空时操作就会报异常
//! \Note 在使用该宏时，需要内部函数 empty() 函数，想办法如何把该函数变通用
#define REQUIRES_NONEMPTY (                                                            \
    [&]() { if (empty()) throw std::out_of_range("Fetch data from an empty queue!"); } \
)


#endif // GLIB_EXCEPTION_H_
