/*
 * CopyRight (c) 2019 gcj
 * File: queue.hpp
 * Project: algorithm
 * Author: gcj
 * Date: 2019/4/11
 * Description: queue simple implementation
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

// 注释风格按照 // 只有在文件开头的说明使用 /**/，自己要求 // 后面有一个空格
// 利用项目源代码树的全路径。项目 kit 此时 kit/src/google/google_norm.h
#ifndef KIT_GOOGLE_GOOGLE_NORM_H_
#define KIT_GOOGLE_GOOGLE_NORM_H_
**留白**
// #include 头文件包含顺序：相关头文件（项目中的）、C 库、C++库、其他库.h,比如第三方 opencv、Eigen 等等库
// 该项目中的其他头文件,对于每个类的头文件需要按照字母顺序进行排序。
// 但是对于头文件来说，我们仅仅列出头文件中需要包含的一些库，然后按照上面的顺序进行排列，假设下面的头文件，在该文件内都使用了。
#include <sys/types.h> // C 库头文件

#include <vector> // C++ 头文件

#include <Eigen/Core>
#include <opencv2/core/core.hpp> // 其他第三方库

#include "kit/.../..." // 利用了项目内的其他头文件的某个类型或函数


// 鼓励在 .cc 文件内使用匿名命名空间或 static 声明（只在声明处，定义就不要了）。
namespace kit { // 小写字母，最高级命名空间取决于项目名称，不要使用缩写，但是大家都知道的缩写可以用。
namespace google { // 对应于 kit/google 文件夹
// 可以在这里加上 别名
using Uint8Color = std::array<uint8, 3>;

extern const int kGlobalVar; // 全局常量


// TODO 对临时的，短期的解决方案，但不完美的代码使用这个注释，或者写一些 "将来某一天要做的事情"即 未完成事。或明确的事项
// DEPRECATED 弃用注释，不是强制弃用，格式如下：

// DEPRECATED(写说明，帮助其他人修复其调用点) ，或者一般做法，可以将一个弃用函数改造成为一个内联函数，这一函数将调用新的接口。
void FuncTodo() {
    // pass
}
// 改造后的内联函数
inline FuncTodo() {
    // pass
    // 调用新的接口
    FuncOne();
}

// 函数调用时，尽可能易读性：比如： my_widget.Transform(x1, x2, x3, y1, y2, y3, y4, z1, z2, z3);可以改为：
// my_widget.Transform(x1, x2, x3,
//                     y1, y2, y3,
//                     z1, z2, z3);

// 属性和展开为属性的宏，写在函数声明或定义的最前面，即返回类型之前
// 宏的名字，注意全大写，然后下划线链接。
#define MUST_USE_RESULT // 起到提示作用，可以想着用


// 函数声明：注释函数功能，标注： 对于比较明显的就不需要说明了。
// 输入输出，
// 函数是否分配了必须由调用者释放的空间。
// 参数是否可以为空指针，或者在函数内部进行空指针检测，提示错误！用 glog CHECK（）宏。
void GoogleTest(); // 尽量使用这种在命名空间中的函数，不要在命名空间外部定义全局函数，而 static 函数要用在类中


// 类型名称的每个单词首字母均大写, 不包含下划线
using VectorInt = std::vector<int>;
typedef std::vector<int> VectorInt;
enum GoogleChoice { GLOG_CHOICE, GFLAGS_CHOICE }; // 枚举值，采用常量的命名方式。

// 内部变量命名规则与普通变量命名规则一致，小写加下划线链接，但是不需要结尾加下划线
// 自己规定为前置下划线 + 小写字母组合：恰好区分 class
struct GoogleSection {
    int data_member;    // 变量：不需要下划线结尾，当做普通变量处理，小写字母加下划线
    int data_2;
    static int struct_test; // 不用 k 开头，不用加下划线，因为是变量。这种基本上用不到
    const int data = 3;
};
// 列表初始化
GoogleSection cc{2,3};

// 函数代码比较少 10 行的函数声明为内联函数，且声明和定义都放在头文件中，否则编译失败。
// 对于包含 for()等循环 switch 的情况，最好不要用内联，除非，for()等循环 switch 用的少，或基本很少执行。
// 当然对于有些虽然声明为内联，但是编译器有时不会看做内联。比如递归函数、虚函数
inline void TestInline(int test_num) {
    int test_num_buf = test_num + 2;
    return; // 建议 void 返回格式
}
// 类注释：在类前面，描述类的功能
//和用法，比如 Example:...写下简单用法
//除非它的功能相当明显。一般情况下可以不说明，用分行 // 注释法
// 是否可多线程访问实例，需要说明。
class GoogleNorm {  // 对应文件名字 google_norm.h google_norm.cc
public:
    // 属于 static const constexpr 的数据成员要放在 public 后面，剩下其他变量全部是 private
    constexpr static const char* ConfigurationFileActionName = // k 开头 // 因为是 static 常量
      "intensity_to_color";
    static const MemberStaticConst = 3;

    GoogleNorm() = default;
    // 构造函数一般不注释，切记，读代码的人知道构造/析构函数的功能，需要注明函数对参数做了什么，是否取得指针所有权，析构清理了什么
    GoogleNorm(int i ) {}
    GoogleNorm(const GoogleNorm &other) : section_cout_(other.section_cout_) {
        int * temp = new (nothrow) int[other.ClassStatic]();
        // 拷贝元素
        temp
    }
    ~GoogleNorm() = default;
    //or
    // 析构函数，注意释放内存，以及变量对齐问题，显示整齐
    ~GoogleNorm() {
        if (nullptr != queue_) {
            delete[] queue_;
            queue_    = nullptr;
            capacity_ = 0;
            size_     = 0;
            head_     = 0;
            tail_     = 0;
        }
    }

    // 属性宏或展开为属性的宏，写在函数声明或定义的最前面。 表示提醒调用者
    MUST_USE_RESULT bool IsOk();

    void get_const(){ return ClassStatic_; } // 单函数内部要留白左右一个空格

    void get_section() const { return  section_cout_; } // 取值函数和设值函数要全小写加下划线。最好与实际成员变量对应

    // 声明：注释函数功能：对于明显的可以不用说明。比如上面取值和设值函数就不用加注释
    // 输入输出，
    // 函数是否分配了必须由调用者释放的空间。
    // 参数是否可以为空指针，或者在函数内部进行空指针检测，提示错误！用 glog CHECK（）宏。
    void MemberFunc(); // 普通函数 单词首字母大写！

protected:

private: // 类成员变量要加下划线，一律小写字母加下划线，结尾加下划线, 对于归属类的 static 变量，直接用函数命名方式，对于 static 常量，则用常量命名方式
    static int ClassStatic = 3;
    // 空格前置 风格一致
    char *c; // char &c;
    // 类作用域的常量，const constexpr 不要用 k 打头了，直接用函数的命名方式
    const int days_in_week_ = 8;
    // 数据成员注释，一般不用，但是对于变量可以接受 NULL or -1 这种值时，需要注释代表的含义
    int section_cout_; // 类的成员 变量（静态、非静态），下划线结尾
    int *p_; // 一段内存
};

}   // namespace google
}   // namespace kit
**留白一行**
#endif  //KIT_GOOGLE_GOOGLE_NORM_H_
