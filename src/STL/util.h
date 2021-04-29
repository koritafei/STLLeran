/**
 * @Copyright (c) 2021  koritafei
 * @file util.h
 * @brief
 * @author koritafei (koritafei@gmail.com)
 * @version 0.1
 * @date 2021-04-21 11:04:72
 *
 * */

#ifndef __UTIL_H__
#define __UTIL_H__

#include <cstddef>
#include <type_traits>

#include "type_traits.h"

namespace Mystl {

// move
template <class T>
typename std::remove_reference<T>::type &&move(T &&arg) noexcept {
  return static_cast<typename std::remove_reference<T>::type &&>(arg);
}

// forward
template <class T>
T &&forward(typename std::remove_reference<T>::type &arg) noexcept {
  return static_cast<T &&>(arg);
}

template <class T>
T &&forward(typename std::remove_reference<T>::type &&arg) noexcept {
  static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
  return static_cast<T &&>(arg);
}

// swap
template <class Tp>
void swap(Tp &lhs, Tp &rhs) {
  auto tmp(Mystl::move(lhs));
  lhs = Mystl::move(rhs);
  rhs = Mystl::move(tmp);
}

template <class ForwardIter1, class ForwardIter2>
ForwardIter2 swap_range(ForwardIter1 first1,
                        ForwardIter1 last1,
                        ForwardIter2 first2) {
  for (; first1 != last1; ++first1, (void)++first2)
    Mystl::swap(*first1, *first2);
  return first2;
}

template <class Tp, size_t N>
void swap(Tp (&a)[N], Tp (&b)[N]) {
  Mystl::swap_range(a, a + N, b);
}

// pair
// 结构体模板：pair
// 两个模板参数分别表示两个数据类型
// 用first和second来分别取出第一个数据和第二个数据
template <class Ty1, class Ty2>
struct pair {
  typedef Ty1 first_type;
  typedef Ty2 second_type;

  first_type  first;
  second_type second;

  // default constructiable
  template <class Other1 = Ty1,
            class Other2 = Ty2,
            typename     = typename std::enable_if<
                std::is_default_constructible<Other1>::value &&
                    std::is_default_constructible<Other2>::value,
                void>::type>
  constexpr pair() : first(), second() {
  }

  // implicit constructiable for this type
  template <class U1 = Ty1,
            class U2 = Ty2,
            typename std::enable_if<
                std::is_copy_constructible<U1>::value &&
                    std::is_copy_constructible<U2>::value &&
                    std::is_constructible<const U1 &, Ty1>::value &&
                    std::is_constructible<const U2 &, Ty2>::value,
                int>::type = 0>
  pair(const Ty1 &a, const Ty2 &b) : first(a), second(b) {
  }

  // implicit constructiable for other pair
  template <
      class Other1,
      class Other2,
      typename std::enable_if<std::is_constructible<Other1>::value &&
                                  std::is_constructible<Other2>::value &&
                                  std::is_constructible<Other1, Ty1>::value &&
                                  std::is_constructible<Other2, Ty2>::value,
                              int>::type = 0>
  constexpr pair(pair<Other1, Other2> &&other)
      : first(Mystl::forward<Other1>(other.first)),
        second(Mystl::forward<Other2>(other.second)) {
  }

  // explicit constructiable for other pair
  template <
      class Other1,
      class Other2,
      typename std::enable_if<std::is_constructible<Ty1, Other1>::value &&
                                  std::is_constructible<Ty2, Other2>::value &&
                                  (!std::is_convertible<Other1, Ty1>::value ||
                                   !std::is_convertible<Other2, Ty2>::value),
                              int>::type = 0>
  explicit constexpr pair(pair<Other1, Other2> &&other)
      : first(Mystl::forward<Other1>(other.first)),
        second(Mystl::forward<Other2>(other.second)) {
  }

  // copy assign for this pair
  pair &operator=(const pair &rhs) {
    if (this != &rhs) {
      first  = rhs.first;
      second = rhs.second;
    }
    return *this;
  }

  // move assign for this pair
  pair &operator=(pair &rhs) {
    if (this != &rhs) {
      first  = Mystl::move(rhs.first);
      second = Mystl::move(rhs.first);
    }
    return *this;
  }

  // copy assign for other pair
  template <typename Other1, typename Other2>
  pair &operator=(const pair<Other1, Other2> &other) {
    first  = other.first;
    second = other.second;
    return *this;
  }

  // move assign for other pair
  template <typename Other1, typename Other2>
  pair &operator=(pair<Other1, Other2> &&other) {
    first  = Mystl::forward<Other1>(other.first);
    second = Mystl::forward<Other2>(other.second);

    return *this;
  }

  ~pair() = default;

  void swap(pair &other) {
    if (this != other) {
      Mystl::swap(first, other.first);
      Mystl::swap(second, other.second);
    }
  }
};

// 重载比较操作符
template <typename Ty1, typename Ty2>
bool operator==(const pair<Ty1, Ty2> &lhs, const pair<Ty1, Ty2> &rhs) {
  return lhs.first == rhs.first && lhs.second == rhs.second;
}

template <typename Ty1, typename Ty2>
bool operator<(const pair<Ty1, Ty2> &lhs, const pair<Ty1, Ty2> &rhs) {
  return lhs.first < rhs.first ||
         (lhs.first == rhs.first && lhs.second < rhs.second);
}

template <typename Ty1, typename Ty2>
bool operator!=(const pair<Ty1, Ty2> &lhs, const pair<Ty1, Ty2> &rhs) {
  return !(lhs == rhs);
}

template <typename Ty1, typename Ty2>
bool operator>(const pair<Ty1, Ty2> &lhs, const pair<Ty1, Ty2> &rhs) {
  return rhs < lhs;
}

template <typename Ty1, typename Ty2>
bool operator<=(const pair<Ty1, Ty2> &lhs, const pair<Ty1, Ty2> &rhs) {
  return !(rhs < lhs);
}

template <typename Ty1, typename Ty2>
bool operator>=(const pair<Ty1, Ty2> &lhs, const pair<Ty1, Ty2> &rhs) {
  return !(lhs < rhs);
}

// 重载swap
template <class Ty1, class Ty2>
void swap(pair<Ty1, Ty2> &lhs, pair<Ty1, Ty2> &rhs) {
  lhs.swap(rhs);
}

// 全局函数，两个数字组成一个pair
template <typename Ty1, typename Ty2>
pair<Ty1, Ty2> make_pair(Ty1 &&first, Ty2 &&second) {
  return pair<Ty1, Ty2>(Mystl::forward<Ty1>(first),
                        Mystl::forward<Ty2>(second));
}

}  // namespace Mystl

#endif /* __UTIL_H__ */
