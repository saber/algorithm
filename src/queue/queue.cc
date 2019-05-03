/*
 * CopyRight (c) 2019 gcj
 * File: queue.cc
 * Project: algorithm
 * Author: gcj
 * Date: 2019/4/15
 * Description: use queue
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 * reference 网上
 */

//! \brief 一个网上下载的测试队列函数，把自己实现的队列对应替换为标准库中的队列。经过测试能够满足
//!         网上提供的代码
//! \Note 该测试文件已经被 use_queue.cc 测试文件替换

#include "./queue.hpp"
#include <iostream>
// #include <iostream>
// #include <Queue>
#include <string>
using namespace std;
using namespace glib;
void test_empty()
{
  Queue<int> myQueue;
  int sum (0);

  for (int i=1;i<=12;i++) {
      myQueue.Push(i);
  }
  while (!myQueue.empty())
  {
     sum += myQueue.front();
     myQueue.Pop();
  }
    cout << "total: " << sum << endl;
    // cout << "空: " << myQueue.front() << endl;
}//运行结果： total： 55
void test_Pop()
{
  Queue<int> myQueue;
  int myint;

  cout << "\nPlease enter some integers (enter 0 to end):\n";

  do
  {
    cin >> myint;
    myQueue.Push (myint);
  } while (myint);

  cout << "myQueue contains: " << myQueue.size();
  while (!myQueue.empty())
  {
    cout << " " << myQueue.front();
    myQueue.Pop();
  }
}
/********
运行结果：
Please enter some integers (enter 0 to end):
512
605
420
517
532
0
myQueue contains:  512 605 420 517 532 0
********/
void test_size()
{
  Queue<int> myints;
  cout << "0. size: " << (int) myints.size() << endl;

  for (int i=0; i<5; i++) myints.Push(i);
  cout << "1. size: " << (int) myints.size() << endl;

  myints.Pop();
  cout << "2. size: " << (int) myints.size() << endl;
}
/****
运行结果：
0. size: 0
1. size: 5
2. size: 4
****/
int main()
{
    test_empty();
    cout<<"\n***********************************************\n";
    test_size();
    cout<<"\n***********************************************\n";
    test_Pop();
    cout<<"\n***********************************************\n";
    Queue<string> q;

    // insert three elements into the Queue
    q.Push("These ");
    q.Push("are ");
    q.Push("more than ");
    //cout << "number of elements in the Queue: " << q.size()<< endl;

    // read and print two elements from the Queue
    cout << q.front();
    q.Pop();
    cout << q.front();
    q.Pop();
    //cout << "number of elements in the Queue: " << q.size()<< endl;

    // insert two new elements
    q.Push("four ");
    q.Push("words!");
    //cout << "\nnumber of elements in the Queue: " << q.size()<< endl;
    // skip one element
    q.Pop();

    // read and print two elements
    cout << q.front();
    q.Pop();
    cout << q.front() << endl;
    q.Pop();

    // print number of elements in the Queue
    cout << "number of elements in the Queue: " << q.size()<< endl;
}
/*******
*运行结果：
total: 55
***********************************************
0. size: 0
1. size: 5
2. size: 4
***********************************************
Please enter some integers (enter 0 to end):
512
605
420
517
532
0
myQueue contains:  512 605 420 517 532 0
***********************************************
These are four words!
number of elements in the Queue: 0
Process returned 0 (0x0)   execution time : 33.512 s
Press any key to continue.
********/
