/*************************************************************************************
 * Description  : 用于初始化空间构造元素
 * Version      : 1.0
 * Author       : koritafei(koritafei@gmailcom)
 * Date         : 2021-04-21 17:20:40
 * LastEditors  : koritafei(koritafei@gmailcom)
 * LastEditTime : 2021-04-23 14:46:23
 * FilePath     : /STLLearn/src/STL/uninitialized.h
 * Copyright (C) 2021 koritafei(koritafei@gmailcom). All rights reserved.
 *************************************************************************************/

#ifndef __UNINITIALIZED_H__
#define __UNINITIALIZED_H__

#include <type_traits>

#include "algobase.h"
#include "construct.h"
#include "iterator.h"
#include "type_traits.h"
#include "util.h"

namespace Mystl {

/**
 * @brief 把 [first, last) 上的内容复制到以 result
 * 为起始处的空间，返回复制结束的位置
 * @tparam InputIter
 * @tparam ForwardIter
 * @param  first            My Pan doc
 * @param  last             My Pan doc
 * @param  result           My Pan doc
 * @return ForwardIter
 * */
template <class InputIter, class ForwardIter>
ForwardIter unchecked_uninit_copy(InputIter   first,
                                  InputIter   last,
                                  ForwardIter result,
                                  std::true_type) {
  return Mystl::copy(first, last, result);
}

template <class InputIter, class ForwardIter>
ForwardIter unchecked_uninit_copy(InputIter   first,
                                  InputIter   last,
                                  ForwardIter result,
                                  std::false_type) {
  auto cur = result;
  try {
    for (; first != last; ++first, ++cur) {
      Mystl::construct(&*cur, *first);
    }
  } catch (...) {
    for (; result != cur; ++result) {
      Mystl::destroy(&*result);
    }
  }
  return cur;
}

template <class InputIter, class ForwardIter>
ForwardIter unchecked_uninit_copy(InputIter   first,
                                  InputIter   last,
                                  ForwardIter result) {
  return Mystl::unchecked_uninit_copy(
      first,
      last,
      result,
      std::is_trivially_copy_assignable<
          typename iterator_traits<ForwardIter>::value_type>{});
}

/**
 * @brief  把[first,
 * first+n)上的内容复制到result为起始的空间，返回复制结束的位置
 * @tparam InputIter
 * @tparam Size
 * @tparam ForwardIter
 * @param  first            My Pan doc
 * @param  n                My Pan doc
 * @param  result           My Pan doc
 * @return ForwardIter
 * */
template <class InputIter, class Size, class ForwardIter>
ForwardIter unchecked_uninit_copy_n(InputIter   first,
                                    Size        n,
                                    ForwardIter result,
                                    std::true_type) {
  return Mystl::copy_n(first, n, result).second;
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter unchecked_uninit_copy_n(InputIter   first,
                                    Size        n,
                                    ForwardIter result,
                                    std::false_type) {
  auto cur = result;
  try {
    for (; n > 0; --n, ++cur, ++first) {
      Mystl::construct(&*cur, *first);
    }
  } catch (...) {
    for (; result != cur; ++result) {
      Mystl::destroy(&*result);
    }
  }

  return cur;
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter uninitialized_copy_n(InputIter first, Size n, ForwardIter result) {
  return Mystl::unchecked_uninit_copy_n(
      first,
      n,
      result,
      std::is_trivially_copy_assignable<
          typename iterator_traits<InputIter>::value_type>{});
}

/**
 * @brief  在[first, last)区间填充元素
 * @tparam ForwardIter
 * @tparam T
 * @param  first            My Pan doc
 * @param  last             My Pan doc
 * @param  value            My Pan doc
 * */
template <class ForwardIter, class T>
void unchecked_uninit_fill(ForwardIter first,
                           ForwardIter last,
                           const T&    value,
                           std::true_type) {
  Mystl::fill(first, last, value);
}

template <class ForwardIter, class T>
void unchecked_uninit_fill(ForwardIter first,
                           ForwardIter last,
                           const T&    value,
                           std::false_type) {
  auto cur = first;
  try {
    for (; cur != last; ++cur) {
      Mystl::construct(&*cur, value);
    }
  } catch (...) {
    for (; first != cur; ++first) {
      Mystl::destroy(&*first);
    }
  }
}

template <class ForwardIter, class T>
void unchecked_uninit_fill(ForwardIter first,
                           ForwardIter last,
                           const T&    value) {
  Mystl::unchecked_uninit_fill(
      first,
      last,
      value,
      std::is_trivially_copy_assignable<
          typename iterator_traits<ForwardIter>::value_type>{});
}

/**
 * @brief 把[first, last)上的内容移动到以 result
 * 为起始处的空间，返回移动结束的位置
 * @tparam InputIterator
 * @tparam ForwardIter
 * @param  first            My Pan doc
 * @param  last             My Pan doc
 * @param  result           My Pan doc
 * @return ForwardIter
 * */
template <class InputIterator, class ForwardIter>
ForwardIter unchecked_uninit_move(InputIterator first,
                                  InputIterator last,
                                  ForwardIter   result,
                                  std::true_type) {
  return Mystl::move(first, last, result);
}

template <class InputIter, class ForwardIter>
ForwardIter unchecked_uninit_move(InputIter   first,
                                  InputIter   last,
                                  ForwardIter result,
                                  std::false_type) {
  ForwardIter cur = result;
  try {
    for (; first != last; ++first, ++cur) {
      Mystl::construct(&*cur, Mystl::move(*first));
    }
  } catch (...) {
    Mystl::destroy(result, cur);
  }
  return cur;
}

template <class InputIter, class ForwardIter>
ForwardIter uninitialized_move(InputIter   first,
                               InputIter   last,
                               ForwardIter result) {
  return Mystl::unchecked_uninit_move(
      first,
      last,
      result,
      std::is_trivially_move_assignable<
          typename iterator_traits<InputIter>::value_type>{});
}

/**
 * @brief 把[first, first + n)上的内容移动到以
 * result为起始处的空间，返回移动结束的位置
 * @tparam InputIter
 * @tparam Size
 * @tparam ForwardIter
 * @param  first            My Pan doc
 * @param  n                My Pan doc
 * @param  result           My Pan doc
 * @return ForwardIter
 * */
template <class InputIter, class Size, class ForwardIter>
ForwardIter unchecked_uninit_move_n(InputIter   first,
                                    Size        n,
                                    ForwardIter result,
                                    std::true_type) {
  return Mystl::move(first, first + n, result);
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter unchecked_uninit_move_n(InputIter   first,
                                    Size        n,
                                    ForwardIter result,
                                    std::false_type) {
  auto cur = result;
  try {
    for (; n > 0; --n, ++first, ++cur) {
      Mystl::construct(&*cur, Mystl::move(&*first));
    }
  } catch (...) {
    for (; result != cur; ++result)
      Mystl::destroy(&*result);
    throw;
  }

  return cur;
}

template <class InputIter, class Size, class ForwardIter>
ForwardIter unchecked_uninit_move_n(InputIter   first,
                                    Size        n,
                                    ForwardIter result) {
  return Mystl::unchecked_uninit_move_n(
      first,
      n,
      result,
      std::is_trivially_move_assignable<
          typename iterator_traits<InputIter>::value_type>{});
}
}  // namespace Mystl

#endif /* __UNINITIALIZED_H__ */