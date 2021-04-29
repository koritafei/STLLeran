/*************************************************************************************
 * Description  : list 模板类
 * Version      : 1.0
 * Author       : koritafei(koritafei@gmailcom)
 * Date         : 2021-04-21 17:20:56
 * LastEditors  : koritafei(koritafei@gmailcom)
 * LastEditTime : 2021-04-26 15:12:00
 * FilePath     : /STLLearn/src/STL/list.h
 * Copyright (C) 2021 koritafei(koritafei@gmailcom). All rights reserved.
 *************************************************************************************/

#ifndef __LIST_H__
#define __LIST_H__

#include <initializer_list>
#include <type_traits>

#include "algobase.h"
#include "exceptdef.h"
#include "functional.h"
#include "iterator.h"
#include "memory.h"
#include "util.h"

namespace Mystl {

template <class T>
struct list_node_base;

template <class T>
struct list_node;

template <class T>
struct node_traits {
  typedef list_node_base<T> *base_ptr;
  typedef list_node<T> *     node_ptr;
};

template <class T>
struct list_node_base {
  typedef typename node_traits<T>::base_ptr base_ptr;
  typedef typename node_traits<T>::node_ptr node_ptr;

  base_ptr prev;  // 前一个节点
  base_ptr next;  // 下一个节点

  node_ptr as_node() {
    return static_cast<node_ptr>(self());
  }

  void unlink() {
    prev = next = self();
  }

  base_ptr self() {
    return static_cast<node_ptr>(&*this);
  }
};

template <class T>
struct list_node : public list_node_base<T> {
  typedef typename node_traits<T>::base_ptr base_ptr;
  typedef typename node_traits<T>::node_ptr node_ptr;

  T value;  // 数据域

  list_node() = default;
  list_node(const T &v) : value(v) {
  }
  list_node(T &&v) : value(Mystl::move(v)) {
  }

  base_ptr as_base() {
    return static_cast<base_ptr>(&*this);
  }

  node_ptr self() {
    return static_cast<node_ptr>(&*this);
  }
};

template <class T>
struct list_iterator
    : public Mystl::iterator<Mystl::bidirectional_iterator_tag, T> {
  typedef T                                 value_type;
  typedef T *                               pointer;
  typedef T &                               reference;
  typedef typename node_traits<T>::base_ptr base_ptr;
  typedef typename node_traits<T>::node_ptr node_ptr;
  typedef list_iterator<T>                  self;

  base_ptr node_;  // 指向当前节点

  list_iterator() = default;
  list_iterator(base_ptr x) : node_(x) {
  }
  list_iterator(node_ptr x) : node_(x->as_base()) {
  }

  list_iterator(const list_iterator &rhs) : node_(rhs.node_) {
  }

  // 重载操作符
  reference operator*() const {
    return node_->as_node()->value;
  }

  pointer operator->() const {
    return &(operator*());
  }

  self &operator++() {
    MYSTL_DEBUG(node_ != nullptr);
    node_ = node_->next;

    return *this;
  }

  self operator++(int) {
    self tmp = *this;
    ++*this;
    return tmp;
  }

  self &operator--() {
    MYSTL_DEBUG(node_ != nullptr);
    node_ = node_->prev;
    return *this;
  }

  self &operator--(int) {
    self tmp = *this;
    --*this;
    return tmp;
  }

  // 重载比较操作符
  bool operator==(const self &rhs) const {
    return node_ == rhs.node_;
  }

  bool operator!=(const self &rhs) const {
    return node_ != rhs.node_;
  }
};

template <class T>
struct list_const_iterator : public iterator<bidirectional_iterator_tag, T> {
  typedef T                                 value_type;
  typedef const T *                         pointer;
  typedef const T &                         reference;
  typedef typename node_traits<T>::base_ptr base_ptr;
  typedef typename node_traits<T>::node_ptr node_ptr;
  typedef list_const_iterator<T>            self;

  base_ptr node_;

