/*
 * CopyRight (c) 2019 gcj			      // 在后面添加上你的署名
 * File: template.hpp 			          // 该算法对应的名字，以小写+下划线形式命名
 * Project: algorithm
 * Author: gcj 							  // 在后面添加你自己的署名
 * Date: 2019/4/8 						  // 修改成当前文件创作完成时的日期
 * Description: a template                // 简单描述文件包含的算法
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#ifndef GLIB_TEMPLATE_HPP_      // 将 TEMPLATE 替换为当前文件的名字
#define GLIB_TEMPLATE_HPP_

#include <iostream>             // 一些标准库
#include <vector>
#include <assert.h>
#include "ohter_type.h"         // 你自己定义的

// 下面对应内容需要根据贡献指导文档 CONTRIBUTION.md 的介绍自行替换。
// 以及参照代码相应的规范文档 CODING_GUIDELINES.md 调整下面的示例。

//! \brief 描述该文件对应的功能
//!        基本功能：
//!           1）支持动态增加内存从后插入元素、下标访问功能[]、合并两个有序数组（仅支持小到大内置类型）
//!                 对应函数：PushBack、operator[]、Merge
//!
//!           2）其他普通函数：返回数组有效字节数、数组总容量、排序函数
//!                 对应函数：size、capacity、Sort
//!
//!           3）内部函数：比较两个元素大小
//!                 对应函数：Compare
//!
//! \Note
//!      1）该数组不支持，赋值、移动赋值、移动拷贝功能，仅仅做了一个动态数组的简单实现
//!
//! \TODO
//!      1）支持移动赋值、移动构造功能
//!      2）实现 LRU 缓存淘汰策略——在单链表中已经实现了
//!      3）实现一个大小固定的有序数组，支持动态增删改操作——实际上考核插入时的数据移动、删除时的垃圾回收策略
//!      4）leetCode 题目待查找
//!
//! \platform
//!      ubuntu16.04 g++ version 5.4.0
//!
//! \TODO Reference
//!
//! \conclusion
//!

namespace glib {                       // 为了保证整个项目的一致，这个命名空间不变，仍然使用 glib
using namespace std;
...... 其他一些声明

#define MY_MACRO_THAT_SCARES;          // 你自己定义宏处，要全大写加下划线（但是最好不要使用宏）
const int GlibGlobalVariable;          // 如果你想定义命名空间的全局变量，此时该变量作为 glib 命名空间的一个接口，
                                       // 此时按照函数命名规则，首字母大写
static int GlibGlobalVariable;         // 当然，最好不要定义一个命名空间内的静态存储期属性变量/常量

template <typename T>                  // 如果有必要，可以做一些模板类的前置声明
class Template;

// 简单函数直接 「// + 注释」，或者 「\brief+注释」，
// 复杂函数可以按照下面 CoreFunction 上面的注释说明

// 该函数的功能
//! \brief 该函数的功能
void FuncTo2() {                       // 一个全局非内联函数的写法，供外部调用
    static int temp_variable;          // 函数内部静态变量，命名规则是小写加下划线，与普通变量命名一致
}

inline void FuncTo() {                 // 一个全局的内联函数的写法

}

namespace template_internal {          // 如果想定义一些当前文件内部使用的函数
                                       // 可以使用 internal_文件名，来定义命名空间
                                       //此时默认规定，当前函数只能用在当前文件中使用
    const static int InternalVariable;
    static int InternalVariable;

    // 定义一些当前文件内部需要使用的函数
    // 改造后的内联函数
    inline void FuncTodo() {

    }

} // namespace template_internal

enum class TemplateEnum {              // 如果使用了枚举，那么按照这个格式，内部元素是大写，下划线连接
    PRE_ORDER,
    IN_ORDER,
    POST_ORDER
}；


struct UrlTableProperties {            // 如果使用了结构体，最后没有下划线，全部都是小写，下划线连接单词
    const int table_name = 5;
    int table_name;
    static int table_name;
};

// 可以在这里做一下当前类的简单介绍
template <typename T, typename _Key, typename _Value, typename _Hash = std::hash<_Key>>
class Template {                       // 核心算法的开发，需要替换为当前实现算法的名字
public: // 类型声明
    // 一系列的类型的声明处
    using KeyType    = _Key;
    using MappedType = _Value;
    using Hasher     = _Hash;
    template <_Scalar>
    using VectorTemplate = std::vector<_Scalar>;
    typedef std::vector<int> VectorInt;

public: // 构造函数相关
    Template() : template_const_member_(0), template_member_(2) { cout << "默认构造函数" << endl; }
    explicit Template(int template_member) : template_member_(template_member) {}
    Template(int template_const_member, int template_member)
        : template_member_(template_member), template_const_member_(template_const_member) {
        // 内部其他代码
        type_ptr_ = new T[10]();
    }
    Template(const Template &other)
        : template_member_(other.template_member_),
          template_const_member_(other.template_const_member_) {
        // 内部其他代码
    }
    // 可选的移动拷贝
    Template(Template &&other) : template_member_(other.template_member_)
                                 template_const_member_(other.template_const_member_) {
        other.template_member_       = 0;
        other.template_const_member_ = 0;
    }
    Template& operator=(const Template &other) {
        if (&other != this) {
            template_member_       = other.template_member_;
            type_ptr_ = new T[10]();
            copy(type_ptr_, other.type_ptr_); // 做变量的拷贝
        }
        return *this;
    }

    Template& operator=(Template &&other) {                      // 可选的移动赋值
        if (&other != this) {
            template_member_       = other.template_member_;
            type_ptr_              = other.type_ptr_;
            other.template_member_ = 0;
            other.type_ptr_ = nullptr;
            // 一些其他操作
        }
        return *this;
    }

    // 有默认拷贝、没有拷贝、有赋值、没有默认赋值
    Template& operator=(const Template &other) = default;
    Template& operator=(const Template &other) = delete;
    Template(const Template &other) = default;
    Template(const Template &other) = delete;


    ~Template() {                       // 析构函数，如果内部有申请内存操作，那么可以按照这种方式释放内存
        // 做一些释放资源的事
        if (nullptr != type_ptr_) {
            delete type_ptr_;
            // ....
        }
    }

public: // 外部调用核心函数

    // 正式的成员函数

    // 取值、设值、状态函数，比较短小的，可以按照如下方式进行书写
    void set_template_member(int template_member) { template_member_ = template_member; }
    int  get_template_member() const              { return template_member_;            }
    bool is_empty() const                         { return is_true;                     }

    // 形式参数的命名规则，以及形参变量是引用还是拷贝。需要看是否是内置类型或者根据实际需求来定
    // 形式参数太长要分行
    void FindTemplate(const T &input_member,
                      const string &strings,
                      int temp_variable) {
        // 二元操作符号，可以是下面其中一种
        v = w * x + y / z;
        v = w*x + y/z;
        v = w * (x + z);
    }


    // 假设下面的函数是算法的核心函数，那么需要按照下面格式书写注释。前三个必选，后面可选

    //! \brief 简单描述当前函数的功能
    //! \note 当前函数使用过程中需要的注意事项
    //! \complexity 时间复杂度（如果有最好、最坏、平均复杂度要分别说明），空间复杂度（如果是 O(1)，可以默认不写）
    //! \param example_one 该变量有什么功能
    //! \return 返回值有什么含义
    //! \method 用了什么技巧方法
    //! \description 解决的实际问题的简单描述
    //! \reference 参考的资料代码
    //! \TODO 还有那些不足
    //!      1）xx
    //!      2）xxx
    //! \example 如果当前函数使用复杂，可以举例子
    //!         int example_one = 2;
    //!         CoreFunction(example_one);
    int CoreFunction(int example_one) {
        bool flag = false;
        // ....
        // if (xxx) flag = ture;
        return flag;
    }
private: // 内部辅助函数 helper function

public: // 外部调用成员变量
    int TemplateMember;                  // 如果定义了外部可获取的共有成员变量，那么需要与函数命名规则一致，
                                         // 首字母大写且无下划线连接
    const int TableName = 5;

private: // 内部成员变量
    // 私有变量直接是小写单词，单词之间用下划线连接，最后是下划线结尾，
    // 当然为了美观，可以进行必要的对齐。也可以不对齐，但是自己的风格要一致。建议对齐！
    //constexpr int template_const_member_ = 0;
    const int template_const_member_ = 0;
    bool      is_ture_               = false;
    int       template_member_;
    T*        type_ptr_;
}; // class Template

// 如果你有一些当前算法的应用，可以定义一个「文件名_app」的命名空间
// 表示外部可以实际来访问。比如单链表可以有 LRU 算法，那么下面就可以写应用算法
namespace template_app {

//! \brief 单链表应用之 LRU 缓存淘汰算法实现
......其他一些描述
template <typename T>
void LRUBySingleList() {

}

} // namespace template_app
} // namespace glib

#endif // GLIB_TEMPLATE_HPP_
