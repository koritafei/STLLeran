/**
 * @Copyright (c) 2021  koritafei
 * @file construct.h
 * @brief
 * @author koritafei (koritafei@gmail.com)
 * @version 0.1
 * @date 2021-04-21 11:04:95
 *
 * */

#ifndef __CONSTRUCT_H__
#define __CONSTRUCT_H__

#include <new>
#include <type_traits>

#include "iterator.h"
#include "type_traits.h"

namespace Mystl {
template <class Ty>
void construct(Ty *ptr) {
  ::new ((void *)ptr) Ty();
}

template <class Ty1, class Ty2>
void construct(Ty1 *ptr, const Ty2 &value) {
  ::new ((void *)ptr) Ty1(value);
}

template <class Ty, class... Args>
void construct(Ty *ptr, Args &&...args) {
  ::new ((void *)ptr) Ty(Mystl::forward<Args>(args)...);
}

// destroy 将对象析构
template <class Ty>
void destroy_one(Ty *, std::true_type) {
}

template <class Ty>
void destroy_one(Ty *pointer, std::false_type) {
  if (pointer != nullptr) {
    pointer->~Ty();
  }
}

template <class ForwardIter>
void destroy_cat(ForwardIter, ForwardIter, std::true_type) {
}

template <class Ty>
void destroy(Ty *pointer) {
  destroy_one(pointer, std::is_trivially_destructible<Ty>{});
}

template <class ForwardIter>
void destroy(ForwardIter first, ForwardIter last) {
  destroy_cat(first,
              last,
              std::is_trivially_destructible<
                  typename iterator_traits<ForwardIter>::value_type>{});
}

}  // namespace Mystl

#endif /* __CONSTRUCT_H__ */