  list_const_iterator() = default;
  list_const_iterator(base_ptr x) : node_(x) {
  }
  list_const_iterator(node_ptr x) : node_(x->as_base()) {
  }

  list_const_iterator(const list_iterator<T> &rhs) : node_(rhs.node_) {
  }

  list_const_iterator(const list_const_iterator &rhs) : node_(rhs.node_) {
  }

  reference operator*() const {
    return node_->as_node()->value;
  }

  pointer operator->() const {
    return &(operator*());
  }

  self &operator++() {
    MYSTL_DEBUG(node_ != nullptr);
    node_ = node_->next;
    return *this;
  }

  self &operator++(int) {
    self tmp = *this;
    ++*this;
    return tmp;
  }

  self &operator--() {
    MYSTL_DEBUG(node_ != nullptr);
    node_ = node_->prev;
    return *this;
  }

  self &operator--(int) {
    self tmp = *this;
    --*this;
    return tmp;
  }

  // 重载比较操作符
  bool operator==(const self &rhs) const {
    return node_ == rhs.node_;
  }

  bool operator!=(const self &rhs) const {
    return node_ != rhs.node_;
  }
};

template <class T>
class list {
public:
  // list 的嵌套类别定义
  typedef Mystl::allocator<T>                 allocator_type;
  typedef Mystl::allocator<T>                 data_allocator;
  typedef Mystl::allocator<list_node_base<T>> base_allocator;
  typedef Mystl::allocator<list_node<T>>      node_allocator;

  typedef typename allocator_type::value_type      value_type;
  typedef typename allocator_type::pointer         pointer;
  typedef typename allocator_type::const_pointer   const_pointer;
  typedef typename allocator_type::reference       reference;
  typedef typename allocator_type::const_reference const_reference;
  typedef typename allocator_type::size_type       size_type;
  typedef typename allocator_type::difference_type difference_type;

  typedef list_iterator<T>                        iterator;
  typedef list_const_iterator<T>                  const_iterator;
  typedef Mystl::reverse_iterator<iterator>       reverse_iterator;
  typedef Mystl::reverse_iterator<const_iterator> const_reverse_iterator;

  typedef typename node_traits<T>::base_ptr base_ptr;
  typedef typename node_traits<T>::node_ptr node_ptr;

  allocator_type get_allocator() {
    return node_allocator();
  }

  list() {
    fill_init(0, value_type());
  }

  explicit list(size_type n) {
    fill_init(n, value_type());
  }

  list(size_type n, const T &value) {
    fill_init(n, value);
  }

  template <class Iter,
            typename std::enable_if<Mystl::is_input_iterator<Iter>::value_type,
                                    int>::type = 0>
  list(Iter first, Iter last) {
    copy_init(first, last);
  }

  list(std::initializer_list<T> ilist) {
    copy_init(ilist.begin(), ilist.end());
  }

  list(const list &rhs) {
    copy_init(rhs.cbegin(), rhs.cend());
  }

  list(list &&rhs) noexcept : node_(rhs.node_), size_(rhs.size_) {
    rhs.node_ = nullptr;
    rhs.size_ = 0;
  }

  list &operator=(const list &rhs) {
    if (this != &rhs) {
      assign(rhs.begin(), rhs.end());
    }

    return *this;
  }

  list &operator=(list &&rhs) noexcept {
    clear();
    splice(end(), rhs);
    return *this;
  }

  list &operator=(std::initializer_list<T> ilist) {
    list tmp(ilist.begin(), ilist.end());
    swap(tmp);

    return *this;
  }

  ~list() {
    if (node_) {
      clear();
      base_allocator::deallocate(node_);
      node_ = nullptr;
      size_ = 0;
    }
  }

  // 迭代器相关操作
  iterator begin() noexcept {
    return node_->next;
  }

  const_iterator begin() const noexcept {
    return node_->next;
  }

  iterator end() noexcept {
    return node_;
  }

