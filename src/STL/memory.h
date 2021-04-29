/*************************************************************************************
 * Description  : 高级内存动态管理
 * Version      : 1.0
 * Author       : koritafei(koritafei@gmailcom)
 * Date         : 2021-04-21 17:20:46
 * LastEditors  : koritafei(koritafei@gmailcom)
 * LastEditTime : 2021-04-23 17:55:11
 * FilePath     : /STLLearn/src/STL/memory.h
 * Copyright (C) 2021 koritafei(koritafei@gmailcom). All rights reserved.
 *************************************************************************************/

#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <climits>
#include <cstddef>
#include <cstdlib>
#include <memory>
#include <type_traits>

#include "algobase.h"
#include "allocator.h"
#include "construct.h"
#include "uninitialized.h"
#include "util.h"

namespace Mystl {

/**
 * @brief 获取对象地址
 * @tparam Tp
 * @param  value            My Pan doc
 * @return constexpr Tp*
 * */
template <class Tp>
constexpr Tp* address_of(Tp& value) noexcept {
  return &value;
}

/**
 * @brief Get the buffer helper object
 * @tparam T
 * @param  len              My Pan doc
 * @return pair<T*, ptrdiff_t>
 * */
template <class T>
pair<T*, ptrdiff_t> get_buffer_helper(ptrdiff_t len, T*) {
  if (len > static_cast<ptrdiff_t>(INT_MAX / sizeof(T))) {
    len = INT_MAX / sizeof(T);
  }

  while (len > 0) {
    T* tmp = static_cast<T*>(malloc(static_cast<size_t>(len) * sizeof(T)));
    if (tmp) {
      return pair<T*, ptrdiff_t>(tmp, len);
    }
    len /= 2;  // 申请失败减小申请内存大小
  }

  return pair<T*, ptrdiff_t>(nullptr, 0);
}

template <class T>
pair<T*, ptrdiff_t> get_temporary_buffer(ptrdiff_t len) {
  return get_buffer_helper(len, static_cast<T*>(0));
}

template <class T>
pair<T*, ptrdiff_t> get_temporary_buffer(ptrdiff_t len, T*) {
  return get_buffer_helper(len, static_cast<T*>(0));
}

template <class T>
void release_temporary_buffer(T* ptr) {
  free(ptr);
}

/**
 * @brief 进行临时缓冲区的申请和释放
 * @tparam ForwardIterator
 * @tparam T
 * */
template <class ForwardIterator, class T>
class temporary_buffer {
public:
  temporary_buffer(ForwardIterator first, ForwardIterator last);

  ~temporary_buffer() {
    Mystl::destroy(buffer, buffer + len);
    free(buffer);
  }

  ptrdiff_t size() const noexcept {
    return len;
  }

  ptrdiff_t requested_size() const noexcept {
    return original_len;
  }

  T* begin() noexcept {
    return buffer;
  }

  T* end() noexcept {
    return buffer + len;
  }

private:
  ptrdiff_t original_len;  // 缓冲区申请大小
  ptrdiff_t len;           // 缓冲区实际大小
  T*        buffer;        // 指向缓冲区指针

  void allocate_buffer();
  void initialize_buffer(const T&, std::true_type) {
  }

  void initialize_buffer(const T& value, std::false_type) {
    Mystl::unchecked_fill_n(buffer, len, value);
  }

  temporary_buffer(const temporary_buffer&);
  void operator=(const temporary_buffer&);
};

template <class ForwardIterator, class T>
temporary_buffer<ForwardIterator, T>::temporary_buffer(ForwardIterator first,
                                                       ForwardIterator last) {
  try {
    len = Mystl::distance(first, last);
    allocate_buffer();
    if (len > 0) {
      initialize_buffer(*first, std::is_trivially_default_constructible<T>());
    }
  } catch (...) {
    free(buffer);
    buffer = nullptr;
    len    = 0;
  }
}

template <class ForwardIterator, class T>
void temporary_buffer<ForwardIterator, T>::allocate_buffer() {
  original_len = len;
  if (len > static_cast<ptrdiff_t>(INT_MAX / sizeof(T))) {
    len = INT_MAX / sizeof(T);
  }
  while (len > 0) {
    buffer = static_cast<T*>(malloc(len * sizeof(T)));
    if (buffer) break;
    len /= 2;
  }
}

template <class T>
class auto_ptr {
public:
  typedef T elem_type;

  explicit auto_ptr(T* ptr = nullptr) : m_ptr(ptr) {
  }

  auto_ptr(auto_ptr& rhs) : m_ptr(rhs.release()) {
  }

  template <class U>
  auto_ptr(auto_ptr<U>& rhs) : m_ptr(rhs.release()) {
  }

  auto_ptr& operator=(auto_ptr& rhs) {
    delete m_ptr;
    m_ptr = rhs.release();
  }

  template <class U>

  auto_ptr& operator=(auto_ptr<U>& rhs) {
    if (this->get() != rhs.get()) {
      delete m_ptr;
      m_ptr = rhs.release();
    }
    return *this;
  }

  ~auto_ptr() {
    delete m_ptr;
  }

  T& operator*() const {
    return *m_ptr;
  }

  T* operator->() const {
    return m_ptr;
  }

  T* get() const {
    return m_ptr;
  }

  T* release() {
    T* tmp = m_ptr;
    m_ptr  = nullptr;
    return tmp;
  }

  void reset(T* p = nullptr) {
    if (m_ptr != p) {
      delete m_ptr;
      m_ptr = p;
    }
  }

private:
  T* m_ptr;
};

}  // namespace Mystl

#endif /* __MEMORY_H__ */