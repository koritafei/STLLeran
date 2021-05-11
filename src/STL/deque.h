/**
 * @ Description  :
 * @ Version      : 1.0
 * @ Author       : koritafei(koritafei@gmail.com)
 * @ Date         : 2021-05-08 18:20:30
 * @ LastEditors  : koritafei(koritafei@gmail.com)
 * @ LastEditTime : 2021-05-11 10:49:25
 * @ FilePath     : /STLLearn/src/STL/deque.h
 * @ Copyright (C) 2021 koritafei(koritafei@gmail.com). All rights reserved.
 * */

#ifndef __DEQUEUE_H__
#define __DEQUEUE_H__

#include <initializer_list>

#include "exceptdef.h"
#include "iterator.h"
#include "memory.h"
#include "util.h"

namespace Mystl {

#ifdef max
#pragma message("#undefing macro max")
#undef max
#endif  // max

#ifdef min
#pragma message("#undefing macro min")
#undef min
#endif  // min

// defaut size 8
#ifndef DEQUE_MAP_INIT_SIZE
#define DEQUE_MAP_INIT_SIZE 8
#endif  // DEQUE_MAP_INIT_SIZE

}  // namespace Mystl

#endif /* __DEQUEUE_H__ */