  const_iterator end() const noexcept {
    return node_;
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

  const_iterator cbegin() const noexcept {
    return begin();
  }

  const_iterator cend() const noexcept {
    return end();
  }

  const_reverse_iterator crbegin() const noexcept {
    return rbegin();
  }

  const_reverse_iterator crend() const noexcept {
    return rend();
  }

  // 容量相关操作
  bool empty() const noexcept {
    return node_->next == node_;
  }

  size_type size() const noexcept {
    return size_;
  }

  size_type max_size() const noexcept {
    return static_cast<size_type>(-1);
  }

  // 访问元素相关
  reference front() {
    MYSTL_DEBUG(!empty());
    return *begin();
  }

  const_reference front() const noexcept {
    MYSTL_DEBUG(!empty());
    return *begin();
  }

  reference back() {
    MYSTL_DEBUG(!empty());
    return *(--end());
  }

  const_reference back() const noexcept {
    MYSTL_DEBUG(!empty());
    return *(--end());
  }

  // 调整容器相关操作
  // assign
  void assign(size_type n, const value_type &value) {
    fill_assign(n, value);
  }

  template <class Iter,
            typename std::enable_if<Mystl::is_input_iterator<Iter>::value,
                                    int>::type = 0>
  void assign(Iter first, Iter last) {
    copy_assign(first, last);
  }

  void assign(std::initializer_list<T> ilist) {
    copy_assign(ilist.begin(), ilist.end());
  }

  // emplace / emplace_front /emplace_back
  template <class... Args>
  void emplace_front(Args... args) {
    THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
    auto link_code = create_node(Mystl::forward<Args>(args)...);
    link_nodes_at_front(link_code->as_base(), link_code->as_base());
    ++size_;
  }

  template <class... Args>
  void emplace_back(Args &&...args) {
    THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
    auto link_node = create_node(Mystl::forward<Args>(args)...);
    link_nodes_ad_back(link_node->as_base(), link_node->as_base());
    ++size_;
  }

  template <class... Args>
  iterator emplace(const_iterator pos, Args &&...args) {
    THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
    auto link_node = create_node(Mystl::forward<Args>(args)....);
    link_nodes(pos.node_, link_node->as_base(), link_node->as_base());
    ++size_;
    return iterator(link_node);
  }

  // insert
  iterator insert(const_iterator pos, const value_type &value) {
    THROE_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
    auto link_node = create_node(value);
    ++size_;
    return link_iter_node(pos, link_node->as_base());
  }

  iterator insert(const_iterator pos, value_type &&value) {
    THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
    auto link_node = create_node(Mystl::move(value));
    ++size_;
    return link_iter_node(pos, link_node->as_base());
  }

  iterator insert(const_iterator pos, size_type n, const value_type &value) {
    THROW_LENGTH_ERROR_IF(size_ > max_size() - n, "list<T>'s size too big");
    return fill_insert(pos, n, value);
  }

  template <class Iter,
            typename std::enable_if<Mystl::is_input_iterator<Iter>::value,
                                    int>::type = 0>
  iterator insert(const_iterator pos, Iter first, Iter last) {
    size_type n = Mystl::distance(first, last);
    THROW_LENGTH_ERROR_IF(size_ > max_size() - n, "lsit<T>'s size too big");
    return copy_insert(pos, n, first);
  }

  // push_front / push_back
  void push_front(const value_type &value) {
    THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "lsit<T>'s size too big");
    auto link_node = create_node(value);
    link_node_at_front(link_node->as_base(), link_node->as_base());
    ++size_;
  }

  void push_front(value_type &&value) {
    emplace_front(Mystl::move(value));
  }

  void push_back(const value_type &value) {
    THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
    auto link_node = create_node(value);
    link_node_at_back(link_node->as_base(), link_node->as_base());
    ++size_;
  }

  void push_back(value_type &&value) {
    emplace_back(Mystl::move(value));
  }

