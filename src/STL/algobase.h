/*************************************************************************************
 * Description  : stl 基础算法
 * Version      : 1.0
 * Author       : koritafei(koritafei@gmailcom)
 * Date         : 2021-04-21 17:20:24
 * LastEditors  : koritafei(koritafei@gmail.com)
 * LastEditTime : 2021-05-06 11:24:24
 * FilePath     : /STLLearn/src/STL/algobase.h
 * Copyright (C) 2021 koritafei(koritafei@gmailcom). All rights reserved.
 *************************************************************************************/

#ifndef __ALGO_BASE_H__
#define __ALGO_BASE_H__

#include <cstring>
#include <type_traits>

#include "iterator.h"
#include "util.h"

namespace Mystl {

#ifdef max
#pragma message("#undefing marco max")
#undef max
#endif  // max

#ifdef min
#pragma message("#undefing marco min")
#undef min
#endif  // min

/**
 * @brief max取最大值
 * @tparam T
 * @param  lhs              My Pan doc
 * @param  rhs              My Pan doc
 * @return const T&
 * */
template <class T>
const T& max(const T& lhs, const T& rhs) {
  return lhs < rhs ? rhs : lhs;
}

template <class T, class Compare>
const T& max(const T& lhs, const T& rhs, Compare comp) {
  return comp(lhs, rhs) ? rhs : lhs;
}

/**
 * @brief min求两者之间的最小值
 * @tparam T
 * @param  lhs              My Pan doc
 * @param  rhs              My Pan doc
 * @return const T&
 * */
template <class T>
const T& min(const T& lhs, const T& rhs) {
  return rhs < lhs ? rhs : lhs;
}

template <class T, class Compare>
const T& min(const T& lhs, const T& rhs, Compare comp) {
  return comp(rhs, lhs) ? rhs : lhs;
}

template <class FIter1, class FIter2>
void iter_swap(FIter1 lhs, FIter2 rhs) {
  Mystl::swap(lhs, rhs);
}

/**
 * @brief copy 将[first, last)区间内的元素拷贝到[result, result+(last -
 * fitst))内
 * @tparam InputIter
 * @tparam OutputIter
 * @param  first            My Pan doc
 * @param  last             My Pan doc
 * @param  result           My Pan doc
 * @return OutputIter
 * */
template <class InputIter, class OutputIter>
OutputIter unchecked_copy_cat(InputIter  first,
                              InputIter  last,
                              OutputIter result,
                              Mystl::input_iterator_tag) {
  for (; first != last; ++first, ++result) {
    *result = *first;
  }

  return result;
}

// ramdom_access_iterator_tag 版本
template <class RandomIter, class OutputIter>
OutputIter unchecked_copy_cat(RandomIter first,
                              RandomIter last,
                              OutputIter result,
                              Mystl::random_access_iterator_tag) {
  for (auto n = last - first; n > 0; --n, ++first, ++result) {
    *result = *first;
  }
  return result;
}

template <class InputIter, class OutputIter>
OutputIter unchecked_copy(InputIter first, InputIter last, OutputIter result) {
  return unchecked_copy_cat(first, last, result, iterator_category(first));
}

// 为 trivially_copy_assignable 类型提供特化版本
template <class Tp, class Up>
typename std::enable_if<
    std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
        std::is_trivially_copy_assignable<Up>::value,
    Up*>::type
unchecked_copy(Tp* first, Tp* last, Up* result) {
  const auto n = static_cast<size_t>(last - first);
  if (n != 0) std::memmove(result, first, n * sizeof(Up));
  return result + n;
}

template <class InputIter, class OutputIter>
OutputIter copy(InputIter first, InputIter last, OutputIter result) {
  return unchecked_copy(first, last, result);
}

/*****************************************************************************************/
// copy_backward
// 将 [first, last)区间内的元素拷贝到 [result - (last - first), result)内
/*****************************************************************************************/
// unchecked_copy_backward_cat 的 bidirectional_iterator_tag 版本
template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2 unchecked_copy_backward_cat(
    BidirectionalIter1 first,
    BidirectionalIter1 last,
    BidirectionalIter2 result,
    mystl::bidirectional_iterator_tag) {
  while (first != last)
    *--result = *--last;
  return result;
}

// unchecked_copy_backward_cat 的 random_access_iterator_tag 版本
template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2 unchecked_copy_backward_cat(
    BidirectionalIter1 first,
    BidirectionalIter1 last,
    BidirectionalIter2 result,
    mystl::random_access_iterator_tag) {
  for (auto n = last - first; n > 0; --n)
    *--result = *--last;
  return result;
}

template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2 unchecked_copy_backward(BidirectionalIter1 first,
                                           BidirectionalIter1 last,
                                           BidirectionalIter2 result) {
  return unchecked_copy_backward_cat(first,
                                     last,
                                     result,
                                     iterator_category(first));
}

// 为 trivially_copy_assignable 类型提供特化版本
template <class Tp, class Up>
typename std::enable_if<
    std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
        std::is_trivially_copy_assignable<Up>::value,
    Up*>::type
unchecked_copy_backward(Tp* first, Tp* last, Up* result) {
  const auto n = static_cast<size_t>(last - first);
  if (n != 0) {
    result -= n;
    std::memmove(result, first, n * sizeof(Up));
  }
  return result;
}

template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2 copy_backward(BidirectionalIter1 first,
                                 BidirectionalIter1 last,
                                 BidirectionalIter2 result) {
  return unchecked_copy_backward(first, last, result);
}

/*****************************************************************************************/
// copy_if
// 把[first, last)内满足一元操作 unary_pred 的元素拷贝到以 result 为起始的位置上
/*****************************************************************************************/
template <class InputIter, class OutputIter, class UnaryPredicate>
OutputIter copy_if(InputIter      first,
                   InputIter      last,
                   OutputIter     result,
                   UnaryPredicate unary_pred) {
  for (; first != last; ++first) {
    if (unary_pred(*first)) *result++ = *first;
  }
  return result;
}

/*****************************************************************************************/
// copy_n
// 把 [first, first + n)区间上的元素拷贝到 [result, result + n)上
// 返回一个 pair 分别指向拷贝结束的尾部
/*****************************************************************************************/
template <class InputIter, class Size, class OutputIter>
Mystl::pair<InputIter, OutputIter> unchecked_copy_n(InputIter  first,
                                                    Size       n,
                                                    OutputIter result,
                                                    Mystl::input_iterator_tag) {
  for (; n > 0; --n, ++first, ++result) {
    *result = *first;
  }
  return Mystl::pair<InputIter, OutputIter>(first, result);
}

template <class RandomIter, class Size, class OutputIter>
Mystl::pair<RandomIter, OutputIter> unchecked_copy_n(
    RandomIter first,
    Size       n,
    OutputIter result,
    Mystl::random_access_iterator_tag) {
  auto last = first + n;
  return Mystl::pair<RandomIter, OutputIter>(last,
                                             Mystl::copy(first, last, result));
}

template <class InputIter, class Size, class OutputIter>
Mystl::pair<InputIter, OutputIter> copy_n(InputIter  first,
                                          Size       n,
                                          OutputIter result) {
  return unchecked_copy_n(first, n, result, iterator_category(first));
}

template <class RandomIter1, class RandomIter2>
RandomIter2 unchecked_move_backward_cat(RandomIter1 first,
                                        RandomIter2 last,
                                        RandomIter2 result,
                                        Mystl::random_access_iterator_tag) {
  for (auto n = last - first; n > 0; --n) {
    *--result = Mystl::move(*--last);
  }

  return result;
}

/*****************************************************************************************/
// move
// 把 [first, last)区间内的元素移动到 [result, result + (last - first))内
/*****************************************************************************************/
// input_iterator_tag 版本
template <class InputIter, class OutputIter>
OutputIter unchecked_move_cat(InputIter  first,
                              InputIter  last,
                              OutputIter result,
                              Mystl::input_iterator_tag) {
  for (; first != last; ++first, ++result) {
    *result = Mystl::move(*first);
  }
  return result;
}

// ramdom_access_iterator_tag 版本
template <class RandomIter, class OutputIter>
OutputIter unchecked_move_cat(RandomIter first,
                              RandomIter last,
                              OutputIter result,
                              Mystl::random_access_iterator_tag) {
  for (auto n = last - first; n > 0; --n, ++first, ++result) {
    *result = Mystl::move(*first);
  }
  return result;
}

template <class InputIter, class OutputIter>
OutputIter unchecked_move(InputIter first, InputIter last, OutputIter result) {
  return unchecked_move_cat(first, last, result, iterator_category(first));
}

// 为 trivially_copy_assignable 类型提供特化版本
template <class Tp, class Up>
typename std::enable_if<
    std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
        std::is_trivially_move_assignable<Up>::value,
    Up*>::type
unchecked_move(Tp* first, Tp* last, Up* result) {
  const size_t n = static_cast<size_t>(last - first);
  if (n != 0) std::memmove(result, first, n * sizeof(Up));
  return result + n;
}

template <class InputIter, class OutputIter>
OutputIter move(InputIter first, InputIter last, OutputIter result) {
  return unchecked_move(first, last, result);
}

/*****************************************************************************************/
// move_backward
// 将 [first, last)区间内的元素移动到 [result - (last - first), result)内
/*****************************************************************************************/
// bidirectional_iterator_tag 版本
template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2 unchecked_move_backward_cat(
    BidirectionalIter1 first,
    BidirectionalIter1 last,
    BidirectionalIter2 result,
    Mystl::bidirectional_iterator_tag) {
  while (first != last)
    *--result = Mystl::move(*--last);
  return result;
}

// random_access_iterator_tag 版本
template <class RandomIter1, class RandomIter2>
RandomIter2 unchecked_move_backward_cat(RandomIter1 first,
                                        RandomIter1 last,
                                        RandomIter2 result,
                                        Mystl::random_access_iterator_tag) {
  for (auto n = last - first; n > 0; --n)
    *--result = Mystl::move(*--last);
  return result;
}

template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2 unchecked_move_backward(BidirectionalIter1 first,
                                           BidirectionalIter2 last,
                                           BidirectionalIter2 result) {
  return unchecked_move_backward_cat(first,
                                     last,
                                     result,
                                     iterator_category(first));
}

template <class Tp, class Up>
typename std::enable_if<
    std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
        std::is_trivially_move_constructible<Up>::value,
    Up*>::type
unchecked_move_backward(Tp* first, Tp* last, Up* result) {
  const size_t n = static_cast<size_t>(last - first);
  if (0 != n) {
    result -= n;
    std::memmove(result, first, n * sizeof(Up));
  }

  return result;
}

template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2 move_backward(BidirectionalIter1 first,
                                 BidirectionalIter1 last,
                                 BidirectionalIter2 result) {
  return uncheck_move_backward(first, last, result);
}

/**
 * @brief equal 比较第一序列[first,last)区间上的元素是否和第二序列相同
 * @tparam InputIterator1
 * @tparam InputIterator2
 * @param  first1           My Pan doc
 * @param  last1            My Pan doc
 * @param  first2           My Pan doc
 * @return true
 * @return false
 * */
template <class InputIterator1, class InputIterator2>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
  for (; first1 != last1; ++first1, ++first2) {
    if (*first1 != *first2) {
      return false;
    }
  }

