/*
 * CopyRight (c) 2019 gcj
 * File: consistent_hash.hpp
 * Project: algorithm
 * Author: gcj
 * Date: 2019/8/19
 * Description: glib public internal macros
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */
#ifndef GLIB_PUBLIC_INTERNAL_MACROS_H_
#define GLIB_PUBLIC_INTERNAL_MACROS_H_

#include <cstddef> // for size_t

//! \brief 简单指定一下类的内部自用一些宏定义

//! \brief 在类中不允许拷贝和赋值
//! \use 用在类的 private 属性中
#define GLIB_DISALLOW_COPY_AND_ASSIGN_PRIVATE(TypeName) \
    TypeName(const TypeName& others); \
    TypeName& operator=(const TypeName& others)

//! \brief 在类中不允许拷贝和赋值
//! \use 可以使用在 pubic 下，这里用了 delete 方式
#define GLIB_DISALLOW_COPY_AND_ASSIGN_PUBLIC(TypeName) \
    TypeName(const TypeName& others) = delete; \
    TypeName& operator=(const TypeName& others) = delete

//! \brief 不允许默认构造函数（构造、拷贝、赋值）
//! \use 建议用在类中只有静态方法时。且只能放在 private 范围内
#define GLIB_DISALLOW_IMPLICIT_CONSTRUCTORS_PRIVATE(TypeName) \
    TypeName(); \
    GLIB_DISALLOW_COPY_AND_ASSIGN_PRIVATE(TypeName)

//! \brief 不允许默认构造函数（构造、拷贝、赋值）
//! \use 建议用在类中只有静态方法时，且可以放在 public or private 中
#define GLIB_DISALLOW_IMPLICIT_CONSTRUCTORS_PUBLIC(TypeName) \
    TypeName(); \
    GLIB_DISALLOW_COPY_AND_ASSIGN_PUBLIC(TypeName)


//! \brief 隐式默认构造函数
#define GLIB_IMPLICIT_CONSTRUCTORS_PUBLIC(TypeName) \
    TypeName() = default; \
    TypeName(const TypeName& others) = default; \
    TypeName& operator=(const TypeName& other) = default

#endif // GLIB_PUBLIC_INTERNAL_MACROS_H_