  // pop_front / pop_back
  void pop_front() {
    MYSTL_DEBUG(!empty());
    auto n = node_->next;
    unlink_nodes(n, n);
    destroy_node(n->as_node());
    --size_;
  }

  void pop_back() {
    MYSTL_DEBUG(!empty());
    auto n = node_->prev;
    unlink_nodes(n, n);
    destroy_node(n->as_base());
    --size_;
  }

  // erase / clear
  iterator erase(const_iterator pos);
  iterator erase(const_iterator first, const_iterator last);

  void clear();

  // resize

  void resize(size_type new_size) {
    resize(new_size, value_type());
  }

  void resize(size_type new_size, const value_type &value);

  void swap(list &rhs) noexcept {
    Mystl::swap(node_, rhs.node_);
    Mystl::swap(size_, rhs.size_);
  }

  // list 相关操作
  void splice(const_iterator pos, list &other);
  void splice(const_iterator pos, list &other, const_iterator it);
  void splice(const_iterator pos,
              list &         other,
              const_iterator first,
              const_iterator last);

  void remove(const value_type &value) {
    remove_if([&](const value_type &v) {
      return v == value;
    });
  }

  template <typename UnaryPredicate>
  void remove_if(UnaryPredicate pred);

  void unique() {
    unique(Mystl::equal_to<T>());
  }

  template <class BinaryPredicate>
  void unique(BinaryPredicate pred);

  void merge(list &x) {
    merge(x, Mystl::less<T>());
  }

  template <class Compare>
  void merge(list &x, Compare comp);

  void sort() {
    list_sort(begin(), end(), size(), Mystl::less<T>());
  }

  template <class Compare>
  void sort(Compare comp) {
    list_sort(begin(), end(), size(), Mystl::less<T>());
  }

  void reverse();

private:
  // create / destroty node
  template <class... Args>
  node_ptr create_node(Args &&...args);
  void     destroy_node(node_ptr p);

  // initialize
  void fill_init(size_type n, const value_type &value);
  template <class Iter>
  void copy_init(Iter first, Iter last);

  // link / unlink
  iterator link_iter_node(const_iterator pos, base_ptr node);
  void     link_nodes(base_ptr p, base_ptr first, base_ptr last);
  void     link_nodes_at_front(base_ptr first, base_ptr last);
  void     link_nodes_at_back(base_ptr first, base_ptr last);
  void     unlink_nodes(base_ptr f, base_ptr l);

  // assign
  void fill_assign(size_type n, const value_type &value);
  template <class Iter>
  void copy_assign(Iter first, Iter last);

  // insert
  iterator fill_insert(const_iterator    pos,
                       size_type         n,
                       const value_type &value);

  template <class Iter>
  iterator copy_insert(const_iterator pos, size_type n, Iter first);

  // sort
  template <class Compare>
  iterator list_sort(iterator first, iterator last, size_type n, Compare comp);