  return true;
}

template <class BidirectionalIter1, class BidirectionalIter2, class Compare>
bool equal(BidirectionalIter1 first1,
           BidirectionalIter1 last1,
           BidirectionalIter2 first2,
           Compare            comp) {
  for (; first1 != last1; ++first1, ++first2) {
    if (!comp(*first1, *first2)) {
      return false;
    }
  }
  return true;
}

/**
 * @brief fill n
 * @tparam OutputIter
 * @tparam Size
 * @tparam T
 * @param  first            My Pan doc
 * @param  n                My Pan doc
 * @param  value            My Pan doc
 * @return OutputIter
 * */
template <class OutputIter, class Size, class T>
OutputIter unchecked_fill_n(OutputIter first, Size n, const T& value) {
  for (; n > 0; --n, ++first) {
    *first = value;
  }
  return first;
}

template <class Tp, class Size, class Up>
typename std::enable_if<std::is_integral<Tp>::value && sizeof(Tp) == 1 &&
                            !std::is_same<Tp, bool>::value &&
                            std::is_integral<Up>::value && sizeof(Up) == 1,
                        Tp*>::type
unchecked_fill_n(Tp* first, Size n, Up value) {
  if (n > 0) {
    std::memmove(first, (unsigned char)value, (size_t)(n));
  }

  return first + n;
}

