/**
 * @Copyright (c) 2021  koritafei
 * @file exceptdef.h
 * @brief 定义异常
 * @author koritafei (koritafei@gmail.com)
 * @version 0.1
 * @date 2021-04-21 17:04:34
 *
 * */

#ifndef __EXCEPTDEF_H__
#define __EXCEPTDEF_H__

#include <cassert>
#include <stdexcept>

namespace Mystl {

#define MYSTL_DEBUG(expr) assert(expr)

#define THROW_LENGTH_ERROR_IF(expr, what)                                      \
  if ((expr)) throw std::length_error(what)

#define THROW_OUT_OF_RANGE_IF(expr, what)                                      \
  if ((expr)) throw std::out_of_range(what)

#define THROW_RUNTIME_ERROR_IF(expr, what)                                     \
  if ((expr)) throw std::runtime_error(what)

}  // namespace Mystl

#endif /* __EXCEPTDEF_H__ */