  base_ptr  node_;  // 指向末尾节点
  size_type size_;  // 大小
};

/**
 * @brief 删除pos处的元素
 * @tparam T
 * @param  pos              My Pan doc
 * @return list<T>::iterator
 * */
template <class T>
typename list<T>::iterator list<T>::erase(const_iterator pos) {
  MYSTL_DEBUG(pos != cend());
  auto n    = pos.node_;
  auto next = n->next;
  unlink_nodes(n, n);
  destroy_node(n->as_node());
  --size_;
  return iterator(next);
}

/**
 * @brief 删除[first,last)内的元素
 * @tparam T
 * @param  first            My Pan doc
 * @param  last             My Pan doc
 * @return list<T>::iterator
 * */
template <class T>
typename list<T>::iterator list<T>::erase(const_iterator first,
                                          const_iterator last) {
  if (first != last) {
    unlink_nodes(first.node_, last.node_->prev);
    while (first != last) {
      auto cur = first.node_;
      ++first;
      destroy_node(cur->as_node());
      --size_;
    }
  }
  return iterator(last.node_);
}

/**
 * @brief 清空list
 * @tparam T
 * */
template <class T>
void list<T>::clear() {
  if (0 != size_) {
    auto cur = node_->next;
    for (base_ptr next = cur->next; cur != node_;
         cur = next, next = cur->next) {
      destroy_node(cur->as_node());
    }
    node_->unlink();
    size_ = 0;
  }
}

/**
 * @brief 重置容器大小
 * @tparam T
 * */
template <class T>
void list<T>::resize(size_type new_size, const value_type &value) {
  auto      i   = begin();
  size_type len = 0;
  while (i != end() && len < new_size) {
    ++i;
    ++len;
  }
  if (len == new_size) {
    erase(i, node_);
  } else {
    insert(node_, new_size - len, value);
  }
}

/**
 * @brief 将list x接合在pos之前
 * @tparam T
 * @param  pos              My Pan doc
 * @param  x                My Pan doc
 * */
template <class T>
void list<T>::splice(const_iterator pos, list &x) {
  MYSTL_DEBUG(this != &x);
  if (!x.empty()) {
    THROW_LENGTH_ERROR_IF(size_ > max_size() - x.size_,
                          "list<T>'s size too bug");
    auto f = x.node_->next;
    auto l = x.node_->prev;

    x.unlink_nodes(f, l);
    link_nodes(pos.node_, f, l);

    size_ += x.size_;
    x.size_ = 0;
  }
}

/**
 * @brief 将it所指节点至于pos之前
 * @tparam T
 * @param  pos              My Pan doc
 * @param  x                My Pan doc
 * @param  it               My Pan doc
 * */
template <class T>
void list<T>::splice(const_iterator pos, list &x, const_iterator it) {
  if (pos.node_ != it.node_ && pos.node_ != it.node_->next) {
    THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
    auto f = it.node_;

    x.unlink_nodes(f, f);
    link_nodes(pos.node_, f, f);
    ++size_;
    --x.size_;
  }
}

/**
 * @brief 将list x的[first, last)内的节点至于pos之前
 * @tparam T
 * @param  pos              My Pan doc
 * @param  x                My Pan doc
 * @param  first            My Pan doc
 * @param  last             My Pan doc
 * */
template <class T>
void list<T>::splice(const_iterator pos,
                     list &         x,
                     const_iterator first,
                     const_iterator last) {
  if (first != last && this != &x) {
    size_type n = Mystl::distance(first, last);
    THROW_LENGTH_ERROR_IF(size_ > max_size() - n, "lsit<T>'s size too big");
    auto f = first.node_;
    auto l = last.node_->prev;

    x.unlink_nodes(f, l);
    link_nodes(pos.node_, f, l);
    size_ += n;
    x.size_ -= n;
  }
}

/**
 * @brief 将另一元操作pred为true的所有的元素移除
 * @tparam T
 * @tparam UnaryPredicate
 * @param  pred             My Pan doc
 * */
template <class T>
template <class UnaryPredicate>
void list<T>::remove_if(UnaryPredicate pred) {
  auto f = begin();
  auto l = end();
  for (auto next = f; f != l; f = next) {
    ++next;
    if (pred(*f)) {
      erase(f);
    }
  }
}

/**
 * @brief 移除list中满足pred为true的元素
 * @tparam T
 * @tparam BinaryPredicate
 * @param  pred             My Pan doc
 * */
template <class T>
template <class BinaryPredicate>
void list<T>::unique(BinaryPredicate pred) {
  auto i = begin();
  auto e = end();
  auto j = i;
  ++j;
  while (j != e) {
    if (pred(*i, *j)) {
      erase(j);
    } else {
      i = j;
    }
    j = i;
    ++j;
  }
}

/**
 * @brief 合并两个list，按照comp为true的顺序
 * @tparam T
 * @tparam Compare
 * @param  x                My Pan doc
 * @param  comp             My Pan doc
 * */
template <class T>
template <class Compare>
void list<T>::merge(list &x, Compare comp) {
  if (this != &x) {
    THROW_LENGTH_ERROR_IF(size_ > max_size() - x.size_,
                          "list<T>'s size too big");
    auto f1 = begin();
    auto l1 = end();
    auto f2 = x.begin();
    auto l2 = x.end();

    while (f1 != l1 && f2 != l2) {
      if (comp(*f2, *f1)) {
        auto next = f2;
        ++next;
        for (; next != l2 && comp(*next, *f1); ++next)
          ;
        auto f = f2.node_;
        auto l = next.node_->prev;
        f2     = next;

        // link node
        x.unlink_nodes(f, l);
        link_nodes(f1.node_, f, l);
        ++f1;
      } else {
        ++f1;
      }
    }

    // 链接剩余部分
    if (f2 != l2) {
      auto f = f2.node_;
      auto l = l2.node_->prev;
      x.unlink_nodes(f, l);
      link_nodes(l1.node_, f, l);
    }

    size_ += x.size_;
    x.size_ = 0;
  }
}

/**
 * @brief 链表反转
 * @tparam T
 * */
template <class T>
void list<T>::reverse() {
  if (size_ <= 1) {
    return;
  }

  auto i = begin();
  auto e = end();

  while (i.node_ != e.node_) {
    Mystl::swap(i.node_->prev, i.node_->next);
    i.node_ = i.node_->prev;
  }

  Mystl::swap(e.node_->prev, e.node_->next);
}

/**
 * @brief 创建节点
 * @tparam T
 * @tparam Args
 * @param  args             My Pan doc
 * @return list<T>::node_ptr
 * */
template <class T>
template <class... Args>
typename list<T>::node_ptr list<T>::create_node(Args &&...args) {
  node_ptr p = node_allocator::allocate(1);
  try {
    data_allocator::constuctor(Mystl::address_of(p->value),
                               Mystl::forward<Args>(args)...);
    p->prev = nullptr;
    p->next = nullptr;
  } catch (...) {
    node_allocator::deallocate(p);
    throw;
  }
  return p;
}

/**
 * @brief 销毁节点
 * @tparam T
 * @param  p                My Pan doc
 * */
template <class T>
void list<T>::destroy_node(node_ptr p) {
  data_allocator::destroy(Mystl::address_of(p->value));
  node_allocator::deallocate(p);
}

/**
 * @brief 用n个元素初始化容器
 * @tparam T
 * @param  n                My Pan doc
 * @param  p                My Pan doc
 * */
template <class T>
void list<T>::fill_init(size_type n, const value_type &value) {
  node_ = base_allocator::allocate(1);
  node_->unlink();
  size_ = n;
  try {
    for (; n > 0; --n) {
      auto node = create_node(value);
      link_nodes_at_back(node->as_base(), node->as_base());
    }
  } catch (...) {
    clear();
    base_allocator::deallocate(node_);
    node_ = nullptr;
    throw;
  }
}

/**
 * @brief 在pos处链接一个节点
 * @tparam T
 * @param  pos              My Pan doc
 * @param  link_node        My Pan doc
 * @return list<T>::iterator
 * */
template <class T>
typename list<T>::iterator list<T>::link_iter_node(const_iterator pos,
                                                   base_ptr       link_node) {
  if (pos == node_->next) {
    link_nodes_at_front(link_node, link_node);
  } else if (pos == node_) {
    link_nodes_at_back(link_node, link_node);
  } else {
    link_nodes(pos.node_, link_node, link_node);
  }

  return iterator(link_node);
}

/**
 * @brief 在pos处连接[first, last]的节点
 * @tparam T
 * @param  pos              My Pan doc
 * @param  first            My Pan doc
 * @param  last             My Pan doc
 * */
template <class T>
void list<T>::link_nodes(base_ptr pos, base_ptr first, base_ptr last) {
  pos->prev->nexr = first;
  first->prev     = pos->prev;
  pos->prev       = last;
  last->next      = pos;
}

/**
 * @brief 在头部链接[first, last]节点
 * @tparam T
 * @param  first            My Pan doc
 * @param  last             My Pan doc
 * */
template <class T>
void list<T>::link_nodes_at_front(base_ptr first, base_ptr last) {
  first->prev      = node_;
  last->next       = node_->next;
  last->next->next = last;
  node_->next      = first;
}

/**
 * @brief 在尾部链接[first, last]节点
 * @tparam T
 * @param  first            My Pan doc
 * @param  last             My Pan doc
 * */
template <class T>
void list<T>::link_nodes_at_back(base_ptr first, base_ptr last) {
  last->next        = node_;
  first->prev       = node_->prev;
  first->prev->next = first;
  node_->prev       = last;
}

/**
 * @brief 容器与[first,last]断开链接
 * @tparam T
 * @param  first            My Pan doc
 * @param  last             My Pan doc
 * */
template <class T>
void list<T>::unlink_nodes(base_ptr first, base_ptr last) {
  first->prev->next = last->next;
  last->next->prev  = first->prev;
}

template <class T>
void list<T>::fill_assign(size_type n, const value_type &value) {
  auto i = begin();
  auto e = end();
  for (; n > 0 && i != e; --n, ++i) {
    *i = value;
  }
  if (n > 0) {
    insert(e, n, value);
  } else {
    erase(i, e);
  }
}

/**
 * @brief 复制[f2,l2)为容器赋值
 * @tparam T
 * @tparam Iter
 * @param  f2               My Pan doc
 * @param  l2               My Pan doc
 * */
template <class T>
template <class Iter>
void list<T>::copy_assign(Iter f2, Iter l2) {
  auto f1 = begin();
  auto l1 = end();
  for (; f1 != l1 && f2 != l2; ++f1, ++f2) {
    *f1 = *f2;
  }
  if (f2 == l2) {
    erase(f1, l1);
  } else {
    insert(l1, f2, l2);
  }
}

/**
 * @brief 在pos处插入n个元素
 * @tparam T
 * @param  pos              My Pan doc
 * @param  n                My Pan doc
 * @param  value            My Pan doc
 * @return list<T>::iterator
 * */
template <class T>
typename list<T>::iterator list<T>::fill_insert(const_iterator    pos,
                                                size_type         n,
                                                const value_type &value) {
  iterator r(pos.node_);
  if (n != 0) {
    const auto add_size = n;
    auto       node     = create_node(value);
    node->prev          = nullptr;
    r                   = iterator(node);
    iterator end        = r;
    try {
      for (--n; n > 0; --n, ++end) {
        auto next       = create_node(value);
        end.node_->next = next->as_base();
        next->prev      = end.node_;
      }
      size_ += add_size;
    } catch (...) {
      auto enode = end.node_;
      while (true) {
        auto prev = enode->prev;
        destroy_node(enode->as_node());
        if (prev == nullptr) break;
        enode = prev;
      }
      throw;
    }
    link_nodes(pos.node_, r.node_, end.node_);
  }

  return r;
}

/**
 * @brief 在pos处插入[first,last)的元素
 * @tparam T
 * @tparam Iter
 * @param  pos              My Pan doc
 * @param  n                My Pan doc
 * @param  first            My Pan doc
 * @return list<T>::iterator
 * */
template <class T>
template <class Iter>
typename list<T>::iterator list<T>::copy_insert(const_iterator pos,
                                                size_type      n,
                                                Iter           first) {
  iterator r(pos.node_);
  if (n != 0) {
    const auto add_size = n;
    auto       node     = create_node(*first);
    node->prev          = nullptr;
    r                   = iterator(node);
    iterator end        = r;
    try {
      for (--n, ++first; n > 0; --n, ++first, ++end) {
        auto next       = create_node(*first);
        end.node_->next = next->as_base();
        next->prev      = end.node_;
      }
      size_ += add_size;
    } catch (...) {
      auto enode = end.node_;
      while (true) {
        auto prev = enode->prev;
        destroy_node(enode->as_node());
        if (prev == nullptr) {
          break;
        }
        enode = prev;
      }
      throw;
    }

    link_nodes(pos.node_, r.node_, end.node_);
  }

  return r;
}

/**
 * @brief 对list 进行归并排序，返回一个迭代器指向区间最小元素的位置
 * @tparam T
 * @tparam Compared
 * @param  f1               My Pan doc
 * @param  l2               My Pan doc
 * @param  n                My Pan doc
 * @param  comp             My Pan doc
 * @return list<T>::iterator
 * */
template <class T>
template <class Compared>
typename list<T>::iterator list<T>::list_sort(iterator  f1,
                                              iterator  l2,
                                              size_type n,
                                              Compared  comp) {
  if (n < 2) {
    return f1;
  }
  if (n == 2) {
    if (comp(*--l2, *f1)) {
      auto ln = l2.node_;
      unlink_nodes(ln, ln);
      link_nodes(f1.node_, ln, ln);
      return l2;
    }
    return f1;
  }

  auto n2 = n / 2;
  auto l1 = f1;
  Mystl::advance(l1, n2);
  auto result = f1 = list_sort(f1, l1, n2, comp);  // 前半段最小位置
  auto f2 = l1 = list_sort(l1, l2, n - n2, comp);  // 后半段最小位置

  // 把较小的一段区间移到前面
  if (comp(*f2, *f1)) {
    auto m = f2;
    ++m;
    for (; m != l2 && comp(*m, *f1); ++m)
      ;
    auto f = f2.node_;
    auto l = m.node_->prev;
    result = f2;
    l1 = f2 = m;
    unlink_nodes(f, l);
    m = f1;
    ++m;
    link_nodes(f1.node_, f, l);
    f1 = m;
  } else {
    ++f1;
  }

  // 合并两段有序区间
  while (f1 != l1 && f2 != l2) {
    if (comp(*f1, *f2)) {
      auto m = f2;
      ++m;
      for (; m != l2 && comp(*m, *f1); ++m) {
        auto m = f2;
        ++m;
        for (; m != l2 && comp(*m, *f1); ++m)
          ;
        auto f = f2.node_;
        auto l = m.node_->prev;
        if (l1 == l2) l1 = m;
        f2 = m;
        unlink_nodes(f, l);
        m = f1;
        ++m;
        link_nodes(f1.node_, f, l);
        f1 = m;
      }
    } else {
      ++f1;
    }
  }

  return result;
}

/**
 * @brief 操作符重载
 * @tparam T
 * @param  lhs              My Pan doc
 * @param  rhs              My Pan doc
 * @return true
 * @return false
 * */
template <class T>
bool operator==(const list<T> &lhs, const list<T> &rhs) {
  auto f1 = lhs.cbegin();
  auto f2 = rhs.cbegin();
  auto l1 = lhs.cend();
  auto l2 = rhs.cend();

  for (; f1 != l1 && f2 != l2 && *f1 == *f2; ++f1, ++f2)
    ;
  return f1 == l1 && f2 == l2;
}

template <class T>
bool operator<(const list<T> &lhs, const list<T> &rhs) {
  return Mystl::lexicographical_compare(lhs.cbegin(),
                                        lhs.cend(),
                                        rhs.cbegin(),
                                        rhs.cend());
}

template <class T>
bool operator!=(const list<T> &lhs, const list<T> &rhs) {
  return !(lhs == rhs);
}

template <class T>
bool operator>(const list<T> &lhs, const list<T> &rhs) {
  return rhs < lhs;
}

template <class T>
bool operator<=(const list<T> &lhs, const list<T> &rhs) {
  return !(rhs < lhs);
}

template <class T>
bool operator>=(const list<T> &lhs, const list<T> &rhs) {
  return !(lhs < rhs);
}

// 重载 swap
template <class T>
void swap(list<T> &lhs, list<T> &rhs) noexcept {
  lhs.swap(rhs);
}

}  // namespace Mystl

#endif /* __LIST_H__ */
