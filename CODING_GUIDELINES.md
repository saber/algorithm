<!--ts-->

## :memo: C++ 代码规范

简单描述一下项目代码中用到的 C++ 代码规范。基本上是按照 [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html) 进行调整的。中文版本 [点击这里](https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/)。

**当前规范文档会持续更新 ......**</br></br>



### 目录

---

  * [1 命名](#1-命名)
    * [1）一般命名规则](#1一般命名规则)
    * [2）文件命名](#2文件命名)
    * [3）类型命名](#3类型命名)
    * [4）变量&amp;常量命名](#4变量常量命名)
    * [5）函数命名](#5函数命名)
    * [6）枚举命名](#6枚举命名)
    * [7）宏命名](#7宏命名)
    * [8）命名空间命名](#8命名空间命名)
 * [2 格式](#2-格式)
    * [1）空格 or 制表符（tab）](#1空格-or-制表符tab)
    * [2）水平空格](#2水平空格)
 * [3 函数设置](#3-函数设置)
    * [1）参数传递](#1参数传递)
    * [2）其他注意](#2其他注意)</br></br>



### 1 命名

---

#### 1）一般命名规则

- 名字要有一些含义
- 不能使用一些缩写，除非大家都知道该缩写是什么意思。比如 `T` 是模板参数，都是知道的。还有一个单词不能删除其中内部的几个字母。

**Good**

```c++
int price_count_reader;    // 不使用缩写。
int num_errors;            // num 大家都知道含义
int num_dns_connections;   // 都知道 dns 代表什么
```

**Bad**

```c++
int n;                     // 无意义
int nerr;                  // 缩写有歧义
int n_comp_conns;          // 缩写有歧义
int wgc_connections;       // wgc 只有几个人知道什么意思
int pc_reader;             // 除了个人电脑可以用 pc 外。其他东西也可以用其简写
int cstmr_id;              // 删除了单词其中的几个字母
```



#### 2）文件命名

- 文件和目录名字应该是全小写，单词与单词之间，要加下划线 `_` 进行连接。
- 所有测试文件以 `.test.cc` 作为拓展名。头文件（包含具体实现）以 `.hpp` 结尾。头文件仅仅有类型/函数/变量声明，要以 `.h` 作为拓展后缀。其他的源文件同样要以 `.cc` 作为拓展名。

**例子**

- 测试源文件：`single_list.test.cc`
- 一般源文件：`use_map.cc`
- 头文件（含有实现）：`single_list.hpp`
- 头文件（不含实现）：`types.h`
- 目录：`list`、`stl_examples`

 

#### 3）类型命名

所有类型{类、结构体、类型别名、枚举}都有相同的命名规则。都是单词的首字母要大写并且没有下划线。

```c++
// classes and structs
class UrlTable { ... };
class UrlTableTester { ... };
struct UrlTableProperties { ... };

// typedefs
typedef hash_map<UrlTableProperties *, string> PropertiesMap;

// using aliases
using PropertiesMap = hash_map<UrlTableProperties *, string>;

// enums
enum UrlTableErrors { GoogleName };
```



#### 4）变量&常量命名

- 一般变量的名字，都是要小写，且下划线链接。对于全局静态存储期变量直接单词首字母大写，且单词之间不需要下划线连接。外部看来相当于一个函数的调用。
- 前缀含有 `const` `constexpr` 的全局常量。并且其生存期是静态存储期。那么名字必须是全部大写且前面加上一个 `k` 标志。方便从名字中推断出，其是静态存储期的常量。

```c++
// 普通临时变量，普通函数形式参数，结构体成员变量等等
int table_name;
const int table_name;
static int talbe_name;

// 静态全局/非静态全局
static int TableName;
int TableName; // 假设是一个全局变量

// 静态全局/非静态全局常量
const int kTableName = 5; // 全局常量
constexpr int kTableName = 5; 
const static int kTableName = 5; // 全局常量/局部常量
```

- 类内部成员变量&常量

  private 变量，全部是小写单词加下划线连接，且末尾有 `_` 下划线。public 变量，全部是大写字母且没有下划线链接。

  ```c++
  class xx {
  	public:
  		const int TableName = 5; // 为了让外部看起来更像是一个接口函数
  		static int TableName;
  	
    private:
   		int table_name_;
      const int table_name_;
      constexpr int table_name_ = 8;
  };
  ```

- 结构体内部变量&常量

  不管什么情况全部都是小写，注意没有后缀 `_`。

  ```c++
  struct xxx {
    const int table_name = 5;
    int table_name;
    static int table_name;
  };
  ```
  



#### 5）函数命名

函数命名与静态存储期的变量命名规则一致，首字母大写，无下划线连接。

```c++
AddTableEntry()
DeleteUrl()
OpenFileOrDie()
```

需要注意的是，对于成员函数来说，特殊情况是，该成员函数是取值函数、设值函数、返回当前的某个状态。必须要按照普通变量的命名规则。就是全部小写加下划线连接。对于取值设值函数，函数名字最好与其对应的成员名字对应起来。

```c++
class Foo {
    public:
  	    
        Foo(int b) : Bar(), baz_(b) {}   
  			// 设值函数与 baz_ 成员变量对应起来
        void set_baz(int baz) { baz_ = baz; }
  			// 状态函数
  		  bool empty() { return is_empty; }
  			// 与 count_ 对应起来
  			int get_count() { return count_; }
    private:
  			int baz_;
  		  bool is_empty;
  		  int count_;
}
```

 

#### 6）枚举命名

枚举值的命名是首字母全大写，且单词之间下划线连接。

例子

```c++
enum UrlTableErrors {
    OK,
    OUT_OF_MEMORY,
    MALFORMED_INPUT
};

enum TraversalOrder: short {
    PRE_ORDER,
    IN_ORDER,
    POST_ORDER
};
```

 

#### 7）宏命名

建议一般少用宏，但是如果用了宏，那么单词全部大写，且单词之前用下划线分隔。

```c++
#define MY_MACRO_THAT_SCARES;
```

 

#### 8）命名空间命名

命名空间都是小写字母，顶层命名空间一般是项目的名字或者是某个团队负责的某个部分定义的命名空间，避免与标准库或者知名的顶层命名空间冲突。比如 std 命名空间，自己不要使用。对于子目录中有多个文件，在这些文件中除了加上顶层的命名空间外，还需要加上以目录名作为命名空间名。尽量不要使用 `internal` 这种命名空间，如果用到的话，可以前面加上文件的名字 `stack_internal` 这种命名空间即可消除潜在的冲突。

</br></br>

### 2 格式

---

#### 1）空格 or 制表符（tab）

使用 soft tabs ，代表 4 个空格。在自己的编译器或者编辑器中进行设置。这个是仿照 python 代码的格式来设置的。比较清晰。



#### 2）水平空格

对于下面介绍的两个规则，在实际使用时，如果能够代码的排版更清晰，可以适当调整下面的空格规则。比如操作运算符的赋值操作哪里，在代码块中举的例子。就是为了代码的美观，适当牺牲了空格规则。但是整体上仍然是按照下面的要求进行书写代码。

- 一般规则

  - 每行的结尾没有空格

  - 分号前面应该没有空格

    ```c++
    int i = 0;
    ```

  - 在半开的花括号之前一定要有空格

    ```c++
    void f(bool b) {
      //...
    }
    ```

  - 花括号初始化列表内部的值与花括号之间可有可无空格。但是你自己的代码必须要一致

    ```c++
    int x[] = { 0 };
    int x[] = {0};
    ```

  - 类的集成和初始化列表的空格如下

    ```c++
    class Foo : public Bar {
        public:
            // For inline function implementations, put spaces between the braces
            // and the implementation itself.
            Foo(int b) : Bar(), baz_(b) {}  // No spaces inside empty braces.
            void Reset() { baz_ = 0; }      // Spaces separating braces from implementation.
            ...
    }
    ```

- 操作运算符

  - 赋值

    ```c++
    int x = 0;
    int num_black = x;
    // 当然对于上面这两行可以进行如下对齐，使得格式更清晰
    int x 			  = 0;
    int num_black = x;
    ```

  - 其他二元操作符与操作数之间一般会有空格，可以适当的没有空格。下面的的形式都可以，看着清晰即可

    ```c++
    v = w * x + y / z;
    v = w*x + y/z;
    v = w * (x + z);
    ```

  - 一元操作符之间不会有空格

    ```c++
    x = -5;
    ++y;
    z = !y; // or z = noty
    ```

</br></br>

### 3 函数设置

---

- 封装有意义的操作
- 保持函数体短小简洁。最好每个函数不要超过 40 行。
- 一个函数应该执行单一的逻辑操作。
- 如果函数太过短小，可以用 Lambda 表达式进行代替。



#### 1）参数传递

对于输入参数，按值传递一个内置类型的变量是比较少消耗资源的。除非我们在函数内部需要修改该值，那么此时我们才会用引用类型。对于类类型或者一个数组（占用空间比较大）变量，直接用 const 类型的引用即可。减少了拷贝的开销。

**Good**

```c++
void f1(const string& s);  // 引用的方式要比直接拷贝开销小

void f2(int x);            // 内置类型的拷贝资源消耗少于引用
// 如果是想在 f2 函数中修改这个变量 x，可以使用引用
void f2(int &x);
```

**Bad**

```c++
void f3(string s);         // 此时拷贝类，实际上开销大于直接上面的传递引用

void f4(const int& x);     // 开销大于上面的 f2(int x)
```

 

#### 2）其他注意

- 形式参数顺序，要按照先输入，后输出的顺序进行设置。函数内部不改变该值，那么需要在加上 `const ` 前缀，此时是否加上引用，要按照上面参数传递规则。

<!--te-->