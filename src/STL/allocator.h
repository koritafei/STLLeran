/**
 * @Copyright (c) 2021  koritafei
 * @file allocator.h
 * @brief allocator 实现, 管理内存分配，释放，对象的构造和析构
 * @author koritafei (koritafei@gmail.com)
 * @version 0.1
 * @date 2021-04-21 16:04:34
 *
 * */

#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include "construct.h"
#include "util.h"

namespace Mystl {

template <class T>
class allocator {
public:
  typedef T         value_type;
  typedef T*        pointer;
  typedef const T*  const_pointer;
  typedef T&        reference;
  typedef const T&  const_reference;
  typedef size_t    size_type;
  typedef ptrdiff_t difference_type;

  static T* allocate();
  static T* allocate(size_type n);

  static void deallocate(T* ptr);
  static void deallocate(T* ptr, size_type n);

  static void construct(T* ptr);
  static void construct(T* ptr, const T& value);
  static void construct(T* ptr, T&& value);

  template <class... Args>
  static void construct(T* ptr, Args&&... args);

  static void destroy(T* ptr);
  static void destroy(T* ptr, T* last);
};

template <typename T>
T* allocator<T>::allocate() {
  return static_cast<T*>(::operator new(sizeof(T)));
}

template <typename T>
T* allocator<T>::allocate(size_type n) {
  if (0 == n) {
    return nullptr;
  }

  return static_cast<T*>(::operator new(n * sizeof(T)));
}

template <typename T>
void allocator<T>::deallocate(T* ptr) {
  if (nullptr == ptr) return;
  ::operator delete(ptr);
}

template <typename T>
void allocator<T>::deallocate(T* ptr, size_type) {
  if (nullptr == ptr) return;
  ::operator delete(ptr);
}

template <typename T>
void allocator<T>::construct(T* ptr) {
  Mystl::construct(ptr);
}

template <typename T>
void allocator<T>::construct(T* ptr, const T& value) {
  Mystl::construct(ptr, value);
}

template <typename T>
void allocator<T>::construct(T* ptr, T&& value) {
  Mystl::construct(ptr, Mystl::move(value));
}

template <typename T>
template <typename... Args>
void allocator<T>::construct(T* ptr, Args&&... args) {
  Mystl::construct(ptr, Mystl::forward<Args>(args)....);
}

template <typename T>
void allocator<T>::destroy(T* ptr) {
  Mystl::destroy(ptr);
}

template <typename T>
void allocator<T>::destroy(T* first, T* last) {
  Mystl::destroy(first, last);
}

}  // namespace Mystl

#endif /* __ALLOCATOR_H__ */