template <class OutputIter, class Size, class T>
OutputIter fill_n(OutputIter first, Size n, const T& value) {
  return unchecked_fill_n(first, n, value);
}

template <class ForwardIter, class T>
void fill_cat(ForwardIter first,
              ForwardIter last,
              const T&    value,
              Mystl::forward_iterator_tag) {
  for (; first != last; ++first) {
    *first = value;
  }
}

template <class RandomIter, class T>
void fill_cat(RandomIter first,
              RandomIter last,
              const T&   value,
              Mystl::random_access_iterator_tag) {
  fill_n(first, last - first, value);
}

template <class ForwardIter, class T>
void fill(ForwardIter first, ForwardIter last, const T& value) {
  fill_cat(first, last, value, iterator_category(first));
}

/**
 * @brief lexicographical_compare
 * 以字典序排列对两个序列进行比较，当在某个位置发现第一组不相等元素时，有下列几种情况：
 * (1)如果第一序列的元素较小，返回 true ，否则返回 false
 * (2)如果到达 last1 而尚未到达 last2 返回 true
 * (3)如果到达 last2 而尚未到达 last1 返回 false
 * (4)如果同时到达 last1 和 last2 返回 false
 * @tparam InputIter1
 * @tparam InputIter2
 * @param  first1           My Pan doc
 * @param  last1            My Pan doc
 * @param  first2           My Pan doc
 * @param  last2            My Pan doc
 * @return true
 * @return false
 * */
