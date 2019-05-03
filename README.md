<div align = "center"> <img src="./pics/logo2.png" width = "400px"></div><br>

> 记录学习《数据结构与算法之美》专栏的实现代码（c++） & 笔记。

[![](https://img.shields.io/github/issues/saber/algorithm.svg)](https://github.com/saber/algorithm/issues) [![](http://img.shields.io/github/forks/saber/algorithm.svg)](https://github.com/saber/algorithm/network) [![](http://img.shields.io/github/starts/saber/algorithm.svg)](http://img.shields.io/github/saber/algorithm/stargazers) [![](https://travis-ci.org/saber/algorithm.svg?branch=master)](https://travis-ci.org/saber/algorithm) [![](https://img.shields.io/github/release/saber/algorithm.svg)](https://github.com/saber/algorithm/releases)[![jaywcjlove/sb](https://jaywcjlove.github.io/sb/lang/chinese.svg)](README.md)

---



## :file_folder: src

专栏中讲过的数据结构与算法的 C++ 实现。内部包含了详细的代码注释、复杂度分析以及基本实现思路等等。下面图片是其中一个单链表 `.hpp` 注释例子。

**戳这里** :point_right: :point_right: [代码](src/) ，查看所有代码。代码持续更新中 ......

<img src = "./pics/注释格式.png" width = "400px">



## :notebook: notes

为方便阅读学习过程中的笔记，笔记已经按照中文排版格式进行排版。排版格式参照 [中文文案排版指北](https://github.com/sparanoid/chinese-copywriting-guidelines)。

阅读笔记，**戳这里**​ :point_right: :point_right: [笔记](notes/)。笔记持续更新中 ......



## :pencil2: awesome-resources.md

学习过程中收集的关于数据结构与算法的资源，以及大佬经验。

**戳这里** :point_right: :point_right: [awesome-resources.md](awesome-resources.md)。资源持续更新中 ......



## :file_folder: code_practice

一些互联网公司在线测试题目。持续更新中 ......



## :memo: 后记

### Repo 总览

```
├── awesome-resources.md
├── code_practice
│   ├── huawei.cc
│   └── string_encode.cc
├── LICENSE.txt
├── README.md
└── src
    ├── array
    │   ├── array.hpp
    │   └── use_array.cc
    ├── list
    │   ├── single_list.hpp
    │   └── use_single_list.cc
    ├── stack
    │   ├── stack.hpp
    │   └── use_stack.cc
    └── utils
        └── string_common.hpp
```

### QQ 群

专栏推荐的课本资源大概 1GB，因为 Git 上传限制以及 CSDN 同时不能自己设置 0 积分。如果想获取该资源，可以申请加入下面的群。或者直接到 CSDN 平台下载（[资源链接1](https://download.csdn.net/download/geself/11152111) ，[资源链接2](https://download.csdn.net/download/geself/11152094)，[资源链接3](https://download.csdn.net/download/geself/11152117)），不过积分是 CSDN 平台算法制定的，没法自己进行设置 0 积分。当然免费的可以到 qq 群进行下载，这也为大家提供一个学习交流平台，在这里你可以自由地讨论技术问题。

**Note**：为了保证群内不出现微商等闲杂人员，申请加群时，要备注成 saber/algorithm 等字眼，方便筛选。

<img src="pics/qq.png" width="200px">



### :black_nib: 计划

---

本 Repo 中， `src`文件夹包含的代码实现，基本上是按照如下列表顺序进行更新。除了下面一些数据结构与算法，还会包含一些专栏中涉及到的高级算法实现。

##### 数组
* 实现一个支持动态扩容的数组
* 实现一个大小固定的有序数组，支持动态增删改操作
* 实现两个有序数组合并为一个有序数组

##### 链表
* 实现单链表、循环链表、双向链表，支持增删操作
* 实现单链表反转
* 实现两个有序的链表合并为一个有序链表
* 实现求链表的中间结点

##### 栈
* 用数组实现一个顺序栈
* 用链表实现一个链式栈
* 编程模拟实现一个浏览器的前进、后退功能

##### 队列
* 用数组实现一个顺序队列
* 用链表实现一个链式队列
* 实现一个循环队列

##### 递归
* 编程实现斐波那契数列求值f(n)=f(n-1)+f(n-2)
* 编程实现求阶乘n!
* 编程实现一组数据集合的全排列

##### 排序
* 实现归并排序、快速排序、插入排序、冒泡排序、选择排序
* 编程实现O(n)时间复杂度内找到一组数据的第K大元素

##### 二分查找
* 实现一个有序数组的二分查找算法
* 实现模糊二分查找算法（比如大于等于给定值的第一个元素）

##### 散列表
* 实现一个基于链表法解决冲突问题的散列表
* 实现一个LRU缓存淘汰算法

##### 字符串
* 实现一个字符集，只包含a～z这26个英文字母的Trie树
* 实现朴素的字符串匹配算法

##### 二叉树
* 实现一个二叉查找树，并且支持插入、删除、查找操作
* 实现查找二叉查找树中某个节点的后继、前驱节点
* 实现二叉树前、中、后序以及按层遍历

##### 堆
* 实现一个小顶堆、大顶堆、优先级队列
* 实现堆排序
* 利用优先级队列合并K个有序数组
* 求一组动态数据集合的最大Top K

##### 图
* 实现有向图、无向图、有权图、无权图的邻接矩阵和邻接表表示方法
* 实现图的深度优先搜索、广度优先搜索
* 实现Dijkstra算法、A*算法
* 实现拓扑排序的Kahn算法、DFS算法

##### 回溯
* 利用回溯算法求解八皇后问题
* 利用回溯算法求解0-1背包问题

##### 分治
* 利用分治算法求一组数据的逆序对个数

##### 动态规划
* 0-1背包问题
* 最小路径和
* 编程实现莱文斯坦最短编辑距离
* 编程实现查找两个字符串的最长公共子序列
* 编程实现一个数据序列的最长递增子序列