/**
 * @Copyright (c) 2021  koritafei
 * @file type_traits.h
 * @brief 提取类型信息
 * @author koritafei (koritafei@gmail.com)
 * @version 0.1
 * @date 2021-04-20 10:04:23
 *
 * */

#ifndef __TYPE_TRAITS_H__
#define __TYPE_TRAITS_H__

#include <type_traits>

namespace Mystl {
// helper struct

template <class T, T v>
struct m_integral_constant {
  static constexpr T value = v;
};

template <bool b>
using m_bool_constant = m_integral_constant<bool, b>;

typedef m_bool_constant<true>  m_true_type;
typedef m_bool_constant<false> m_false_type;

template <class T1, class T2>
struct pair;

template <class T>
struct is_pair : Mystl::m_false_type {};

template <class T1, class T2>
struct is_pair<Mystl::pair<T1, T2>> : Mystl::m_true_type {};

}  // namespace MyMystl

#endif /* __TYPE_TRAITS_H__ */