template <class InputIter1, class InputIter2>
bool lexicographical_compare(InputIter1 first1,
                             InputIter1 last1,
                             InputIter2 first2,
                             InputIter2 last2) {
  for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
    if (*first1 < *first2) return true;
    if (*first2 > *first1) return false;
  }

  return first1 == last1 && first2 == last2;
}

template <class InputIter1, class InputIter2, class Compare>
bool lexicographical_compare(InputIter1 first1,
                             InputIter1 last1,
                             InputIter2 first2,
                             InputIter2 last2,
                             Compare    comp) {
  for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
    if (comp(*first1, *first2)) return true;
    if (comp(*first2, *first1)) return false;
  }
  return first1 == last1 && first2 == last2;
}

bool lexicographical_compare(const unsigned char* first1,
                             const unsigned char* last1,
                             const unsigned char* first2,
                             const unsigned char* last2) {
  const auto len1 = last1 - first1;
  const auto len2 = last2 - first2;

  const auto result = std::memcmp(first1, first2, Mystl::min(len1, len2));

  return result != 0 ? result < 0 : len1 < len2;
}

/**
 * @brief
 * 平行比较两个序列，找到第一处失配的元素，返回一对迭代器，分别指向两个序列中失配的元素
 * @tparam InputIter1
 * @tparam InputIter2
 * @param first1
 * @param last1
 * @param first2
 * @return
 */
template <class InputIter1, class InputIter2>
Mystl::pair<InputIter1, InputIter2> mismatch(InputIter1 first1,
                                             InputIter1 last1,
                                             InputIter2 first2) {
  while (first1 != last1 && *first1 == *first2) {
    ++first1;
    ++first2;
  }

  return Mystl::pair<InputIter1, InputIter2>(first1, first2);
}

template <class InputIter1, class InputIter2, class Compare>
Mystl::pair<InputIter1, InputIter2> mismatch(InputIter1 first1,
                                             InputIter1 last1,
                                             InputIter2 first2,
                                             Compare    comp) {
  while (first1 != last1 && comp(*first1, *first2)) {
    ++first1;
    ++first2;
  }

  return Mystl::pair<InputIter1, InputIter2>(first1, first2);
}

}  // namespace Mystl

#endif /* __ALGO_BASE_H__ */
