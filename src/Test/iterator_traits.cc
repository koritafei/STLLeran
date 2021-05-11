/**
 * @ Description  :
 * @ Version      : 1.0
 * @ Author       : koritafei(koritafei@gmail.com)
 * @ Date         : 2021-05-11 16:05:25
 * @ LastEditors  : koritafei(koritafei@gmail.com)
 * @ LastEditTime : 2021-05-11 16:22:09
 * @ FilePath     : /STLLearn/src/Test/iterator_tarits.cc
 * @ Copyright (C) 2021 koritafei(koritafei@gmail.com). All rights reserved.
 * */

#include <iostream>

template <class T>
struct MyIter {
  typedef T value_type;
  T *       ptr;
  MyIter(T *p = nullptr) : ptr(p) {
  }

  T &operator*() const {
    return *ptr;
  }
};

// class type
template <class T>
struct iterator_traits {
  typedef typename T::value_type value_type;
};

// 偏特化1
template <class T>
struct iterator_traits<T *> {
  typedef T value_type;
};

// 偏特化2
template <class T>
struct iterator_traits<const T *> {
  typedef T value_type;
};

/**
 * @brief 首先询问iteartor_traits<T>::value_type 特化版本进入特化版本处理；
 *        否则进入正常处理流程
 * @tparam T
 * @param  iter             My Pan doc
 * @return T::value_type
 * */
template <class T>
typename iterator_traits<T>::value_type func(T iter) {
  return *iter;
}

int main(int argc, char **argv) {
  MyIter<int> iter(new int(8));
  std::cout << func(iter) << std::endl;
  int *p = new int(2);
  std::cout << func(p) << std::endl;
  const int k = 10;
  std::cout << func(&k) << std::endl;
}
