/*************************************************************************************
 * Description  : vector 实现
 * Version      : 1.0
 * Author       : koritafei(koritafei@gmail.com)
 * Date         : 2021-04-28 14:01:41
 * LastEditors  : koritafei(koritafei@gmail.com)
 * LastEditTime : 2021-04-29 18:22:31
 * FilePath     : /STLLearn/src/STL/Vector.h
 * Copyright (C) 2021 koritafei(koritafei@gmail.com). All rights reserved.
 *************************************************************************************/

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <initializer_list>

#include "exceptdef.h"
#include "iterator.h"
#include "memory.h"
#include "util.h"

namespace Mystl {

#ifdef max
#pragma message("#undefing macro max")
#endif  // max

#ifdef min
#pragma message("#undefing macro min")
#endif  // min

template <class T>
class vector {
  static_assert(!std::is_same<bool, T>::value,
                "vector<bool> is abandoned in Mystl");

public:
  typedef Mystl::allocator<T> allocator_type;
  typedef Mystl::allocator<T> data_allocator;

  typedef typename allocator_type::value_type      value_type;
  typedef typename allocator_type::pointer         pointer;
  typedef typename allocator_type::const_pointer   const_pointer;
  typedef typename allocator_type::reference       reference;
  typedef typename allocator_type::const_reference const_reference;
  typedef typename allocator_type::size_type       size_type;
  typedef typename allocator_type::difference_type difference_type;

  typedef value_type*                             iterator;
  typedef const value_type*                       const_iterator;
  typedef Mystl::reverse_iterator<iterator>       reverse_iterator;
  typedef Mystl::reverse_iterator<const_iterator> const_reverse_iterator;

  allocator_type get_allocator() {
    return data_allocator();
  }

  vector() noexcept {
    try_init();
  }

  explicit vector(size_type n) {
    fill_init(n, value_type());
  }

  vector(size_type n, const value_type& value) {
    fill_init(n, value);
  }

  template <class Iter,
            typename std::enable_if<Mystl::is_input_iterator<Iter>::value,
                                    int>::type = 0>
  vector(Iter first, Iter last) {
    MYSTL_DEBUG(!(last < first));
    range_init(first, last);
  }

  vector(const vector& rhs) {
    range_init(rhs.begin_, rhs.end_);
  }

  vector(vector&& rhs) noexcept
      : begin_(rhs.begin_), end_(rhs.end_), cap_(rhs.cap_) {
    rhs.begin_ = nullptr;
    rhs.end_   = nullptr;
    rhs.cap_   = nullptr;
  }

  vector(std::initializer_list<value_type> ilist) {
    range_init(ilist.begin(), ilist.end());
  }

  vector& operator=(const vector& rhs);
  vector& operator=(vector&& rhs) noexcept;

  vector& operator=(std::initializer_list<value_type> ilist) {
    vector tmp(ilist.begin(), ilist.end());
    swap(tmp);
    return *this;
  }

  ~vector() {
    destroy_and_recover(begin_, end_, cap_ - begin_);
    begin_ = end_ = cap_ = nullptr;
  }

  // 迭代器相关操作
  iterator begin() noexcept {
    return begin_;
  }

  const_iterator begin() const noexcept {
    return begin_;
  }

  iterator end() noexcept {
    return end_;
  }

  const_iterator end() const noexcept {
    return end_;
  }

  reverse_iterator rbegin() noexcept {
    return reverse_iterator(end());
  }

  const_reverse_iterator rbegin() const noexcept {
    return reverse_iterator(end());
  }

  reverse_iterator rend() noexcept {
    return reverse_iterator(begin());
  }

  const_reverse_iterator rend() const noexcept {
    return reverse_iterator(begin());
  }

  const_iterator cbegin() noexcept {
    return begin();
  }

  const_iterator cend() noexcept {
    return end();
  }

  const_reverse_iterator crbegin() noexcept {
    return rbegin();
  }

  const_reverse_iterator crend() noexcept {
    return rend();
  }

  // 容量相关
  bool empty() const noexcept {
    return begin_ == end_;
  }

  size_type size() const noexcept {
    return static_cast<size_type>(end_ - begin_);
  }

  size_type max_size() const noexcept {
    return static_cast<size_type>(-1) / sizeof(T);
  }

  size_type capacity() const noexcept {
    return static_cast<size_type>(cap_ - begin_);
  }

  void reserve(size_type n);
  void shrink_to_fit();

  // 访问元素相关操作
  reference operator[](size_type n) {
    MYSTL_DEBUG(n < size());
    return *(begin_ + n);
  }

  const_reference operator[](size_type n) const {
    MYSTL_DEBUG(n < size());
    return *(begin_ + n);
  }

  reference at(size_type n) {
    THROW_OUT_OF_RANGE_IF(!(n < size()),
                          "vector<T>::at() subscript out of range");
    return (*this)[n];
  }

  const_reference at(size_type n) const {
    THROW_OUT_OF_RANGE_IF(!(n < size()),
                          "vector<T>::at() subscript out of range");
    return (*this)[n];
  }

  reference front() {
    MYSTL_DEBUG(!empty());
    return *begin_;
  }

  const_reference front() const {
    MYSTL_DEBUG(!empty());
    return *begin_;
  }

  reference back() {
    MYSTL_DEBUG(!empty());
    return *(end_ - 1);
  }

  const_reference back() const {
    MYSTL_DEBUG(!empty());
    return *(end_ - 1);
  }

  pointer data() noexcept {
    return begin_;
  }

  const_pointer data() const {
    return begin_;
  }

  // 修改容器相关操作
  // assign
  void assign(size_type n, const value_type& value) {
    fill_assign(n, value);
  }

  template <class Iter,
            typename std::enable_if<Mystl::is_input_iterator<Iter>::value,
                                    int>::type = 0>
  void assign(Iter first, Iter last) {
    MYSTL_DEBUG(!(last < first));
    copy_assign(first, last, iterator_category(first));
  }

  void assign(std::initializer_list<value_type> il) {
    copy_assign(il.begin(), il.end(), Mystl::forward_iterator_tag{});
  }

  // emplace / emplace back
  template <class... Args>
  iterator emplace(const_iterator pos, Args&... args);

  template <class... Args>
  void emplace_back(Args&... args);

  // push_back / pop_back
  void push_back(const value_type& value);
  void push_back(value_type&& value) {
    emplace_back(Mystl::move(value));
  }

  void pop_back();

  // insert
  iterator insert(const_iterator pos, const value_type& value);
  iterator insert(const_iterator pos, value_type&& value) {
    return emplace(pos, Mystl::move(value));
  }

  iterator insert(const_iterator pos, size_type n, const value_type& value) {
    MYSTL_DEBUG(pos >= begin() && pos <= end());
    return fill_insert(const_cast<iterator>(pos), n, value);
  }

  template <class Iter,
            typename std::enable_if<Mystl::is_input_iterator<Iter>::value,
                                    int>::type = 0>
  void insert(const_iterator pos, Iter first, Iter laser) {
  }

  // erase / clear
  iterator erase(const_iterator pos);
  iterator erase(const_iterator first, const_iterator last);

  void clear() {
    erase(begin(), end());
  }

  // resize / reverse
  void resize(size_type new_size) {
    return resize(new_size, value_type());
  }

  void resize(size_type new_size, const value_type& value);

  void reverse() {
    Mystl::reverse(begin(), end());
  }
  
  

private:
  iterator begin_;  // 使用空间的头部
  iterator end_;    // 使用空间的尾部
  iterator cap_;    // 存储空间的尾部
};
}  // namespace Mystl

#endif /*__VECTOR_H__*/
